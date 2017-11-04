<<<<<<< HEAD
#pragma once
#include "novemberlib/novemberlib.h"

class CSiteCommandsManager : public CCommandsManager
{
public:
	virtual CCommandResult processCommand(CFCGIRequest* currRequest);
	CCommandResult getInfoCommand(CFCGIRequest* currRequest) const;

	CSiteCommandsManager();
	virtual ~CSiteCommandsManager();
};

=======
#pragma once
#include "novemberlib/novemberlib.h"

class CSiteCommandsManager : public CCommandsManager
{
public:
	virtual CCommandResult processCommand(CFCGIRequest* currRequest);
	CCommandResult getInfoCommand(CFCGIRequest* currRequest) const;

	CSiteCommandsManager();
	virtual ~CSiteCommandsManager();
};

>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
