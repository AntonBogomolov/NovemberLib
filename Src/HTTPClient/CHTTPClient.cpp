#include "../../Include/novemberlib/HTTPClient/CHTTPClient.h"

#include "../../Include/novemberlib/HTTPClient/CProxy.h"
#include "../../Include/novemberlib/utils/utils.h"
#include "../../Include/novemberlib/utils/md5.h"

#include "time.h"

CHTTPClient::CHTTPClient()
{
    std::srand(unsigned(std::time(0)));
    curl_global_init(CURL_GLOBAL_ALL);

    activeProxyState.isProxyEnabled = false;
    activeProxyState.proxyList = nullptr;
    activeProxyState.singleProxy = nullptr;

    threadPool = new CThreadPool();
}

CHTTPClient::~CHTTPClient()
{
    curl_global_cleanup();
    delete threadPool;

    delete activeProxyState.proxyList;
    activeProxyState.proxyList = nullptr;
}

int CHTTPClient::createRequest(const std::string& uri, bool isSyncRequest,
                const std::map<std::string, std::string> getData, const std::map<std::string, std::string> postData,
                const CProxy* proxy)
{
    int requestId = std::rand() % 2147483600;

    const CProxy* currProxy = nullptr;
    if(getIsProxyEnabled())
    {
        if(proxy && proxy->getIsWorking()) currProxy = proxy;
    }

    if(!isSyncRequest)
    {
        threadPool->createThread(this, uri, requestId, getData, postData, currProxy);
    }
    else
    {
        threadHandler(uri, requestId, getData, postData, currProxy);
    }

    return requestId;
}

void CHTTPClient::threadHandler(const std::string& uri, const int requestId, const std::map<std::string, std::string> getData,
                                const std::map<std::string, std::string> postData, const CProxy* proxy)
{
	CURL *curl;
    curl = curl_easy_init();
    if(curl == NULL)
    {
        CLog::getInstance()->addError("Can't create curl connect");
        return;
    }
    const CProxy* currProxy = proxy;
    std::string cookieFileName = "cookies.txt";

    CHTTPClientRequest request;
    std::string uriAndGet = uri;
    if(!getData.empty())
    {
        uriAndGet += "?";
        for(auto it = getData.begin(); it != getData.end(); ++it)
        {
            uriAndGet += it->first;
            uriAndGet += "=";
            uriAndGet += urlEncode(it->second);
            if(it != (--getData.end())) uriAndGet += "&";
        }
    }

    CLog::getInstance()->addInfo("CURL URI: "+uriAndGet);
    char *uriStr = new char[uriAndGet.length()+1];
    strcpy(uriStr, uriAndGet.c_str());
    uriStr[uriAndGet.length()] = '\0';

    curl_easy_setopt(curl, CURLOPT_URL, uriStr);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:51.0) Gecko/20100101 Firefox/51.0");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    //curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&request);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CHTTPClientRequest::contentHandler);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&request);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, &CHTTPClientRequest::locationHandler);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 60L);

    if(currProxy && currProxy->getIsWorking())
    {
        cookieFileName = md5(currProxy->getIP()) + "-" + cookieFileName;
        CLog::getInstance()->addInfo(cookieFileName);
        std::string proxyURI = currProxy->getURI();
        CLog::getInstance()->addInfo("CURL PROXY: " + proxyURI);
        curl_easy_setopt(curl, CURLOPT_PROXY, proxyURI.c_str());
    }
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR,  cookieFileName.c_str());
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookieFileName.c_str());

    //struct curl_slist *headers = NULL;
    char *postfields = NULL;
    if(!postData.empty())
    {
        std::string postDataDecode = "";//urlEncode(postData);
        std::map<std::string, std::string> pData;

        for(auto it = postData.begin(); it != postData.end(); ++it)
        {
            postDataDecode += it->first;
            postDataDecode += "=";
            postDataDecode += urlEncode(it->second);
            if(it != (--postData.end())) postDataDecode += "&";
        }
        char *postfields = new char[postDataDecode.length()+1];
        strcpy(postfields, postDataDecode.c_str());
        postfields[postDataDecode.length()] = '\0';

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(postfields));
        curl_easy_setopt(curl, CURLOPT_POST, 1);
    }
    //curl_slist_append(headers, "charsets: utf-8");
    //curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

    CURLcode code;
    code = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    delete [] postfields;
    delete [] uriStr;

    if(code != CURLE_OK && currProxy)
    {
        if(currProxy) currProxy->setIsWorking(false);
        CLog::getInstance()->addError("curl proxy error, try to turn off proxy");
        threadHandler(uri, requestId, getData, postData, nullptr);
    }
    else
    if(code != CURLE_OK && !getIsProxyEnabled())
    {
        CLog::getInstance()->addError("curl curl_easy_perform error");
        addResult(new CHTTPClientRequestResult("BAD", "BAD", valueToString(requestId)));
        return;
    }
    else
    {
        addResult(new CHTTPClientRequestResult(request.getContent(), request.getLocation(), valueToString(requestId)));
    }
}

void CHTTPClient::enableProxy()
{
    activeProxyState.isProxyEnabled = true;
}
bool CHTTPClient::getIsProxyEnabled()
{
    return activeProxyState.isProxyEnabled;
}
void CHTTPClient::disableProxy()
{
    activeProxyState.isProxyEnabled = false;
}

///////////////////////////////////////////////////////////////////////////////

CHTTPClientRequest::CHTTPClientRequest()
{
    content = "";
}

CHTTPClientRequest::~CHTTPClientRequest()
{

}

std::string CHTTPClientRequest::getContent() const
{
    return content;
}

std::string CHTTPClientRequest::getLocation() const
{
    return location;
}

size_t CHTTPClientRequest::contentHandler(const char* data, const size_t size, const size_t nmemb, void* p)
{
    return static_cast<CHTTPClientRequest*>(p)->contentHandlerImpl(data, size, nmemb);
}

size_t CHTTPClientRequest::contentHandlerImpl(const char* data, const size_t size, const size_t nmemb)
{
    content.append(data, size * nmemb);
    return size * nmemb;
}

size_t CHTTPClientRequest::locationHandler(const char* data, const size_t size, const size_t nmemb, void* p)
{
    return static_cast<CHTTPClientRequest*>(p)->locationHandlerImpl(data, size, nmemb);
}

size_t CHTTPClientRequest::locationHandlerImpl(const char* data, const size_t size, const size_t nmemb)
{
    location.append(data, size * nmemb);
    return size * nmemb;
}

///////////////////////////////////////////////////////////////////////////////////

CThreadPool::CThreadPool()
{

}

CThreadPool::~CThreadPool()
{
    for(auto it = threads.begin(); it != threads.end(); ++it)
    {
        //delete (*it);
    }
}

std::thread::id CThreadPool::createThread(CHTTPClient* obj, const std::string& uri, const int requestId,
                const std::map<std::string, std::string> getData, const std::map<std::string, std::string> postData, const CProxy* proxy)
{
    for(unsigned int i = 0; i < threads.size(); i++)
    {
        if(threads[i].joinable() == 0)
        {
            threads[i] =  std::thread(&CHTTPClient::threadHandler, obj, uri, requestId, getData, postData, proxy);
            return threads[i].get_id();
        }
    }
    threads.push_back(std::thread(&CHTTPClient::threadHandler, obj, uri, requestId, getData, postData, proxy));
    return threads.back().get_id();
}

/////////////////////////////////////////////////////////////////////////////////////////


CHTTPClientRequestResult::CHTTPClientRequestResult(const std::string& content, const std::string& location,
                                                   const std::string& taskHash) :CResult(0, taskHash)
{
    data = new json();
    (*data)["content"]  = content;
    (*data)["location"] = location;
}

CHTTPClientRequestResult::~CHTTPClientRequestResult()
{

}

const std::string CHTTPClientRequestResult::getContent() const
{
    return (*data)["content"];
}
const std::string CHTTPClientRequestResult::getLocation() const
{
    return (*data)["location"];
}

///////////////////////////////////////////////////////////////////////////////////////////
