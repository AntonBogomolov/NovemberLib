#ifndef CMANAGERS_H
#define CMANAGERS_H

#include "../interfaces/ITemplateSingleton.h"

class CPageManager;
class CSessionManager;
class CCommandsManager;
class CResourceManager;
class CDBManager;

class CManagers : public ITemplateSingleton<CManagers>
{
	public:
		CPageManager* getPageManager() const;
		CSessionManager* getSessionManager() const;
		CCommandsManager* getCommandManager() const;
		CResourceManager* getResourceManager() const;
		CDBManager* getDBManager() const;

		void lock();

		void setPageManager(CPageManager* pageManager);
		void setSessionManager(CSessionManager* sessionManager);
		void setCommandManager(CCommandsManager* commandManager);
		void setResourceManager(CResourceManager* resourceManager);
		void setDBManager(CDBManager* dbManager);

		virtual ~CManagers();
	protected:
	private:
		CPageManager* 			pageManager;
		CSessionManager*		sessionManager;
		CCommandsManager*		commandManager;
		CResourceManager* 		resourceManager;
		CDBManager*				dbManager;
		bool isLocked;

		CManagers();
		friend CManagers* ITemplateSingleton<CManagers>::getInstance();
};

#endif // CMANAGERS_H
