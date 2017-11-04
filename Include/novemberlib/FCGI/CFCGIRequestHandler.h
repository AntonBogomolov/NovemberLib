#ifndef CFCGIREQUESTHANDLER_H
#define CFCGIREQUESTHANDLER_H

#include "../interfaces/IUncopyable.h"

#include <string>
#include <vector>
#include <map>

#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <fcgio.h>

class CFileDescriptor;

class CRequestFileSet : public IUncopyable
{
	public:
		 const CFileDescriptor* getFile(const std::string& fileIdString) const;
		 void addFile(CFileDescriptor* file);

		 void clean();

		 CRequestFileSet();
		 virtual ~CRequestFileSet();
	private:
		std::vector<CFileDescriptor*> files;
};

class ParamSet
{
public:
    bool has(const std::string& param) const;
    std::string get(const std::string& param, const std::string& def) const;
    void set(const std::string& param, const std::string& val);

    std::map<std::string,std::string>& map()
    {
        return params;
    }

    void clear();

    template<typename T> T get(const std::string& param, const T& def) const
    {
        std::map<std::string,std::string>::const_iterator it = params.find(param);
        if(it != params.end())
        {
            T val(0);
            std::stringstream ss(it->second);
            ss >> val;

            return val;
        }
        else return def;
    }

    template<typename T> void set(const std::string& param, const T& val)
    {
        std::map<std::string, std::string>::iterator it = params.find(param);

        std::stringstream ss;
        ss << val;

        if(it != params.end()) params.erase(it);
        params.insert(std::pair<std::string, std::string>(param, ss.str()));
    }

private:
    std::map<std::string, std::string> params;
};

class CFCGIRequestHandler : public IUncopyable
{
	public:
		bool acceptRequest();
		void endRequest();

		const std::string& uri() const{ return _uri; }
		void setURI(const std::string& uri) { _uri = uri;  }
		const std::string& location() const{ return _location; }

		const std::string& locale()const{ return _locale; }
		void locale(const std::string& loc){ _locale = loc; }

		std::stringstream response;
		ParamSet post;
		ParamSet get;
		ParamSet header;
		ParamSet param;
		ParamSet cookie;
		CRequestFileSet* files;

		void createCookie(const std::string& name, const std::string& data, long time);
		static void parseURICouples(const std::string& uri, ParamSet& pset);
		//parse field like [name: value; opt1=val1; opt2="val2"]
		static void parseMIMEField(const std::string& field, std::string& name, std::string& value, ParamSet& options);

		CFCGIRequestHandler(int listenSocket = 0);
		virtual ~CFCGIRequestHandler();
	protected:
	private:
		void initBufs();
		void parseMIMEData();
		std::stringstream header_stream;
		FCGX_Request request;
		int _listenSocket;

		fcgi_streambuf *sin;
		fcgi_streambuf *sout;
		fcgi_streambuf *serr;
		std::istream in;
		std::ostream out;
		std::ostream err;

		std::string _uri;
		std::string _location;
		std::string _locale;

		unsigned int options;
		bool flushed;
};

#endif // CFCGIREQUESTHANDLER_H
