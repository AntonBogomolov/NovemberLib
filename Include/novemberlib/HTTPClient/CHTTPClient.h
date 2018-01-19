#ifndef CHTTPCLIENT_H
#define CHTTPCLIENT_H

#include "../interfaces/ITemplateSingleton.h"
#include "../HTTPClient/IResultsStore.h"

#include <curl/curl.h>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <map>

class CProxyList;
class CProxy;

struct ProxyState
{
    bool isProxyEnabled;
    CProxyList* proxyList;
    CProxy* singleProxy;
};

class CHTTPClientRequestResult : public CResult
{
public:
    CHTTPClientRequestResult(const std::string& content, const std::string& location, const std::string& taskHash);
    ~CHTTPClientRequestResult();

    const std::string getContent() const;
    const std::string getLocation() const;
};

class CThreadPool;
class CHTTPClient : public ITemplateSingleton<CHTTPClient>, public CResultsStore
{
friend class CThreadPool;
public:
    ~CHTTPClient();
    void enableProxy();
    bool getIsProxyEnabled();
    void disableProxy();

    int createRequest(const std::string& uri, bool isSyncRequest = true,
                const std::map<std::string, std::string> getData  = std::map<std::string, std::string>(),
                const std::map<std::string, std::string> postData = std::map<std::string, std::string>(), const CProxy* proxy = nullptr);
protected:
    void threadHandler(const std::string& uri, const int requestId,
                const std::map<std::string, std::string> getData,
                const std::map<std::string, std::string> postData,const CProxy* proxy);
private:
    CThreadPool* threadPool;
    CHTTPClient();
    friend CHTTPClient* ITemplateSingleton::getInstance();

    ProxyState activeProxyState;
    ProxyState savedProxyState;
};

class CHTTPClientRequest
{
public:
    CHTTPClientRequest();
    ~CHTTPClientRequest();

    std::string getContent() const;
    std::string getLocation() const;

    static size_t contentHandler(const char* data, const size_t size, const size_t nmemb, void* p);
    static size_t locationHandler(const char* data, const size_t size, const size_t nmemb, void* p);
private:
    std::string content;
    std::string location;

    size_t contentHandlerImpl(const char* data, const size_t size, const size_t nmemb);
    size_t locationHandlerImpl(const char* data, const size_t size, const size_t nmemb);
};

class CThreadPool
{
    public:
        CThreadPool();
        std::thread::id createThread(CHTTPClient* obj, const std::string& uri, const int requestId,
                const std::map<std::string, std::string> getData,
                const std::map<std::string, std::string> postData, const CProxy* proxy);

        ~CThreadPool();
    private:
        std::vector<std::thread> threads;
};

#endif // CHTTPCLIENT_H
