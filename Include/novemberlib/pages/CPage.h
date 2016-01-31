#ifndef CPAGE_H
#define CPAGE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

class CFCGIRequest;

class CPage
{
	public:
		const std::string getPageName() const;
		const std::string getPageHTMLContent() const;

		template<class T> static CPage* createPage(const std::string& name, const CFCGIRequest* currRequest)
		{
			return new T(name, currRequest);
		}

		CPage(const std::string& name, const CFCGIRequest* currRequest);		
		virtual ~CPage();
	protected:
		virtual void buildPage() const;

		virtual std::string buildHeader() const;
		virtual std::string buildFooter() const;
		virtual std::string buildContent() const = 0;

		mutable bool isPageBuilded;
		mutable std::string pageHTMLContent;
		mutable std::string tmpString;

		std::string pageName;
				
		const CFCGIRequest* currRequest;
	private:
};

#endif // CPAGE_H
