#ifndef CRESOURCEMANAGER_H
#define CRESOURCEMANAGER_H

#include "../interfaces/IUncopyable.h"

#include <string>
#include <vector>

class CFCGIRequest;

class CFileDescriptor : IUncopyable
{
	public:
		const std::string getFullFileName() const;
		const std::string getFileType() const;
		const std::string getFileMIME() const;
		const std::string getFileMD5Cache() const;
		long  getFileSize() const;
		const char* getFileData() const;
		long getEtag() const;

		bool getIsValid() const;
		bool getIsPermanentCached() const;

		CFileDescriptor(const std::string& fullFileName, bool isAlwaysCached = false);
		CFileDescriptor(const std::string& fileIdString, const char* fileData, const long dataLen, const std::string& fileMIME);
		virtual ~CFileDescriptor();
	private:
		std::string fileType;
		std::string fullFileName;
		std::string md5Cache;
		std::string MIME;
		bool isValid;
		bool isPermanentInCache;
		char* data;

		long fileSize;
		long etag;

		void loadFile();
};

class CFileCache : IUncopyable
{
	public:
		void addFileToCache(const CFileDescriptor* file);
		const CFileDescriptor* addFileToCache(const std::string& fileName, bool isAlwaysInCache);
		const CFileDescriptor* findFile(const std::string& fileName);
		const CFileDescriptor* findOrAddFile(const std::string& fileName, bool isAlwaysInCache);
		bool isFileInCache(const std::string& fileName);
		long getMaxCacheSize() const;
		long getCacheSize() const;

		CFileCache(const long maxCacheSize);
		virtual ~CFileCache();
	private:
		void cleanUnusedFiles();
		void freeCacheForFile(const CFileDescriptor* file);

		std::vector<const CFileDescriptor*> files;
		long maxCacheSize;
		long cacheSize;
};

class CResourceResult
{
    public:
		const std::string getCacheControlStr() const;
		const std::string getEtag() const;
		const std::string getMIME() const;
		const std::string getMessage() const;
		const char* getData() const;
		long getDataSize() const;
		bool getIsValid() const;
		bool getIsSuccess() const;

		void setCacheControlStr(const std::string& cacheControlStr);
		void setEtag(const std::string& etag);
		void setMIME(const std::string& mime);
		void setMessage(const std::string& message);
		void setData(const char* fileData);
		void setDataSize(const long fileSize);
		void setIsValid(const bool isValid);
		void setIsSuccess(const bool isSuccess);
		void setFile(const CFileDescriptor* file);

		CResourceResult();
		CResourceResult(const CResourceResult& result);
		CResourceResult(const char* fileData, const long fileSize, const std::string& mime, const std::string& etag, const std::string& cacheControlStr = "private, max-age=2592000" );
		~CResourceResult();
    private:
		std::string cacheControlStr;
		std::string etag;
		std::string mime;
		std::string message;
		const char* fileData;
		long fileSize;

		bool isValid;
		bool isSuccess;
};

class CResourceManager
{
	public:
		static const std::string loadTextFile(const std::string& fullFileName);
		static const std::string saveFile(const std::string& fileName, const std::string& fileMimeType, const long fileSize, const char* fileData);

		virtual CResourceResult processResourceRequest(CFCGIRequest* currRequest);

		bool getIsNeedProcessResourceRequest() const;
		void setIsNeedProcessResourceRequest(const bool value);

		CResourceManager();
		virtual ~CResourceManager();
	protected:
		CFileCache* fileCache;
		bool isNeedProcessResourceRequest;
	private:
};

#endif // CRESOURCEMANAGER_H
