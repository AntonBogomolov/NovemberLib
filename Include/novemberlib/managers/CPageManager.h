#ifndef CPAGEMANAGER_H
#define CPAGEMANAGER_H

#include "../pages/CPage.h"
#include "../interfaces/IUncopyable.h"

#include <string>
#include <vector>

class CFCGIRequest;
class CErrorPage;

class CPageManager : public IUncopyable
{
	public:
		typedef CPage* (*factoryFuncPtr)(const std::string& name, const CFCGIRequest* currRequest);
		typedef std::map<std::string, factoryFuncPtr> PageTypes;

		virtual const std::string getPageContent(const std::string& pageName, const CFCGIRequest* currRequest) const;
		void setErrorPage(CErrorPage* errorPage);
		const std::string getErrorPageContent(const std::string& errorCode, const std::string& errorMessage) const;

		bool getIsNeedProcessPages() const;
		void setIsNeedProcessPages(const bool value);
				
		template<typename Type>	void addPageType(const std::string& pageName)
		{
			factoryFuncPtr function = &CPage::createPage<Type>;
			pageTypes.insert(std::make_pair(pageName, function));
		}
		
		CPage* createPage(const std::string& name, const CFCGIRequest* currRequest) const
		{
			CPageManager::PageTypes::const_iterator it = pageTypes.find(name);
			if (it == pageTypes.end()) return NULL;

			return (*(it->second))(name, currRequest);
		}

		bool getIsPageExist(const std::string& name)
		{
			CPageManager::PageTypes::const_iterator it = pageTypes.find(name);
			if (it == pageTypes.end()) return false;

			return true;
		}

		CPageManager();
		virtual ~CPageManager();
	protected:
		bool isNeedProcessPages;	
		mutable CErrorPage* errorPage;
		PageTypes pageTypes;
	private:
};

#endif // CPAGEMANAGER_H
