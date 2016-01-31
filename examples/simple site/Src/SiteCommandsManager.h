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

