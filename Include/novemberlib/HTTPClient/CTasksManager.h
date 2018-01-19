#ifndef CTASKSMANAGER_H
#define CTASKSMANAGER_H

#include "../utils/CLog.h"
#include "../utils/utils.h"
#include "../utils/json.h"
#include "../utils/md5.h"
#include "../helpers/CConfigHelper.h"
#include "../HTTPClient/IResultsStore.h"

#include <vector>
#include <queue>
#include <thread>
#include <chrono>

using nlohmann::json;
using namespace std::chrono::_V2;
using namespace std::chrono;

class CProxy;

class CTaskUserInfo
{
public:
    CTaskUserInfo()
    {
        isValid = false;
    }
    CTaskUserInfo(const std::string& token, const long sessionEndTime, const CProxy* proxy = nullptr)
    {
        this->token = token;
        this->sessionEndTime = sessionEndTime;
        this->proxy = proxy;
    }
    ~CTaskUserInfo(){}

    const std::string getToken() const
    {
        return token;
    }
    long getSessionEndTime() const
    {
        return sessionEndTime;
    }
    bool getIsValid() const
    {
        return isValid;
    }
    const CProxy* getProxy() const
    {
        return proxy;
    }
protected:
    std::string token;
    long sessionEndTime;
    bool isValid;
    const CProxy* proxy;
};

class CTaskHandler
{
public:
    CTaskHandler(const std::string& token, std::thread* thread = nullptr)
    {
        this->thread = thread;
        nextTaskTime = system_clock::now();
        this->token = token;
    }
    ~CTaskHandler()
    {
        if(thread) thread->join();
        delete thread;
    }
    const std::string getToken() const
    {
        return token;
    }
    const CTaskUserInfo& getTaskUserInfo() const
    {
        return loginInfo;
    }
    const time_point<system_clock>& getNextTaskTime() const
    {
        return nextTaskTime;
    }
    void setThread(std::thread* thread)
    {
        if(this->thread) this->thread->join();
        delete this->thread;

        this->thread = thread;
    }
    void setTaskUserInfo(const CTaskUserInfo& userInfo)
    {
        this->loginInfo = userInfo;
    }

    mutable time_point<system_clock> nextTaskTime;
private:
    std::thread* thread;
    std::string token;
    CTaskUserInfo loginInfo;
};

template<typename T>
class CTasksManager;

template<typename T>
class CTaskData
{
    public:
        friend CTasksManager<T>;
        CTaskData(const int taskType, const T* taskData, const int userId, bool storeInDB = false, const std::string& taskHash = " ")
        :   userId(userId), taskId(std::rand() % 2147483600), taskType(taskType), taskData(taskData),taskResult(nullptr),
            taskHash(taskHash), storeInDB(storeInDB)
        {
            time(&creationTime);
            if(taskHash.length() == 0) this->taskHash = md5(valueToString(taskId));
        }

        ~CTaskData()
        {
            delete taskData;
        }
        int getTaskId() const
        {
            return taskId;
        }
        int getTaskType() const
        {
            return taskType;
        }
        const T* getTaskData() const
        {
            return taskData;
        }
        const T* getTaskResult() const
        {
            return taskResult;
        }
        const CTaskHandler* getTaskHandler() const
        {
            return taskHandler;
        }
        time_t getCreationTime() const
        {
            return creationTime;
        }
        int getTaskUserId() const
        {
            return userId;
        }
        const std::string getTaskHash() const
        {
            return taskHash;
        }
        bool getIsMayStoreInDB() const
        {
            return storeInDB;
        }

        void setResult(CResult* result) const
        {
            result->setHash(taskHash);
            taskResult = result;
        }
        void setTaskHandler(const CTaskHandler* handler) const
        {
            this->taskHandler = handler;
        }
    protected:
        const int userId;
        const int taskId;
        const int taskType;
        const T* taskData;
        mutable CResult* taskResult;
        mutable const CTaskHandler* taskHandler;
        mutable std::string taskHash;
        time_t creationTime;
        bool storeInDB;

        CTaskData& operator=(const CTaskData& rhs) = delete;
        CTaskData(const CTaskData& rhs) = delete;
};

template<typename T>
class CTasksManager : public CResultsStore
{
private:
    std::queue<const CTaskData<T>*> tasks;
    std::vector<CTaskHandler*> workers;
    bool isActive;
    long sleepTimeMs;

    CTasksManager& operator=(const CTasksManager& rhs) = delete;
    CTasksManager(const CTasksManager& rhs) = delete;
public:
    CTasksManager(const std::vector<CTaskUserInfo> users)
    {
        isActive = true;

        CConfigHelper* settingsManager = CConfigHelper::getInstance();
        sleepTimeMs = settingsManager->getLongParamValue("taskSleepTimeMs", 50);

        for(auto it = users.begin(); it != users.end(); ++it)
        {
            CTaskUserInfo currUser = (*it);
            CTaskHandler* taskHandler = new CTaskHandler(currUser.getToken());
            taskHandler->setTaskUserInfo(currUser);
            taskHandler->setThread(new std::thread(&CTasksManager::ThreadLoop, this, taskHandler));
            workers.push_back(taskHandler);
        }
    }

    virtual ~CTasksManager()
    {
        isActive = false;
        for(auto it = workers.begin(); it != workers.end(); ++it)
        {
            delete (*it);
        }
        while(!tasks.empty())
        {
            const CTaskData<T>* currTask = tasks.front();
            delete currTask;
            tasks.pop();
        }
    }

    void setMinTimeInterval(const long timeInterval)
    {
        this->sleepTimeMs = timeInterval;
    }
    long getMinTimeInterval() const
    {
        return sleepTimeMs;
    }

    virtual const std::string addAndExecuteTaskOnReady( const int taskType, const T* taskData, const int userId,
                                                        const bool isCanStoreInDB = false, const std::string taskHash = "")
    {
        const CResult* result = getResult(taskHash, isCanStoreInDB);
        time_t now;
        time(&now);
        CConfigHelper* config = CConfigHelper::getInstance();
        if(result != nullptr)
        {
            time_t deltaTime = now - result->getCreationTime();
            if(deltaTime < config->getIntParamValue("resultLifetime", CResult::RESULT_LIFETIME))return result->getHash();
        }

        CTaskData<T>* newTask = new CTaskData<T>(taskType, taskData, userId, isCanStoreInDB, taskHash);
        tasks.push(newTask);

        return newTask->getTaskHash();
    }
    virtual const CResult* executeTaskNow(const CTaskHandler* handler, const int taskType, const T* taskData, const int userId,
                                         const bool isCanStoreInDB = false, const std::string& taskHash = "")
    {
        const CResult* oldResult = getResult(taskHash, isCanStoreInDB);
        time_t now;
        time(&now);
        CConfigHelper* config = CConfigHelper::getInstance();
        if(oldResult != nullptr)
        {
            time_t deltaTime = now - oldResult->getCreationTime();
            if(deltaTime < config->getIntParamValue("resultLifetime", CResult::RESULT_LIFETIME))return oldResult;
        }
        CTaskData<T>* newTask = new CTaskData<T>(taskType, taskData, userId, isCanStoreInDB, taskHash);
        newTask->setTaskHandler(handler);
        CResult* result = executeTask(newTask);
        newTask->setResult(result);
        addResult(result, isCanStoreInDB);
        delete newTask;

        return result;
    }
private:
    virtual CResult* executeTask(const CTaskData<T>* task) = 0;

    void ThreadLoop(const CTaskHandler* taskHandler)
    {
        while(isActive)
        {
            time_point<system_clock> now;
            now = system_clock::now();

            if(tasks.empty())
            {
                std::this_thread::sleep_for(milliseconds(sleepTimeMs));
            }
            else if(taskHandler->getNextTaskTime() <= now)
            {
                const CTaskData<T>* currTask = tasks.front();
                tasks.pop();

                std::string logMessage = "Task id: " + valueToString(currTask->getTaskId()) + " in work";
                CLog::getInstance()->addInfo(logMessage);
                currTask->setTaskHandler(taskHandler);
                CResult* result = executeTask(currTask);
                if(result != nullptr)
                {
                    currTask->setResult(result);
                    addResult(result, currTask->getIsMayStoreInDB());

                    logMessage = "Task id: " + valueToString(currTask->getTaskId()) + " done success";
                    CLog::getInstance()->addInfo(logMessage);
                }
                else
                {
                    logMessage = "Task id: " + valueToString(currTask->getTaskId()) + " done unsuccess";
                    CLog::getInstance()->addInfo(logMessage);
                }
                delete currTask;

                time_point<system_clock> tmp = system_clock::now() + milliseconds(sleepTimeMs);
                taskHandler->nextTaskTime = tmp;
            }
        }
    }
};

#endif // CTASKSMANAGER_H
