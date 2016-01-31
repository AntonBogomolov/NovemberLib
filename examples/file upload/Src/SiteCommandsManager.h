#pragma once
#include "novemberlib/novemberlib.h"

class CSiteCommandsManager : public CCommandsManager
{
public:
	virtual CCommandResult processCommand(CFCGIRequest* currRequest);
	CCommandResult loginCommand(CFCGIRequest* currRequest) const;
	CCommandResult fileUploadCommand(CFCGIRequest* currRequest) const;
	CCommandResult fileDelCommand(CFCGIRequest* currRequest) const;
	
	CSiteCommandsManager();
	virtual ~CSiteCommandsManager();
private:
	const std::string handleFiles(CFCGIRequest* currRequest) const;
};
