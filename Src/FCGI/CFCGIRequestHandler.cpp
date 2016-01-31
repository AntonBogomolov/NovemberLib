#include "../../Include/novemberlib/FCGI/CFCGIRequestHandler.h"

#include "../../Include/novemberlib/utils/utils.h"
#include "../../Include/novemberlib/utils/URI.h"
#include "../../Include/novemberlib/utils/MIMEType.h"

#include "../../Include/novemberlib/managers/CManagers.h"
#include "../../Include/novemberlib/managers/CResourceManager.h"
#include "../../Include/novemberlib/helpers/CConfigHelper.h"
#include "../../Include/novemberlib/utils/CLog.h"

CFCGIRequestHandler::CFCGIRequestHandler(int listenSocket) :
            in(std::cin.rdbuf()),
            out(std::cout.rdbuf()),
            err(std::cerr.rdbuf())
{
	sin  = NULL;
	sout = NULL;
	serr = NULL;
	options = 0;
	flushed = true;

	files = new CRequestFileSet();

	FCGX_InitRequest(&request, listenSocket, 0);
}

CFCGIRequestHandler::~CFCGIRequestHandler()
{
	endRequest();
	delete files;
}

void CFCGIRequestHandler::initBufs()
{
    flushed = false;

    delete sin;
    sin = new fcgi_streambuf(request.in);
    in.rdbuf(sin);

	delete sout;
    sout = new fcgi_streambuf(request.out);
    out.rdbuf(sout);

    delete serr;
    serr = new fcgi_streambuf(request.err);
    err.rdbuf(serr);
}

bool CFCGIRequestHandler::acceptRequest()
{
    bool ok = FCGX_Accept_r(&request) == 0;
    if(ok)
    {
        initBufs();

        for( ; *request.envp; ++request.envp)
        {
            std::string couple(*request.envp);
            std::string name;
            std::string value;
            bool mode = true;

            for(size_t i = 0; i < couple.size(); i++)
            {
                char c = couple[i];
                if(mode)
                {
                    if(c == '=') mode = !mode;
                    else name += c;
                }
                else value += c;

                if(i == couple.size()-1) param.set(name, value);
            }
        }

        //build location and get vars
        _uri = param.get("REQUEST_URI","");
        size_t sp1 = _uri.find("?");
        _location = _uri.substr(0, sp1);

        if(sp1 != std::string::npos)
        {
            std::string gets = _uri.substr(sp1+1);
            parseURICouples(gets, get);
        }

        //search locale
        std::string lname;
        std::string lvalue;
        ParamSet lopts;
        parseMIMEField("field: "+param.get("HTTP_ACCEPT_LANGUAGE",";"), lname, lvalue, lopts);
        //parse languages
        std::vector<std::string> langs;
        std::string tmp="";
        size_t min = 1000;
        int imin = -1;
        for(size_t i = 0; i < lvalue.size(); i++)
        {
            const char& c = lvalue[i];
            bool end = (i == lvalue.size()-1);

            if(c == ',' || end)
            {
                if(end && c != ' ') tmp += c;

                if(tmp.size() != 0)
                {
                    if(tmp.size() < min)
                    {
                        min = tmp.size();
                        imin = langs.size();
                    }

                    langs.push_back(tmp);
                    tmp = "";
                }
            }
            else if(c != ' ') tmp += c;
        }
        if(imin != -1) _locale = langs[imin];

        //post vars
        if(param.get("CONTENT_TYPE","").find("application/x-www-form-urlencoded") != std::string::npos)
        {
            const long length = param.get("CONTENT_LENGTH", 0);
            char* data = new char[length];

            in.read(data, length);
            parseURICouples(std::string(data, length), post);
			delete [] data;
        }
        else if(param.get("CONTENT_TYPE","").find("multipart/form-data") != std::string::npos) parseMIMEData();

        //cookies
        std::string cname;
        std::string cvalue;
        parseMIMEField("Cookie: value; "+param.get("HTTP_COOKIE", ""), cname, cvalue, cookie);
    }

    return ok;
}

void CFCGIRequestHandler::endRequest()
{
    if(!flushed)
    {
        std::map<std::string, std::string>::iterator it = header.map().begin();
        while(it != header.map().end())
        {
            out << it->first << ": " << it->second << "\r\n";
            it++;
        }

        out << header_stream.str();
        out << "\r\n";
        out << response.str();

        do in.ignore(1024);
        while (in.gcount() == 1024);

		delete sin;
		delete sout;
		delete serr;
        sin = NULL;
        sout = NULL;
        serr = NULL;

        files->clean();

        in.rdbuf(std::cin.rdbuf());
        out.rdbuf(std::cout.rdbuf());
        err.rdbuf(std::cerr.rdbuf());

        get.clear();
        post.clear();
        header.clear();
        cookie.clear();
        param.clear();

        response.clear();
        response.str("");

        header_stream.clear();
        header_stream.str("");

        flushed = true;
    }
}

void CFCGIRequestHandler::parseURICouples(const std::string& uri, ParamSet& pset)
{
    std::string tmp;
    std::string name;
    bool mode = true;

    for(size_t i = 0; i < uri.size(); i++)
    {
        char c = uri[i];
        bool end = (i == uri.size()-1);

        //var name
        if(mode)
        {
            if(c == '=')
            {
                if(end) pset.set(URI::decode(tmp), "");

                name = tmp;
                mode = !mode;
                tmp = "";
            }
            else if(c == '&')
            {
                tmp = "";
                pset.set(URI::decode(tmp), "");
            }
            else if(end)
            {
                tmp += c;
                pset.set(URI::decode(tmp), "");
            }
            else tmp += c;
        }
        else
        {
            if(c == '&' || end)
            {
                if(end) tmp += c;

                pset.set(URI::decode(name), URI::decode(tmp));
                tmp = "";
                name = "";
                mode = !mode;
            }
            else tmp += c;
        }
    }
}

void CFCGIRequestHandler::parseMIMEData()
{
    const int NONE = 0;
    const int DATA = 1;
    const int PARAM = 2;

    const int TEXT = 0;
    const int FILE = 1;

    int mode = NONE;
    int type = TEXT;

	std::string fileData;
	std::string sep;
    std::string tmp;
    std::string boundary;
    std::string eboundary;
    long boundary_pos = 0;
    std::vector<std::string> params;
    ParamSet pset;

    long done = 0;
    long length = param.get("CONTENT_LENGTH", 0);
    while(done < length)
    {
        char c = in.get();
        //first line : boundary
        if(mode == NONE)
        {
            //search for line
            if(c == '\n' || c == '\r')
            {
                sep += c;
                if(sep == "\r\n")
                {
                    mode = PARAM;
                    sep = "";
                    //real boundary
                    eboundary = "\r\n"+boundary;
                }
            }
            else boundary += c;
        }
        else if(mode == PARAM)
        {
            //search for line
            if(c == '\n' || c == '\r')
            {
                sep += c;
                if(sep == "\r\n")
                {
                    if(tmp.size() > 0)
                    {
                        params.push_back(tmp);
                        tmp = "";
                    }
                    else
                    {
                        //parse mime params
                        for(size_t i = 0; i < params.size(); i++)
                        {
                            std::string name;
                            std::string value;
                            ParamSet opts;
                            parseMIMEField(params[i], name, value, opts);

                            //analyse field
                            if(name == "Content-Disposition")
                            {
                                std::string pname = opts.get("name","");
                                pset.set("name", pname);

                                if(opts.has("filename"))
                                {
                                    //prepare file reading
                                    std::string filename = opts.get("filename","");;
                                    pset.set("filename", filename);
                                    type = FILE;
                                    fileData = "";
                                    //pfile = &(file.alloc(pname));
                                }

                            }
                            else if(name == "Content-Type") pset.set("type", value);
                        }

                        mode = DATA;
                        boundary_pos = 0;
                        params.clear();
                    }
                    sep = "";
                }
            }
            else tmp += c;
        }
        else if(mode == DATA)
        {
            if(type == FILE) fileData += c; //pfile->data += c;
            else tmp += c;

            //waiting \r\n and boundary
            //test if the boundary will be match
            if(c == eboundary[boundary_pos])
            {
                //boundary possible
                if((size_t)boundary_pos == eboundary.size() - 1)
                {
                    //boundary : save data
                    mode = PARAM;
                    if(type == FILE)
                    {
                        //delete boundary from data
                        if(fileData.size() > eboundary.size())fileData.resize(fileData.size() - eboundary.size());
                        else fileData.clear();

                        std::string fileIdString = pset.get("name", "");
                        std::string fileMIME 	 = pset.get("type", "");

                        CConfigHelper* gs = CConfigHelper::getInstance();
						if( fileData.size() > 0 && fileData.size() < (size_t)gs->getLongParamValue("maxFileSize", 10485760))
						{
							CFileDescriptor* file = new CFileDescriptor(fileIdString, fileData.c_str(), fileData.size() * sizeof(char), fileMIME);
							files->addFile(file);
						}
						fileData.clear();
                    }
                    else if(type == TEXT)
                    {
                        //delete boundary from data
                        if(tmp.size() > eboundary.size())
                            tmp.resize(tmp.size()-eboundary.size());
                        else
                            tmp.clear();
                        post.set(pset.get("name",""), tmp);
                    }

                    tmp = "";
                    type = TEXT;
                    pset.clear();

                    //skip the two next characters
                    in.get();
                    in.get();
                }

                boundary_pos++;
            }
            else //no match, reset
                boundary_pos = 0;
        }
        done++;
    }
}

void CFCGIRequestHandler::parseMIMEField(const std::string& field, std::string& name, std::string& value, ParamSet& options)
{
    const int NONE = 0;
    const int NAME = 1;
    const int VALUE = 2;

    //split to ":"
    size_t sp1 = field.find(":");
    if(sp1 != std::string::npos)
    {
        name = deleteChars(field.substr(0, sp1), " \n\r");

        //get value
        size_t sp2 = field.find(";");
        if(sp2 != std::string::npos)
        {
            value = deleteChars(field.substr(sp1+1, sp2-sp1-1)," \n\r");

            //parse options
            int mode = NONE;
            std::string oname, ovalue;
            for(size_t i = sp2; i < field.size(); i++)
            {
                char c = field[i];
                bool end = (i == field.size()-1);

                //waiting
                if(mode == NONE)
                {
                    if(c != ';' && c != ' ')
                    {
                        oname += c;
                        mode = NAME;
                    }
                }
                //parse name
                else if(mode == NAME)
                {
                    if(c == '=') mode = VALUE;
                    else if(c != ' ' && c != '=') oname += c;
                }
                //parse value
                else if(mode == VALUE)
                {
                    if(c == ';' || end)
                    {
                        mode = NONE;
                        if(end) ovalue += c;

                        options.set(URI::decode(oname), URI::decode(deleteChars(ovalue,"\"")));
                        oname = "";
                        ovalue = "";
                    }
                    else ovalue += c;
                }
            }
        }
        else value = field.substr(sp1+2);
    }
}

void CFCGIRequestHandler::createCookie(const std::string& name, const std::string& data, long time)
{
    if(name.size() > 0 && time >= 0 && name.size() < 100 && data.size() < 100)
    {
        header_stream << "Set-Cookie: " << URI::encode(name) << "=" << URI::encode(data) << ";"
                      << " Path=/;"
                      << " Max-Age=" << time << "\r\n";
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

CRequestFileSet::CRequestFileSet()
{

}

CRequestFileSet::~CRequestFileSet()
{
	clean();
}

const CFileDescriptor* CRequestFileSet::getFile(const std::string& fileIdString) const
{
	for(auto it = files.begin(); it != files.end(); ++it)
	{
		CFileDescriptor* currFile = (*it);
		if(currFile->getFullFileName() == fileIdString) return currFile;
	}
	return NULL;
}

void CRequestFileSet::addFile(CFileDescriptor* file)
{
	if(!file->getIsValid()) return;
	if(getFile(file->getFullFileName()) != NULL) return;

	files.push_back(file);
}

void CRequestFileSet::clean()
{
	for(auto it = files.begin(); it != files.end(); ++it)
	{
        CFileDescriptor* currFile = (*it);
        delete currFile;
	}
	files.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParamSet::has(const std::string& param) const
{
    return params.find(param) != params.end();
}

void ParamSet::clear()
{
    params.clear();
}

std::string ParamSet::get(const std::string& param, const std::string& def) const
{
    std::map<std::string,std::string>::const_iterator it = params.find(param);
    if(it != params.end())
        return it->second;
    else
        return def;
}

void ParamSet::set(const std::string& param, const std::string& val)
{
    std::map<std::string, std::string>::iterator it = params.find(param);

    if(it != params.end())
        params.erase(it);

    params.insert(std::pair<std::string, std::string>(param,val));
}
