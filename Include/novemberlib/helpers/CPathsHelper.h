#ifndef CPATHSMANAGER_H
#define CPATHSMANAGER_H

#include "../interfaces/ITemplateSingleton.h"

#include <string>
#include <vector>

class CPath
{
	public:
		const std::string getRelativeDir() const;
		const std::string getDirName() const;
		const std::string getAllowedMIMEs() const;
		bool getIsMIMEFit(const std::string& resourceMIME,const bool isOnlyForSiteContent = false) const;
		bool getIsOnlyForSiteContent() const;

		CPath(const std::string& dir, const std::string& allowedMIMEs = "*/*",const bool isOnlyForSiteContent = false);
		~CPath();
	private:
		std::string dir;
		std::string allowedMIMEs;
		bool isOnlyForSiteContent;
};

class CPathsHelper : public ITemplateSingleton<CPathsHelper>
{
	public:
		void addPath(const std::string& dir, const std::string& allowedMIMEs = "*/*",const bool isOnlyForSiteContent = false);
		const CPath* getPath(const std::string& dir) const;
		const CPath* getRootPath() const;
		const std::vector<const CPath*> getPathsFitsForResourceMIME(const std::string& resourceMIME,const bool isOnlyForSiteContent = false) const;
		const CPath* getPathFitsForResourceMIME(const std::string& resourceMIME,const bool isOnlyForSiteContent = false) const;

		const std::string& getUserContentDir() const;
		const std::string& getSiteContentDir() const;

		virtual ~CPathsHelper();
	protected:
		std::vector<CPath*> paths;

		std::string userContentDirName;
		std::string siteContentDirName;
	private:
		friend CPathsHelper* ITemplateSingleton::getInstance();
		CPath* rootPath;
		CPathsHelper();
};

#endif // CPATHSMANAGER_H
