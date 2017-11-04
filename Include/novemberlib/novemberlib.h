#ifndef NOVEMBERLIB_H
#define NOVEMBERLIB_H

#include "interfaces/IErrorMessage.h"
#include "interfaces/IJSONSerializer.h"
#include "interfaces/IStringSerializer.h"
#include "interfaces/ITemplateSingleton.h"
#include "interfaces/IUncopyable.h"
#include "interfaces/IValidTester.h"

#include "utils/CCodeTimer.h"
#include "utils/CLog.h"
#include "utils/config.h"
#include "utils/md5.h"
#include "utils/MIMEType.h"
#include "utils/URI.h"
#include "utils/utils.h"
<<<<<<< HEAD
#include "utils/json.h"

#include "HTTPClient/IResultsStore.h"
#include "HTTPClient/CTasksManager.h"
#include "HTTPClient/CProxy.h"
#include "HTTPClient/CHTTPClient.h"
=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311

#include "FCGI/CFCGIApp.h"
#include "FCGI/CFCGIRequest.h"
#include "FCGI/CFCGIRequestHandler.h"

#include "helpers/CConfigHelper.h"
#include "helpers/CMessageParser.h"
#include "helpers/CPathsHelper.h"
#include "helpers/CSyncHelper.h"
#include "helpers/CTemplateHelper.h"
#include "helpers/CTextHelper.h"

#include "managers/CCommandsManager.h"
#include "managers/CDBManager.h"
#include "managers/CDefaultCommandManager.h"
#include "managers/CMySQLDBManager.h"
#include "managers/CPageManager.h"
#include "managers/CResourceManager.h"
#include "managers/CSessionManager.h"
#include "managers/CManagers.h"

#include "pages/CErrorPage.h"
#include "pages/CPage.h"

#include "CBan.h"
#include "CDefaultUser.h"
#include "CHTMLTemplate.h"

#endif // NOVEMBERLIB_H
