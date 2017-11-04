#include <vector>
#include <string>

#include "../utils/utils.h"

#ifndef CPROXY_H
#define CPROXY_H

class CProxy
{
    public:
        static const int PROTOCOL_HTTP  = 1;
        static const int PROTOCOL_HTTPS = 2;

        CProxy( const std::string& ip, int port = 80, const int protocol = PROTOCOL_HTTP,
                const std::string& countryCode = "")
        {
            this->ip = ip;
            this->port = port;
            this->protocol = protocol;
            this->countryCode = countryCode;
            if(this->protocol < PROTOCOL_HTTP || this->protocol > PROTOCOL_HTTPS) this->protocol = PROTOCOL_HTTP;
            isWorking = false;
        }
        ~CProxy()
        {

        }
        const std::string getURI() const
        {
            std::string protStr = "http://";
            if(protocol == PROTOCOL_HTTPS) protStr = "https://";
            return protStr + ip + ":" + valueToString(port);
        }
        const std::string getIP() const
        {
            return ip;
        }
        const std::string getCountryCode() const
        {
            return countryCode;
        }
        int getProtocol() const
        {
            return protocol;
        }
        bool getIsWorking() const
        {
            return isWorking;
        }
        void setIsWorking(bool isWorking) const
        {
            this->isWorking = isWorking;
        }
        bool testProxy();
    private:
        std::string ip;
        std::string countryCode;
        int port;
        int protocol;
        mutable bool isWorking;
};

class CProxyList
{
    public:
        CProxyList(const int maxProxyCnt = 10);
        ~CProxyList();

        void getProxyListFromWeb(const std::string& countryCode = "", const int protocol = 0);
        void saveListToFile();
        void loadFromFile();

        const CProxy* getRandomProxy() const;
        std::vector<CProxy*>& getProxyList();
        std::vector<const CProxy*> getProxyListByProtocol(const int protocol) const;
        std::vector<const CProxy*> getProxyListByCountryCode(const std::string& countryCode) const;
        int getProxyCount() const;
    private:
        std::vector<CProxy*> proxyList;
        int maxProxyCnt;
        CProxy* parseProxyStringFromWeb(const std::string& tableString) const;
        CProxy* parseProxyStringFromFile(const std::string& fileString) const;
};

#endif // CPROXY_H
