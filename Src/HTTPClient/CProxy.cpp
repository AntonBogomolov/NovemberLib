#include "../../Include/novemberlib/HTTPClient/CProxy.h"

#include "../../Include/novemberlib/HTTPClient/CHTTPClient.h"
#include "../../Include/novemberlib/utils/CLog.h"

#include "time.h"
#include <fstream>

using namespace std;

bool CProxy::testProxy()
{
    CHTTPClient* httpClient = CHTTPClient::getInstance();
    std::string connectionString = getURI();

    isWorking = true;
    bool isProxyWasEnabled = httpClient->getIsProxyEnabled();
    httpClient->enableProxy();

    time_t startTime;
    time_t endTime;
    time_t duration;
    time(&startTime);

    CLog::getInstance()->addInfo("Proxy test: " + valueToString(getIP()));
    int reqId = httpClient->createRequest("www.google.ru", false, map<string, string>(), map<string, string>(), this);
    //httpClient->restoreProxyState();

    const CHTTPClientRequestResult* result = dynamic_cast<const CHTTPClientRequestResult*>(httpClient->waitAndGetResult(valueToString(reqId)));
    if(!result || result->getContent() == "BAD")
    {
        isWorking = false;
    }

    if(!isProxyWasEnabled) httpClient->disableProxy();
    time(&endTime);
    duration = endTime - startTime;

    CLog::getInstance()->addInfo("PING");
    CLog::getInstance()->addInfo(duration);
    if(duration > 10)
    {
        isWorking = false;
    }
    return isWorking;
}

////////////////////////////////////////////////////////////////////////////////////////////

CProxyList::CProxyList(const int maxProxyCnt)
{
    this->maxProxyCnt = maxProxyCnt;
}
CProxyList::~CProxyList()
{
    for(auto it = proxyList.begin(); it != proxyList.end(); ++it)
    {
        CProxy* currProxy = (*it);
        delete currProxy;
    }
}

void CProxyList::getProxyListFromWeb(const std::string& countryCode, const int protocol)
{
    CHTTPClient* httpClient = CHTTPClient::getInstance();
    int reqId = httpClient->createRequest("http://free-proxy-list.net/anonymous-proxy.html", true);

    const CHTTPClientRequestResult* result = dynamic_cast<const CHTTPClientRequestResult*>(httpClient->waitAndGetResult(valueToString(reqId)));
    if(result->getContent() == "BAD")
    {
        CLog::getInstance()->addError("Cant load proxy list from free-proxy-list.net");
        return;
    }
    std::string resultContent = result->getContent();
    size_t tableStart = resultContent.find("<tbody>");
    if(tableStart == std::string::npos) return;
    tableStart += 7;
    size_t tableEnd = resultContent.find("</tbody>",tableStart);
    if(tableEnd == std::string::npos) return;
    size_t tableLen = tableEnd - tableStart;
    if(tableLen <= 0) return;

    std::string tableContent = resultContent.substr(tableStart, tableLen);
    size_t pos = 0;
    while(tableContent.find("<tr>", pos) != std::string::npos )
    {
        pos+=4;
        size_t tableStringEnd = tableContent.find("</tr>", pos);
        if(tableStringEnd == std::string::npos) continue;

        std::string tableString = tableContent.substr(pos, tableStringEnd - pos);
        pos = tableStringEnd;
        pos += 4;
        CProxy* proxy = parseProxyStringFromWeb(tableString);
        if(!proxy) continue;
        bool isWorking = true;
        if((countryCode.size() > 1 && countryCode != proxy->getCountryCode()) ||
            ((protocol == CProxy::PROTOCOL_HTTP || protocol == CProxy::PROTOCOL_HTTPS) && (protocol != proxy->getProtocol())) )
        {
            isWorking = false;
        }
        if( isWorking)isWorking = proxy->testProxy();
        if(!isWorking)
        {
            delete proxy;
            continue;
        }
        proxyList.push_back(proxy);
        if(getProxyCount() >= maxProxyCnt) break;
    }
    CLog::getInstance()->addInfo("success load proxy list from free-proxy-list.net");
}
CProxy* CProxyList::parseProxyStringFromWeb(const std::string& tableString) const
{
    // IP
    size_t tdStart = tableString.find("<td>");
    size_t tdEnd = 0;
    std::string ip;
    if(tdStart != std::string::npos)
    {
        tdStart += 4;
        tdEnd = tableString.find("</td>", tdStart);
        size_t tdLen = tdEnd - tdStart;
        if(tdLen <= 0) return nullptr;
        ip = tableString.substr(tdStart, tdLen);
    }
    // PORT
    tdStart = tableString.find("<td>", tdEnd);
    std::string port;
    if(tdStart != std::string::npos)
    {
        tdStart += 4;
        tdEnd = tableString.find("</td>", tdStart);
        size_t tdLen = tdEnd - tdStart;
        if(tdLen <= 0) return nullptr;
        port = tableString.substr(tdStart, tdLen);
    }
    // COUNTRY CODE
    tdStart = tableString.find("<td>", tdEnd);
    std::string countryCode;
    if(tdStart != std::string::npos)
    {
        tdStart += 4;
        tdEnd = tableString.find("</td>", tdStart);
        size_t tdLen = tdEnd - tdStart;
        if(tdLen <= 0) return nullptr;
        countryCode = tableString.substr(tdStart, tdLen);
    }
    // COUNTRY
    tdStart = tableString.find("<td>", tdEnd);
    std::string country;
    if(tdStart != std::string::npos)
    {
        tdStart += 4;
        tdEnd = tableString.find("</td>", tdStart);
        size_t tdLen = tdEnd - tdStart;
        if(tdLen <= 0) return nullptr;
        country = tableString.substr(tdStart, tdLen);
    }
    // ANON
    tdStart = tableString.find("<td>", tdEnd);
    std::string anon;
    if(tdStart != std::string::npos)
    {
        tdStart += 4;
        tdEnd = tableString.find("</td>", tdStart);
        size_t tdLen = tdEnd - tdStart;
        if(tdLen <= 0) return nullptr;
        anon = tableString.substr(tdStart, tdLen);
    }
    // GOOGLE
    tdStart = tableString.find("<td>", tdEnd);
    std::string google;
    if(tdStart != std::string::npos)
    {
        tdStart += 4;
        tdEnd = tableString.find("</td>", tdStart);
        size_t tdLen = tdEnd - tdStart;
        if(tdLen <= 0) return nullptr;
        google = tableString.substr(tdStart, tdLen);
    }
    // HTTPS
    tdStart = tableString.find("<td>", tdEnd);
    std::string https;
    if(tdStart != std::string::npos)
    {
        tdStart += 4;
        tdEnd = tableString.find("</td>", tdStart);
        size_t tdLen = tdEnd - tdStart;
        if(tdLen <= 0) return nullptr;
        https = tableString.substr(tdStart, tdLen);
    }

    int protocol = https == "yes" ? CProxy::PROTOCOL_HTTPS : CProxy::PROTOCOL_HTTP;
    int proxyPort = 0;
    try
    {
        proxyPort = stoi(port);
    }
    catch(...)
    {
        proxyPort = 0;
    }
    if(ip.size() < 7 || ip.size() > 16 || proxyPort <= 0) return nullptr;
    return new CProxy(ip, proxyPort, protocol, countryCode);
}

void CProxyList::saveListToFile()
{
    std::string fileData = "";
    for(auto it = proxyList.begin(); it != proxyList.end(); ++it)
    {
        CProxy* currProxy = (*it);
        fileData += currProxy->getURI();
        fileData += "[";
        fileData += currProxy->getCountryCode();
        fileData += "]";
        fileData += '\n';
    }

    std::ofstream file;
	try
	{
        file.open("proxy.conf");
        file << fileData;
	}
	catch(...)
	{
		CLog::getInstance()->addError("Cant save proxy.conf");
	}
	file.close();
}

void CProxyList::loadFromFile()
{
    std::ifstream file;
    std::string tmpStr = "";
	try
	{
		file.open("proxy.conf");
		if(!file.is_open())
		{
			CLog::getInstance()->addError("Cant open proxy.conf");
			return;
		}

		while(!file.eof())
		{
            if(getProxyCount() >= maxProxyCnt) break;

			std::getline(file, tmpStr);
			CProxy* proxy = parseProxyStringFromFile(tmpStr);
            if(!proxy) continue;
            bool isWorking = proxy->testProxy();
            if(!isWorking) delete proxy;
            proxyList.push_back(proxy);
		}
	}
	catch(...)
	{
		CLog::getInstance()->addError("Cant read proxy.conf");
	}
	file.close();
}

CProxy* CProxyList::parseProxyStringFromFile(const std::string& fileString) const
{
    size_t start = 0;
    size_t end = 0;
    size_t len = 0;

    end = fileString.find("://", start);
    if(end == std::string::npos) return nullptr;
    end += 3;
    len = end - start;
    if(len <=0 ) return nullptr;
    std::string protocol = fileString.substr(start, len);
    start = end;

    end = fileString.find(":", start);
    if(end == std::string::npos) return nullptr;
    len = end - start;
    if(len <=0 ) return nullptr;
    std::string ip = fileString.substr(start, len);
    start = end + 1;

    end = fileString.find("[", start);
    if(end == std::string::npos) return nullptr;
    len = end - start;
    if(len <=0 ) return nullptr;
    std::string port = fileString.substr(start, len);
    start = end + 1;

    end = fileString.find("]", start);
    if(end == std::string::npos) return nullptr;
    len = end - start;
    if(len <=0 ) return nullptr;
    std::string countryCode = fileString.substr(start, len);
    start = end + 1;

    int proxyProtocol = protocol == "https://" ? CProxy::PROTOCOL_HTTPS : CProxy::PROTOCOL_HTTP;
    int proxyPort = 0;
    try
    {
        proxyPort = stoi(port);
    }
    catch(...)
    {
        proxyPort = 0;
    }
    if(ip.size() < 7 || ip.size() > 16 || proxyPort <= 0) return nullptr;
    return new CProxy(ip, proxyPort, proxyProtocol, countryCode);
}

const CProxy* CProxyList::getRandomProxy() const
{
    size_t proxyCnt = getProxyCount();
    if(proxyList.empty()) return nullptr;
    int index = std::rand()%proxyCnt;

    return proxyList[index];
}
std::vector<CProxy*>& CProxyList::getProxyList()
{
    return proxyList;
}
std::vector<const CProxy*> CProxyList::getProxyListByProtocol(const int protocol) const
{
    std::vector<const CProxy*> result;
    for(auto it = proxyList.begin(); it != proxyList.end(); ++it)
    {
        const CProxy* currProxy;
        if(currProxy->getIsWorking() && currProxy->getProtocol() == protocol) result.push_back(currProxy);
    }
    return result;
}
std::vector<const CProxy*> CProxyList::getProxyListByCountryCode(const std::string& countryCode) const
{
    std::vector<const CProxy*> result;
    for(auto it = proxyList.begin(); it != proxyList.end(); ++it)
    {
        const CProxy* currProxy;
        if(currProxy->getIsWorking() && currProxy->getCountryCode() == countryCode) result.push_back(currProxy);
    }
    return result;
}
int CProxyList::getProxyCount() const
{
    return proxyList.size();
}
