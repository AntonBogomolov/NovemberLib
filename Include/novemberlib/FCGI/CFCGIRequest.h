#ifndef CFCGIREQUEST_H
#define CFCGIREQUEST_H

#include "CFCGIRequestHandler.h"
#include <string>
#include <sstream>

class CDefaultUser;

class CFCGIRequest
{
	public:
		bool response();
	
		const CDefaultUser* getUser() const;
		CDefaultUser* getUserForModify();
		void setUser(CDefaultUser* newUser);

		CFCGIRequestHandler* getRequestForModify();
		const CFCGIRequestHandler* getRequest() const;

		CFCGIRequest(CFCGIRequestHandler* request);
		~CFCGIRequest();
	protected:
	private:
		CDefaultUser* userData;
		CFCGIRequestHandler* currRequest;
};

#endif // CFCGIREQUEST_H
