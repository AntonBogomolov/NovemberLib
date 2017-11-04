#include "../../Include/novemberlib/managers/CResourceManager.h"

#include "../../Include/novemberlib/FCGI/CFCGIRequest.h"
#include "../../Include/novemberlib/managers/CManagers.h"
#include "../../Include/novemberlib/helpers/CSyncHelper.h"
#include "../../Include/novemberlib/helpers/CPathsHelper.h"
#include "../../Include/novemberlib/helpers/CConfigHelper.h"
#include "../../Include/novemberlib/utils/MIMEType.h"
#include "../../Include/novemberlib/utils/CLog.h"
#include "../../Include/novemberlib/utils/utils.h"
#include "../../Include/novemberlib/utils/md5.h"

#include <string>
#include <fstream>
#include <sstream>

CResourceManager::CResourceManager()
{
	CConfigHelper* settingsManager = CConfigHelper::getInstance();
	long fileCacheSize = 40*1024*1024;
	if(settingsManager == NULL)
	{
		CLog::getInstance()->addWarning("SettingsManager not initialized yet, fileCacheSize set to def value");
	}
	else
	{
		fileCacheSize = settingsManager->getLongParamValue("maxFileCacheSize", 40*1024*1024);
	}

	fileCache = new CFileCache(fileCacheSize);
	isNeedProcessResourceRequest = true;
}

CResourceManager::~CResourceManager()
{
	delete fileCache;
}

bool CResourceManager::getIsNeedProcessResourceRequest() const
{
	return isNeedProcessResourceRequest;
}

void CResourceManager::setIsNeedProcessResourceRequest(const bool value)
{
	isNeedProcessResourceRequest = value;
}

CResourceResult CResourceManager::processResourceRequest(CFCGIRequest* currRequest)
{
	CResourceResult result;
	result.setMessage("Not valid file");

	if(!isNeedProcessResourceRequest)
	{
		result.setIsValid(false);
		return result;
	}

	std::string resCatalog;
	std::string resFileName;
	std::string resPath;
	std::string resUri = currRequest->getRequest()->uri();
	CPathsHelper* paths = CPathsHelper::getInstance();

	size_t fileNameStartPos = resUri.find_last_of('/');
	resFileName = resUri.substr(fileNameStartPos + 1, std::string::npos);
	if((int)resFileName.length() == 0 || resFileName.find_first_of('?') != std::string::npos)
	{
		result.setIsValid(false);
		return result;
	}
	resCatalog  = resUri.substr(1, fileNameStartPos - 1);
	size_t usercontendDirPos = resCatalog.rfind(paths->getUserContentDir());
	size_t sitecontendDirPos = resCatalog.rfind(paths->getSiteContentDir());

	bool isSiteContent = false;
	if( (resUri.find_first_of('/', 1) == std::string::npos) ||
		(usercontendDirPos == std::string::npos && sitecontendDirPos == std::string::npos))
	{
		isSiteContent = true;
	}
	else 
	{
		isSiteContent = sitecontendDirPos != std::string::npos && (usercontendDirPos == std::string::npos || sitecontendDirPos > usercontendDirPos);
		if(!isSiteContent) 	resCatalog = resCatalog.substr(usercontendDirPos + paths->getUserContentDir().length(), std::string::npos);
		else				resCatalog = resCatalog.substr(sitecontendDirPos + paths->getSiteContentDir().length(), std::string::npos);
	}	

	const CPath* path = paths->getPath(resCatalog);
	if(path != NULL && path->getIsOnlyForSiteContent() == isSiteContent)
	{
		if(isSiteContent) 	resPath = paths->getSiteContentDir() + path->getRelativeDir() + resFileName;
		else 				resPath = paths->getUserContentDir() + path->getRelativeDir() + resFileName;
		
		const CFileDescriptor* file = fileCache->findOrAddFile(resPath, isSiteContent);		
		if(!file->getIsValid())
		{
			result.setIsSuccess(false);			
			return result;
		}
		result.setFile(file);
		return result;
	}
	else
	{
		const std::vector<const CPath*> dirs = paths->getPathsFitsForResourceMIME(MIMEType::getInstance()->get(resFileName), isSiteContent);
		for(auto it = dirs.begin(); it != dirs.end(); ++it)
		{
			const CPath* currDir = (*it);
			if(isSiteContent) 	resPath = paths->getSiteContentDir() + currDir->getRelativeDir() + resFileName;
			else 				resPath = paths->getUserContentDir() + currDir->getRelativeDir() + resFileName;

			const CFileDescriptor* file = fileCache->findOrAddFile(resPath, isSiteContent);
			if(!file->getIsValid())
			{
				continue;
			}
			result.setFile(file);
			return result;
		}
	}

	result.setIsValid(false);
	return result;
}

const std::string CResourceManager::loadTextFile(const std::string& fullFileName)
{
	std::string fileText = "";

	std::ifstream file;
	std::ostringstream outStr;

	CSyncHelper::getInstance()->getFileMutex()->lock();
	file.open(fullFileName.c_str());
	if(!file.bad())
	{
		outStr << file.rdbuf();
		outStr.str().swap(fileText);
	}
	file.close();
	CSyncHelper::getInstance()->getFileMutex()->unlock();

	return fileText;
}

const std::string CResourceManager::saveFile(const std::string& fileName, const std::string& fileMimeType, const long fileSize, const char* fileData)
{
	std::ofstream file;
	CConfigHelper* settingsManager = CConfigHelper::getInstance();
	CPathsHelper* paths = CPathsHelper::getInstance();

	if(fileSize > settingsManager->getLongParamValue("maxFileSize", 10485760l)) return "";
	const CPath* currDir = paths->getPathFitsForResourceMIME(fileMimeType);
	if(currDir == NULL)
	{
		CLog::getInstance()->addError(fileMimeType + ":  no dir for this file type!");
		return "";
	}

	std::string fileCat  = currDir->getDirName();
	if((int)fileCat.length() > 0) fileCat += "/";

	std::string fileType = MIMEType::getInstance()->getFileExt(fileMimeType);
	if(fileCat == "none" || fileType == "none" )
	{
		CLog::getInstance()->addError(fileMimeType + " file type not allow!");
		return "";
	}

	std::string fullFileName = paths->getUserContentDir() + fileCat + fileName + "." + fileType;

	CSyncHelper::getInstance()->getFileMutex()->lock();
	if(!isFileExists(fullFileName))
	{
		file.open(fullFileName);
		if(file.is_open())
		{
			CLog::getInstance()->addInfo("File Path:" + fullFileName);
			CLog::getInstance()->addInfo("File Type:" + fileMimeType);
			CLog::getInstance()->addInfo("File size:" + valueToString(fileSize));

			try
			{
				file.write(fileData, fileSize);
			}
			catch(...)
			{
				CLog::getInstance()->addError("Cant write file!");
				fullFileName = "";
			}
		}
		else
		{
			CLog::getInstance()->addError("Cant create file!");
			fullFileName = "";
		}
		file.close();
	}

	CSyncHelper::getInstance()->getFileMutex()->unlock();
	return fullFileName;
}

/////////////////////////////////////////////////////////////////////

CResourceResult::CResourceResult()
{
	isValid = true;
	isSuccess = true;

	fileData = NULL;
	fileSize = 0;
	message = "";
	cacheControlStr = "private, max-age=2592000";
	etag = "";
	mime = "text/html; charset=utf-8";
}

CResourceResult::CResourceResult(const CResourceResult& result)
{
	setIsValid(result.getIsValid());
	setIsSuccess(result.getIsSuccess());

	setData(result.getData());
	setDataSize(result.getDataSize());
	setCacheControlStr(result.getCacheControlStr());
	setMessage(result.getMessage());
	setEtag(result.getEtag());
	setMIME(result.getMIME());
}

CResourceResult::CResourceResult(const char* fileData, const long fileSize, const std::string& mime, const std::string& etag, const std::string& cacheControlStr)
{
	this->isValid = true;
	this->isSuccess = true;

	this->fileData = fileData;
	this->fileSize = fileSize;
	this->cacheControlStr = cacheControlStr;
	this->message = "";
	this->etag = etag;
	this->mime = mime;
}

CResourceResult::~CResourceResult()
{

}

void CResourceResult::setFile(const CFileDescriptor* file)
{
	fileData = file->getFileData();
	fileSize = file->getFileSize();
	mime = file->getFileMIME();
	etag = valueToString(file->getEtag());
}

const std::string CResourceResult::getCacheControlStr() const
{
	return cacheControlStr;
}

const std::string CResourceResult::getEtag() const
{
	return etag;
}

const std::string CResourceResult::getMIME() const
{
	return mime;
}

const std::string CResourceResult::getMessage() const
{
	return message;
}

const char* CResourceResult::getData() const
{
	return fileData;
}

long CResourceResult::getDataSize() const
{
	return fileSize;
}

bool CResourceResult::getIsValid() const
{
	return isValid;
}

bool CResourceResult::getIsSuccess() const
{
	return isSuccess;
}

void CResourceResult::setCacheControlStr(const std::string& cacheControlStr)
{
	this->cacheControlStr = cacheControlStr;
}

void CResourceResult::setEtag(const std::string& etag)
{
	this->etag = etag;
}

void CResourceResult::setMIME(const std::string& mime)
{
	this->mime = mime;
}

void CResourceResult::setMessage(const std::string& message)
{
	this->message = message;
}

void CResourceResult::setData(const char* fileData)
{
	this->fileData = fileData;
}

void CResourceResult::setDataSize(const long fileSize)
{
	this->fileSize = fileSize;
}

void CResourceResult::setIsValid(const bool isValid)
{
	this->isValid = isValid;
}

void CResourceResult::setIsSuccess(const bool isSuccess)
{
	this->isSuccess = isSuccess;
}

/////////////////////////////////////////////////////////////////////

CFileDescriptor::CFileDescriptor(const std::string& fullFileName, bool isAlwaysCached)
{
	isValid = false;
	this->fullFileName = fullFileName;
	this->isPermanentInCache = isAlwaysCached;

	loadFile();
}

CFileDescriptor::CFileDescriptor(const std::string& fileIdString, const char* fileData, const long dataLen, const std::string& fileMIME)
{
	isValid = true;
	this->fullFileName = fileIdString;
	this->isPermanentInCache = false;
	this->fileSize = dataLen;

	if(fileSize > 0 && fileData)
	{
		data = new char[fileSize];
		std::memcpy(data, fileData, dataLen);

		md5Cache = md5(fileData, (size_t)dataLen);
		MIME = fileMIME;
		fileType = MIMEType::getInstance()->getFileExt(MIME);
		etag = fileSize;
	}
	else
	{
		isValid = false;
	}
}

CFileDescriptor::~CFileDescriptor()
{
	delete [] data;
}

void CFileDescriptor::loadFile()
{
	std::ifstream file;

	struct stat fileStat;
	if(stat(fullFileName.c_str(), &fileStat) == 0)
	{
		fileSize 	= fileStat.st_size;
		etag 		= fileStat.st_ino;
	}
	else
	{
		CLog::getInstance()->addError("file load error");
		isValid = false;
		return;
	}

	try
	{
		file.open(fullFileName);
	}
	catch(std::exception& e)
	{
		CLog::getInstance()->addError(e.what());
		isValid = false;
		file.close();
		return;
	}

	data = new char[fileSize];	
	file.read(data, fileSize);	

	if(!file)
	{
		CLog::getInstance()->addError("file load error");
		isValid = false;
		delete [] data;
		file.close();
		return;
	}	
	md5Cache = md5(data, fileSize);
	
	size_t dotPos = fullFileName.find_last_of(".");
	if(dotPos == std::string::npos)
	{
		fileType = "none";
	}
	else
	{
		fileType = fullFileName.substr(dotPos + 1, std::string::npos);
	}
	MIME = MIMEType::getInstance()->get(fileType);
	
	isValid = true;
	file.close();
}

bool CFileDescriptor::getIsValid() const
{
	return isValid;
}

bool CFileDescriptor::getIsPermanentCached() const
{
	return isPermanentInCache;
}

const char* CFileDescriptor::getFileData() const
{
	return data;
}

const std::string CFileDescriptor::getFullFileName() const
{
	return fullFileName;
}

const std::string CFileDescriptor::getFileType() const
{
	return fileType;
}

const std::string CFileDescriptor::getFileMIME() const
{
	return MIME;
}

const std::string CFileDescriptor::getFileMD5Cache() const
{
	return md5Cache;
}

long CFileDescriptor::getFileSize() const
{
	return fileSize;
}

long CFileDescriptor::getEtag() const
{
	return etag;
}

//////////////////////////////////////////////////////////////////////////

void  CFileCache::addFileToCache(const CFileDescriptor* file)
{
	if(!file->getIsValid()) return;
	if(findFile(file->getFullFileName())) return;
	if(cacheSize >= maxCacheSize) freeCacheForFile(file);

	CSyncHelper::getInstance()->getResourceMutex()->lock();

	files.push_back(file);
	cacheSize += file->getFileSize();
	CLog::getInstance()->addInfo("Add file to cache, cache size: "+ valueToString(cacheSize/1024) + " Kb" );

	CSyncHelper::getInstance()->getResourceMutex()->unlock();
}

const CFileDescriptor* CFileCache::addFileToCache(const std::string& fileName, bool isAlwaysInCache)
{
	const CFileDescriptor* chFile = findFile(fileName);
	if(chFile) return chFile;

	CSyncHelper::getInstance()->getResourceMutex()->lock();
	CFileDescriptor* file = new CFileDescriptor(fileName, isAlwaysInCache);
	CSyncHelper::getInstance()->getResourceMutex()->unlock();

	if(file->getIsValid()) addFileToCache(file);

	return file;
}

const CFileDescriptor* CFileCache::findFile(const std::string& fileName)
{
	CSyncHelper::getInstance()->getResourceMutex()->lock();

	for(unsigned int i = 0; i < files.size(); i++)
    {
		const CFileDescriptor* currFile = files[i];

		if(!currFile) continue;
		if(!currFile->getIsValid()) continue;
		if(currFile->getFullFileName().compare(fileName) == 0 )
		{
			CSyncHelper::getInstance()->getResourceMutex()->unlock();
			return currFile;
		}
	}
	CSyncHelper::getInstance()->getResourceMutex()->unlock();
	return NULL;
}

const CFileDescriptor* CFileCache::findOrAddFile(const std::string& fileName, bool isAlwaysInCache)
{
	const CFileDescriptor* currFile = findFile(fileName);
	if(!currFile)
	{			
		currFile = addFileToCache(fileName, isAlwaysInCache);
	}
	return currFile;
}

void CFileCache::cleanUnusedFiles()
{
	CSyncHelper::getInstance()->getResourceMutex()->lock();

	CLog::getInstance()->addInfo("Cache clean...");

	for(auto it = files.begin(); it != files.end(); )
    {
		const CFileDescriptor* currFile = (*it);
		if(!currFile->getIsPermanentCached())
		{
			cacheSize -= currFile->getFileSize();
			it = files.erase(it);
			delete currFile;
		}
		else
		{
			++it;
		}
	}

	CSyncHelper::getInstance()->getResourceMutex()->unlock();
}

void CFileCache::freeCacheForFile(const CFileDescriptor* file)
{
	CConfigHelper* settingsManager = CConfigHelper::getInstance();
	long sizeToClean = file->getFileSize() + cacheSize - settingsManager->getLongParamValue("maxFileCacheSize", 40*1024*1024);

	CSyncHelper::getInstance()->getResourceMutex()->lock();

	CLog::getInstance()->addInfo("Cache clean...");

	for(auto it = files.begin(); it != files.end(); )
    {
		const CFileDescriptor* currFile = (*it);
		if(!currFile->getIsPermanentCached())
		{
			cacheSize   -= currFile->getFileSize();
			sizeToClean -= currFile->getFileSize();
			it = files.erase(it);
			delete currFile;
		}
		else
		{
			++it;
		}
		if(sizeToClean <= 0) break;
	}

	CSyncHelper::getInstance()->getResourceMutex()->unlock();
}

bool CFileCache::isFileInCache(const std::string& fileName)
{
	if(findFile(fileName)) return true;
	return false;
}

long CFileCache::getMaxCacheSize() const
{
	return maxCacheSize;
}

long CFileCache::getCacheSize() const
{
	return cacheSize;
}

CFileCache::CFileCache(const long maxCacheSize)
{
	this->maxCacheSize = maxCacheSize;
	cacheSize = 0;
}

CFileCache::~CFileCache()
{
    for(unsigned int i = 0; i < files.size(); i++)
    {
		delete files[i];
	}
}
