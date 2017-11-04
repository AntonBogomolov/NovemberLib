#include "../../Include/novemberlib/pages/CErrorPage.h"

CErrorPage::CErrorPage(const std::string& name, const std::string& errorMessage, const std::string& errorCode) : CPage(name, NULL)
{
	setErrorCode(errorCode);
	setErrorMessage(errorMessage);
}

CErrorPage::~CErrorPage()
{
	//dtor
}

std::string CErrorPage::buildContent() const
{
	std::string tmpStr;

	tmpStr = "<h1>";
	tmpStr += errorCode;
	tmpStr += "</h1> \n";
	tmpStr += "<br> \n";
	tmpStr += "<h3>";
	tmpStr += errorMessage;
	tmpStr += "</h3> \n";

	return tmpStr;
}
