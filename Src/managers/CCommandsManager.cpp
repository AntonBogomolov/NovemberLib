#include "../../Include/novemberlib/managers/CCommandsManager.h"

#include "../../Include/novemberlib/managers/CManagers.h"
#include "../../Include/novemberlib/managers/CDBManager.h"
#include "../../Include/novemberlib/FCGI/CFCGIRequest.h"
#include "../../Include/novemberlib/utils/utils.h"
#include "../../Include/novemberlib/utils/md5.h"
#include "../../Include/novemberlib/utils/CLog.h"
#include "../../Include/novemberlib/CDefaultUser.h"

#include <string>

CCommandsManager::CCommandsManager()
{
	isNeedProcessCommands = true;
}

CCommandsManager::~CCommandsManager()
{
	//dtor
}

bool CCommandsManager::getIsNeedProcessCommands() const
{
    return isNeedProcessCommands;
}

void CCommandsManager::setIsNeedProcessCommands(const bool value)
{
    isNeedProcessCommands = value;
}

bool CCommandsManager::isUserReadOnly(CFCGIRequest* currRequest) const
{
	if (!currRequest->getUser()) return false;

    if(!currRequest->getUser()->getIsValid()) return false;
    if(currRequest->getUser()->getReadOnlyBanTime() > 0) return true;
    return false;
}

bool CCommandsManager::isUserAccessClosed(CFCGIRequest* currRequest) const
{
	if (!currRequest->getUser()) return false;
    if(!currRequest->getUser()->getIsValid()) return false;

    return currRequest->getUser()->getIsAccessClosed();
}

bool CCommandsManager::isUserIdentity(CFCGIRequest* currRequest) const
{
	if (!currRequest->getUser()) return false;
    if(!currRequest->getUser()->getIsValid()) return false;

    std::string userIdStr = currRequest->getRequest()->post.get("user_id","0");
    int formUserId = 0;
    try
    {
        formUserId = std::stoi(userIdStr);
    }
    catch(...)
    {
        formUserId = 0;
    }
    if(formUserId != currRequest->getUser()->getUserId()) return false;

    return true;
}

bool CCommandsManager::checkLastChangeTime(CFCGIRequest* currRequest) const
{
	if (!currRequest->getUser()) return false;
    CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

    time_t currUnixTime;
    time(&currUnixTime);

    long lastChangeTime = 0;
    const CDBRequestResult* result = dbRequest->selectRequest(CDBValues("lastChange"), "Users", "`id` = "+valueToString(currRequest->getUser()->getUserId()), "LIMIT 1");
    if(dbRequest->getIsLastQuerySuccess() && result != NULL  && result->getRowsCnt() > 0)
    {
        try{ lastChangeTime = result->getIntValue(0,0); }
        catch(...){ lastChangeTime = 0; }
    }
    long dt = (long)currUnixTime - lastChangeTime;
    if(dt < 10) return false;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////

CCommandResult::CCommandResult()
{
    header = "text/html; charset=utf-8";
    data = "";
    type = CR_TEXT;
    isValid = true;
    isSuccess = false;
<<<<<<< HEAD
    binData = nullptr;
=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
}

CCommandResult::CCommandResult(const CCommandResult& result)
{
    setData(result.getData());
    setIsValid(result.getIsValid());
    setIsSuccess(result.getIsSuccess());
    setType(result.getType());
<<<<<<< HEAD
    if(result.getType() == CR_BIN) setBinData(result.getBinData());
=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
}

CCommandResult::CCommandResult(const std::string& data, const bool isValid, const int type)
{
    this->data = data;
    this->isValid = isValid;
    this->isSuccess = false;
    this->type = type;
    if(this->type == CR_TEXT)    this->header = "text/html; charset=utf-8";
    if(this->type == CR_JSON)    this->header = "application/json; charset=utf-8";
<<<<<<< HEAD
    if(this->type == CR_CSV)     this->header = "application/csv; charset=utf-8";
    if(this->type == CR_BIN)     this->setIsValid(false);
    binData = nullptr;
}

CCommandResult::CCommandResult(std::vector<uint8_t>* binData, const bool isValid)
{
    this->binData = binData;
    this->isValid = isValid;
    this->isSuccess = false;
    this->setType(CR_BIN);
=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
}

CCommandResult::~CCommandResult()
{

}

<<<<<<< HEAD
const std::string CCommandResult::getTextData() const
=======
const std::string CCommandResult::getData() const
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
{
    return data;
}

<<<<<<< HEAD
std::vector<uint8_t>* CCommandResult::getBinData() const
{
    return binData;
}

const char* CCommandResult::getData() const
{
    if(type != CR_BIN) return data.c_str();
    if(type == CR_BIN && binData) return (const char*)binData->data();

    return nullptr;
}

=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
const std::string CCommandResult::getHeader() const
{
    return header;
}

bool CCommandResult::getIsValid() const
{
    return isValid;
}

bool CCommandResult::getIsSuccess() const
{
    return isSuccess;
}

int CCommandResult::getType() const
{
    return type;
}

<<<<<<< HEAD
long CCommandResult::getDataSize() const
{
    if(type != CR_BIN) return data.length();
    else
    if(binData) return binData->size();

    return 0l;
}

void CCommandResult::setBinData(std::vector<uint8_t>* binData)
{
    this->binData = binData;
}

=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
void CCommandResult::setData(const std::string& data)
{
    this->data = data;
}

void CCommandResult::appendData(const std::string& data)
{
    this->data += data;
}

<<<<<<< HEAD
void CCommandResult::appendHeader(const std::string& headerData)
{
    this->header += "\r\n";
    this->header += headerData;
}

=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
void CCommandResult::setIsValid(const bool isValid)
{
    this->isValid = isValid;
}

void CCommandResult::setIsSuccess(const bool isSuccess)
{
    this->isSuccess = isSuccess;
}

void CCommandResult::setType(const int type)
{
    this->type = type;
    if(this->type == CR_TEXT)    this->header = "text/html; charset=utf-8";
    if(this->type == CR_JSON)    this->header = "application/json; charset=utf-8";
<<<<<<< HEAD
    if(this->type == CR_CSV)     this->header = "application/csv; charset=utf-8";
    if(this->type == CR_BIN)
    {
        this->header = "application/octet-stream";
        appendHeader("Content-Transfer-Encoding: binary");
        appendHeader("Content-Length: " + valueToString(getDataSize()));
    }
=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
}
