#include "../Include/novemberlib/CDefaultUser.h"

#include "../Include/novemberlib/utils/utils.h"
#include "../Include/novemberlib/utils/md5.h"

#include "../Include/novemberlib/utils/CLog.h"
#include "../Include/novemberlib/CBan.h"

#include "../Include/novemberlib/helpers/CConfigHelper.h"
#include "../Include/novemberlib/managers/CManagers.h"
#include "../Include/novemberlib/managers/CDBManager.h"
#include "../Include/novemberlib/managers/CSessionManager.h"

#include <cstdlib>


CDefaultUser::CDefaultUser() :
    cookie(""), key(""), pass(""), login(""), salt(""), remouteAddress("127.0.01"),
    userAgent(" "), userId(0), userType(0), lastChange(0), lastAction(0)
{
    setIsValid(false);
}

CDefaultUser::CDefaultUser(const int userId) :
    cookie(""), key(""), pass(""), login(""), salt(""), remouteAddress("127.0.01"),
    userAgent(" "), userId(userId), userType(0), lastChange(0), lastAction(0)
{
    setIsValid(false);
}

CDefaultUser::~CDefaultUser()
{
	for(auto it = bans.begin(); it != bans.end(); ++it)
	{
		CBan* currBan = (*it);
		delete currBan;
	}
}

const std::string& CDefaultUser::getCookie() const
{
	return cookie;
}

int CDefaultUser::getUserId() const
{
	return userId;
}

int CDefaultUser::getUserType() const
{
	if(userType >= UT_ADMIN) return UT_ADMIN;
	if(userType == UT_MODER) return UT_MODER;
	if(userType <= UT_NONE)	 return UT_NONE;

	return UT_USER;
}

long CDefaultUser::getLastChange() const
{
	return lastChange;
}

long CDefaultUser::getLastAction() const
{
	return lastAction;
}

bool CDefaultUser::getIsUserGuest() const
{
	CConfigHelper* gs = CConfigHelper::getInstance();

	if(cookie == gs->getStringParamValue("guestCookieValue", "GUEST")) return true;
	return false;
}

bool CDefaultUser::getIsAccessClosed() const
{
	for(auto it = bans.begin(); it != bans.end(); ++it)
	{
		CBan* currBan = (*it);

		if(currBan->getIsAccessClosed()) return true;
	}
	return false;
}

long CDefaultUser::getReadOnlyBanTime() const
{
	time_t currUnixTime;
	time(&currUnixTime);

	long maxROTime = 0;
	for(auto it = bans.begin(); it != bans.end(); ++it)
	{
		CBan* currBan = (*it);

		long timeToEndRO = 	currBan->getBanCreadtionTime() + currBan->getROTime() - (long)currUnixTime;
		if(timeToEndRO > maxROTime) maxROTime = timeToEndRO;
	}
	return maxROTime;
}

CDefaultUser* CDefaultUser::loginUser(const std::string& login, const std::string& pass)
{
	int userId = authorization(login, pass);
	if(userId > 0)
	{
		CSessionManager* sessionManager =  CManagers::getInstance()->getSessionManager();
		CDefaultUser* newUser = sessionManager->userCreationFunc();

		CDBManager* dbManager = CManagers::getInstance()->getDBManager();
		std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());
		dbRequest->updateRequest("Users", "`key` = '" + dbManager->getEscapeString(genUserKey()) + "'", "`id` = " + valueToString(userId));

		newUser->fillUserDataById(userId);
		return newUser;
	}

	return NULL;
}

int CDefaultUser::authorization(const std::string& login, const std::string& pass)
{
	CConfigHelper* gs = CConfigHelper::getInstance();
	bool isHasDB = gs->getStringParamValue("isHasDB", "false") == "true";
	if(!isHasDB) return 0;

	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

	const CDBRequestResult* result = dbRequest->selectRequest(CDBValues("id,salt,pass"), "Users", "`login` = '"+dbManager->getEscapeString(login)+"'", "LIMIT 1" );
	if(dbRequest->getIsLastQuerySuccess() && result != NULL  && result->getRowsCnt() > 0)
	{
		int id = result->getIntValue(0,0);
		if(id <= 0) return 0;
		std::string salt = result->getStringValue(0,1);
		std::string userPass = result->getStringValue(0,2);

		std::string testedPassHash = md5(md5(pass) + salt);
		if(userPass == testedPassHash) return id;

		CLog::getInstance()->addWarning("Wrong pass");
	}
	CLog::getInstance()->addWarning("Login: " + login + " not exist");
	return 0;
}

CDefaultUser* CDefaultUser::createNewUser(const std::string& login, const std::string& pass, const std::string& cookieData, const std::string& remouteAddress, const  std::string& userAgent)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());
	time_t currUnixTime;
	time(&currUnixTime);

	CSessionManager* sessionManager = CManagers::getInstance()->getSessionManager();
	CDefaultUser* newUser = sessionManager->userCreationFunc();
	CLog::getInstance()->addInfo("New User");

	std::string key = cookieData;
	if(key.length() == 0) key = genUserKey();

	CConfigHelper* gs = CConfigHelper::getInstance();
	bool isHasDB = gs->getStringParamValue("isHasDB", "false") == "true";
	if(isHasDB)
	{
		const CDBRequestResult* result = dbRequest->selectRequest(CDBValues("id"), "Users", "`login` = '"+dbManager->getEscapeString(login)+"'", "LIMIT 1" );
		if(dbRequest->getIsLastQuerySuccess() && result != NULL  && result->getRowsCnt() > 0)
		{
			CLog::getInstance()->addWarning("User login: " + login + " already exist");
			return newUser;
		}

		std::string salt = genUserKey();
		std::string userPass = md5(md5(pass) + salt);

		CDBValues fields;
		fields.addValue("remouteAddress", remouteAddress);
		fields.addValue("type", 1);
		fields.addValue("key", key);
		fields.addValue("login", login);
		fields.addValue("pass", userPass);
		fields.addValue("salt", salt);
		fields.addValue("lastAction", (long)currUnixTime);
		fields.addValue("userAgent", userAgent);
		dbRequest->insertRequest(fields, "Users");
	}
	newUser->fillUserData(key);

	return newUser;
}

void CDefaultUser::fillUserDataById(const int userId)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

	CConfigHelper* gs = CConfigHelper::getInstance();
	bool isHasDB = gs->getStringParamValue("isHasDB", "false") == "true";
	if(!isHasDB)
	{
		fillUserData(gs->getStringParamValue("guestCookieValue", "GUEST"));
		return;
	}

	const CDBRequestResult* result = dbRequest->selectRequest(CDBValues("key"), "Users", "`id` = "+valueToString(userId), "LIMIT 1" );
	if(dbRequest->getIsLastQuerySuccess() && result != NULL  && result->getRowsCnt() > 0)
	{
		std::string key = result->getStringValue(0,0);
		if((int)key.size() > 0) fillUserData(key);
	}
}

bool CDefaultUser::isUserExist(const std::string& userKey)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

	const CDBRequestResult* result = dbRequest->selectRequest(CDBValues("id"), "Users", "`key` = '" + dbManager->getEscapeString(userKey) + "'", "LIMIT 1");
	if (result != NULL  && result->getRowsCnt() > 0) return true;

	return false;
}

void CDefaultUser::fillUserData(const std::string& cookieData)
{
	CConfigHelper* gs = CConfigHelper::getInstance();
	int userId = 0;
	int userType = 0;

	this->userId = 1;
    this->userType = UT_USER;
    this->key = cookieData;
    this->lastChange = 0;
    this->lastAction = 0;
    this->userAgent = "";
    this->pass = "";
    this->salt = "";
    this->login = "";
    this->cookie = gs->getStringParamValue("guestCookieValue", "GUEST");

	bool isHasDB = gs->getStringParamValue("isHasDB", "false") == "true";
	if(!isHasDB || cookieData == gs->getStringParamValue("guestCookieValue", "GUEST"))
	{
		this->userId = 1;
		this->userType = UT_USER;
		this->key = cookieData;
		this->lastChange = 0;
		this->lastAction = 0;
		this->userAgent = "";
		this->pass = "";
		this->salt = "";
		this->login = "";
		this->cookie = gs->getStringParamValue("guestCookieValue", "GUEST");

		setIsValid(true);
		return;
	}

	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

	const CDBRequestResult* result = dbRequest->selectRequest(CDBValues("id,type,remouteAddress,lastChange,lastAction,userAgent,login,pass,salt"), "Users", "`key` = '" + dbManager->getEscapeString(cookieData)  + "'", "LIMIT 1");
	if(dbRequest->getIsLastQuerySuccess() && result != NULL  && result->getRowsCnt() > 0)
	{
		try
		{
			userId   = result->getIntValue(0,0);
			userType = result->getIntValue(0,1);
		}
		catch(...)
		{
			userId   = 0;
			userType = 0;
		}

		key = cookieData;
		remouteAddress = result->getStringValue(0,2);
		lastChange = result->getLongValue(0,3);
		lastAction = result->getLongValue(0,4);
		userAgent = result->getStringValue(0,5);
		login = result->getStringValue(0,6);
		pass = result->getStringValue(0,7);
		salt = result->getStringValue(0,8);
	}

	this->cookie = cookieData;
	this->userId = userId;
	this->userType = userType;

	setIsValid(true);
	if(cookie == "" || userId == 0 || userType < 0 || key == "") setIsValid(false);

	if(getIsValid())
	{
		const CDBRequestResult* result = dbRequest->selectRequest(CDBValues("*"), "Bans", "`userId` = "+valueToString(userId));
		if(dbRequest->getIsLastQuerySuccess() && result != NULL && result->getRowsCnt() > 0)
		{
			time_t currUnixTime;
			time(&currUnixTime);

			for(int i = 0; i < result->getRowsCnt(); i++)
			{
				int banId = 0;
				int banUserId = 0;
				long banCreationTime = 0;
				long banROTime = 0;
				bool banIsAccessClosed;

				banId = result->getIntValue(i, 0);
				banUserId = result->getIntValue(i, 1);
				banROTime = result->getLongValue(i, 2);
				banIsAccessClosed = result->getBoolValue(i, 3);
				banCreationTime = result->getLongValue(i, 4);

				if(banId != 0 && banUserId != 0 && (banIsAccessClosed || currUnixTime <= banCreationTime + banROTime ))
				{
					CBan* ban = new CBan(userId, banROTime, banCreationTime, banUserId, banIsAccessClosed);
					bans.push_back(ban);
				}
				else
				{
					// delete ban
				}
			}
		}
		afterFillUserData();
	}
}

void CDefaultUser::afterFillUserData()
{

}

const std::string& CDefaultUser::getLogin() const
{
	return login;
}

const std::string& CDefaultUser::getPass() const
{
	return pass;
}

const std::string& CDefaultUser::getSalt() const
{
	return salt;
}

const std::string& CDefaultUser::getUserKey() const
{
	return key;
}

const std::string& CDefaultUser::getRemouteAddress() const
{
	return remouteAddress;
}

const std::string& CDefaultUser::getUserAgent() const
{
	return userAgent;
}

const std::vector<CBan*>* CDefaultUser::getUserBans() const
{
	return &bans;
}

std::string CDefaultUser::genUserKey()
{
	char key[17];
	for(int i = 0; i < 16; i++)
	{
		key[i] = rand() % 57 + 65;
		if(key[i] >= 91 && key[i] <= 96) key[i] -= 6;
	}
	key[16] = '\0';

	std::string resultStr = key;
	return resultStr;
}
