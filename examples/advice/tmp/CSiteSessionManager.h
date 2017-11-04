#ifndef CSITESESSIONMANAGER_H
#define CSITESESSIONMANAGER_H

#include "novemberlib/managers/CSessionManager.h"

class CFCGIRequest;

class CSiteSessionManager : public CSessionManager
{
	public:
		virtual bool checkSession(CFCGIRequest* currRequest);

		CSiteSessionManager();
		virtual ~CSiteSessionManager();
	protected:
	private:
};

#endif // CSITESESSIONMANAGER_H
