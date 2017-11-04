#ifndef CResultSSTORE_H
#define CResultSSTORE_H

#include "../utils/utils.h"
#include "../managers/CManagers.h"
#include "../managers/CDBManager.h"
#include "../utils/CLog.h"
#include "../utils/json.h"
#include "../helpers/CConfigHelper.h"

#include <map>
#include <mutex>
#include <chrono>
#include <thread>

using nlohmann::json;

class CResult
{
    public:
        static const int RESULT_LIFETIME = 60;
        const json* getData() const
        {
            return data;
        }
        virtual const std::string toString() const
        {
            if(data == nullptr) return "BAD";
            return data->dump();
        }
        const std::string getHash() const
        {
            return taskHash;
        }
        time_t getCreationTime() const
        {
            return creationTime;
        }
        const std::string getdescriptionString() const
        {
            return descriptionStr;
        }
        virtual void saveToDB() const
        {
            if(userId == 0) return;

            CConfigHelper* settingsManager = CConfigHelper::getInstance();
            std::string dbTableName = "";
            dbTableName = settingsManager->getStringParamValue("taskDBTableName", "");
            if(dbTableName.length() == 0) return;

            CResult* oldResult = CResult::loadFromDB(taskHash);
            CDBManager* dbManager = CManagers::getInstance()->getDBManager();
            std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

            if(oldResult != nullptr)
            {
                 dbRequest->deleteRequest(dbTableName, "`hash`='"+dbManager->getEscapeString(taskHash)+"'");
            }
            delete oldResult;

            CDBValues fields;
            fields.addValue("userId", userId);
            fields.addValue("hash", taskHash);
            fields.addValue("result", urlEncode(compressString(toString())));
            fields.addValue("creationTime", creationTime);
            fields.addValue("descriptionString", urlEncode(descriptionStr));

            dbRequest->insertRequest(fields, dbTableName);
            if(!dbRequest->getIsLastQuerySuccess())
            {
                CLog::getInstance()->addError("Insert to db error");
            }

        }
        static CResult* loadFromDB(const std::string& taskHash)
        {
            if(taskHash.length() <= 1) return nullptr;

            CConfigHelper* settingsManager = CConfigHelper::getInstance();
            std::string dbTableName = "";
            dbTableName = settingsManager->getStringParamValue("taskDBTableName", "");
            if(dbTableName.length() == 0) return nullptr;

            CDBManager* dbManager = CManagers::getInstance()->getDBManager();
            std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

            const CDBRequestResult* result = dbRequest->selectRequest(
                        CDBValues("*"), dbTableName, "`hash` = '"+dbManager->getEscapeString(taskHash)+"'");
            if(dbRequest->getIsLastQuerySuccess() && result != NULL && result->getRowsCnt() > 0)
            {
                int userId = result->getIntValue(0, 0);
                std::string text = "";
                time_t time = result->getLongValue(0, 3);
                std::string descriptionStr = urlDecode(result->getStringValue(0, 4));

                if(userId == 0) return nullptr;
                json* data = new json();
                try
                {
                    text = decompressString(urlDecode(result->getStringValue(0, 2)));
                    *data = json::parse(text);
                }
                catch(...)
                {
                    (*data)["error"] = "bad parse from db";
                }
                CResult* taskResult = new CResult(userId, data, taskHash, time, descriptionStr);
                return taskResult;
            }
            return nullptr;
        }
        static bool isResultInDB(const std::string& taskHash)
        {
            if(taskHash.length() <= 1) return false;

            CConfigHelper* settingsManager = CConfigHelper::getInstance();
            std::string dbTableName = "";
            dbTableName = settingsManager->getStringParamValue("taskDBTableName", "");
            if(dbTableName.length() == 0) return false;

            CDBManager* dbManager = CManagers::getInstance()->getDBManager();
            std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

            const CDBRequestResult* result = dbRequest->selectRequest(
                        CDBValues("*"), dbTableName, "`hash` = '"+dbManager->getEscapeString(taskHash)+"'");
            if(dbRequest->getIsLastQuerySuccess() && result != NULL && result->getRowsCnt() > 0)
            {
                return true;
            }
            return false;
        }
        void setHash(const std::string& taskHash)
        {
            this->taskHash = taskHash;
        }

        CResult(const int userId, json* data, const std::string& taskHash, const time_t creationTime, const std::string descriptionStr = "")
        {
            this->userId = userId;
            this->taskHash = taskHash;
            this->data = data;
            this->descriptionStr = descriptionStr;
            this->creationTime = creationTime;
        }
        CResult(const int userId, json* data, const std::string& taskHash, const std::string descriptionStr = "")
        {
            this->userId = userId;
            this->taskHash = taskHash;
            this->data = data;
            this->descriptionStr = descriptionStr;
            time(&creationTime);
        }
        CResult(const int userId, const std::string& taskHash)
        {
            this->userId = userId;
            this->taskHash = taskHash;
            this->data = nullptr;
            this->descriptionStr = "";
            time(&creationTime);
        }
        virtual ~CResult()
        {
            delete data;
        }
    protected:
        time_t creationTime;
        std::string taskHash;
        json* data;
        int userId;
        std::string descriptionStr;
};

class CResultsStore
{
    public:
        CResultsStore() {initVars();}
        virtual ~CResultsStore()
        {
            clearResults();
        }

        CResult* getResultFromDB(const std::string& taskHash) const
        {
            CResult* result = CResult::loadFromDB(taskHash);
            return result;
        }
        bool getIsResultExsist(const std::string& taskHash, const bool useDB = false) const
        {
            lock.lock();
            auto it = results.find(taskHash);
            lock.unlock();
            if(it != results.end()) return true;

            if(useDB && CResult::isResultInDB(taskHash))return true;
            return false;
        }
        const CResult* getResult(const std::string& taskHash, const bool seekInDB = false) const
        {
            lock.lock();
            auto it = results.find(taskHash);
            lock.unlock();

            if(it != results.end()) return it->second;
            if(seekInDB)
            {
                CResult* result = getResultFromDB(taskHash);
                if(result) addResult(result);
                return result;
            }
            return nullptr;
        }
        const CResult* waitAndGetResult(const std::string& taskHash) const
        {
            int timer = 0;
            do
            {
                timer += sleepTime;
                const CResult* result = getResult(taskHash);
                if(result != nullptr) return result;
                else std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            }while(timer < maxWaitTimeMs);

            return nullptr;
        }
        void clearResults()
        {
            for(auto it = results.begin(); it != results.end(); ++it)
            {
                CResult* currResult = it->second;
                delete currResult;
            }
            results.clear();
        }
        const std::string addResult(CResult* result, const bool useDB = false) const
        {
            std::string taskHash = "BAD";
            if(result == nullptr) return taskHash;

            const CResult* resultInCash = getResult(taskHash, false);
            if(resultInCash != nullptr) delete resultInCash;

            taskHash = result->getHash();
            lock.lock();
            results[taskHash] = result;
            lock.unlock();
            if(useDB) result->saveToDB();

            return taskHash;
        }

        void initVars()
        {
            CConfigHelper* settingsManager = CConfigHelper::getInstance();
            maxWaitTimeMs = 60000;
            sleepTime = 50;
            if(settingsManager == NULL)
            {
                CLog::getInstance()->addWarning("SettingsManager not initialized yet, maxWaitTimeMs set to def value");
            }
            else
            {
                maxWaitTimeMs = settingsManager->getLongParamValue("taskMaxWaitTimeMs", 60000);
                sleepTime = settingsManager->getLongParamValue("taskSleepTimeMs", 50);
            }
        }
    private:
        mutable std::map<std::string, CResult*> results;
        mutable std::mutex lock;

        long maxWaitTimeMs;
        long sleepTime;
};

#endif // CResultSSTORE_H
