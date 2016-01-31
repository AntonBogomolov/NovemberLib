#ifndef CDEFAULTCOMMANDMANAGER_H
#define CDEFAULTCOMMANDMANAGER_H

#include "CCommandsManager.h"

class CFCGIRequest;

class CDefaultCommandManager : public CCommandsManager
{
	public:
		virtual CCommandResult processCommand(CFCGIRequest* currRequest);

		CCommandResult aboutCommand(CFCGIRequest* currRequest) const;

		virtual void guestUserCheck(CFCGIRequest* currRequest) const;

		CDefaultCommandManager();
		virtual ~CDefaultCommandManager();
	protected:
	private:
};

#endif // CDEFAULTCOMMANDMANAGER_H
