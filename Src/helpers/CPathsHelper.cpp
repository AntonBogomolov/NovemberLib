#include "../../Include/novemberlib/helpers/CPathsHelper.h"

CPathsHelper::CPathsHelper()
<<<<<<< HEAD
    : userContentDirName("usercontent/"), siteContentDirName("sitecontent/")
{
=======
{
	userContentDirName = "usercontent/";
	siteContentDirName = "sitecontent/";

>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
	rootPath = new CPath("");
}

CPathsHelper::~CPathsHelper()
{
	for(auto it = paths.begin(); it != paths.end(); ++it)
	{
        CPath* currPath = (*it);
        delete currPath;
	}
	delete rootPath;
}

const std::string& CPathsHelper::getUserContentDir() const
{
	return userContentDirName;
}

const std::string& CPathsHelper::getSiteContentDir() const
{
	return siteContentDirName;
}

void CPathsHelper::addPath(const std::string& dir, const std::string& allowedMIMEs, const bool isOnlyForSiteContent)
{
	paths.push_back(new CPath(dir, allowedMIMEs, isOnlyForSiteContent));
}

const CPath* CPathsHelper::getPath(const std::string& dir) const
{
	for(auto it = paths.begin(); it != paths.end(); ++it)
	{
        CPath* currPath = (*it);
        if(currPath->getDirName() == dir) return currPath;
	}
	return NULL;
}

const CPath* CPathsHelper::getRootPath() const
{
	return rootPath;
}

const std::vector<const CPath*> CPathsHelper::getPathsFitsForResourceMIME(const std::string& resourceMIME, const bool isOnlyForSiteContent) const
{
	std::vector<const CPath*> resultPaths;
	for(auto it = paths.begin(); it != paths.end(); ++it)
	{
        CPath* currPath = (*it);
        if(currPath->getIsMIMEFit(resourceMIME, isOnlyForSiteContent)) resultPaths.push_back(currPath);
	}
	return resultPaths;
}

const CPath* CPathsHelper::getPathFitsForResourceMIME(const std::string& resourceMIME, const bool isOnlyForSiteContent) const
{
	for(auto it = paths.begin(); it != paths.end(); ++it)
	{
        CPath* currPath = (*it);
        if(currPath->getIsMIMEFit(resourceMIME, isOnlyForSiteContent)) return currPath;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////

const std::string CPath::getRelativeDir() const
{
	std::string result = dir;
	if ((int)result.length() > 0) result += "/";
	return result;
}

const std::string CPath::getDirName() const
{
	return dir;
}

const std::string CPath::getAllowedMIMEs() const
{
	return allowedMIMEs;
}

bool CPath::getIsOnlyForSiteContent() const
{
	return isOnlyForSiteContent;
}

<<<<<<< HEAD
CPath::CPath(const std::string& dir, const std::string& allowedMIMEs, const bool isOnlyForSiteContent) :
    dir(dir), allowedMIMEs(allowedMIMEs), isOnlyForSiteContent(isOnlyForSiteContent)
{

=======
CPath::CPath(const std::string& dir, const std::string& allowedMIMEs, const bool isOnlyForSiteContent)
{
	this->dir = dir;
	this->allowedMIMEs = allowedMIMEs;
	this->isOnlyForSiteContent = isOnlyForSiteContent;
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
}

CPath::~CPath()
{

}

bool CPath::getIsMIMEFit(const std::string& resourceMIME, const bool isOnlyForSiteContent) const
{
	if(getIsOnlyForSiteContent() && !isOnlyForSiteContent) return false;
	if(allowedMIMEs.find("*/*") != std::string::npos) return true;

	size_t delimPos = resourceMIME.find_first_of('/');
	if(delimPos == std::string::npos) return false;

	std::string type   = resourceMIME.substr(0, delimPos);
	//std::string format = resourceMIME.substr(delimPos + 1, resourceMIME.length() - delimPos);

	if(allowedMIMEs.find(resourceMIME) != std::string::npos) return true;
	if(allowedMIMEs.find(type + "/*")  != std::string::npos) return true;

	return false;
}
