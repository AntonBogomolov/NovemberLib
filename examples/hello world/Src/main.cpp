#include "novemberlib/novemberlib.h"

#include <iostream>

using namespace std;

class CIndexPage : public CPage
{
	public:
		CIndexPage(const std::string name, const CFCGIRequest* currRequest) : CPage(name, currRequest){}
		virtual ~CIndexPage(){}
	protected:
		virtual std::string buildHeader() const
		{
			return "<!DOCTYPE html><html><header><meta http-equiv='Content-Type' content='text/html; charset=utf-8'/><title>Hello World</title></header>";
		}
		virtual std::string buildContent() const
		{
			return "<body><h1>Hello World!</h1></body>";
		}
		virtual std::string buildFooter() const
		{
			return "</html>";
		}
};

class CApp : public CFCGIApp
{
	public:
		CApp(){}
		virtual ~CApp(){}
	protected:
		virtual void init()
		{
			CFCGIApp::init();

			CPageManager* pages = CManagers::getInstance()->getPageManager();
			pages->addPageType<CIndexPage>("index");
		}
};

int main()
{
    CApp app;
    app.run();
    return 0;
}
