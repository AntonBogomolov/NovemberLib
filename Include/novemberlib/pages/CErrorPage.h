#ifndef CERRORPAGE_H
#define CERRORPAGE_H

#include "CPage.h"
#include "../interfaces/IErrorMessage.h"

class CErrorPage :  public CPage, public IErrorMessage
{
	public:
		CErrorPage(const std::string& name, const std::string& errorMessage, const std::string& errorCode);
		virtual ~CErrorPage();
	protected:
		virtual std::string buildContent() const;
	private:

};

#endif // CERRORPAGE_H
