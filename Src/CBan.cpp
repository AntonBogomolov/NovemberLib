#include "../Include/novemberlib/CBan.h"

#include "../Include/novemberlib/managers/CManagers.h"
#include "../Include/novemberlib/managers/CDBManager.h"
#include "../Include/novemberlib/utils/utils.h"

<<<<<<< HEAD

=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
CBan::CBan(const int id)
{
	this->id = id;

	this->userId = 0;
	this->roTime = 0;
	this->banCreationTime = 0;
	this->isAccessClosed = false;

	readFromDB();
}

CBan::CBan(const int id, const long roTime, const long banCreationTime, const int userId, bool isAccessClosed)
{
	this->id = id;
	this->userId = userId;
	this->roTime = roTime;
	this->banCreationTime = banCreationTime;
	this->isAccessClosed = isAccessClosed;
}

CBan::~CBan()
{
	//dtor
}


void CBan::readFromDB()
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	//std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

	const CDBRequestResult* result = dbRequest->selectRequest(CDBValues("*"), "Bans", "`id` = "+valueToString(id));
	if(dbRequest->getIsLastQuerySuccess() && result != NULL && result->getRowsCnt() > 0)
	{
		userId = result->getIntValue(0,1);
		roTime = result->getLongValue(0,2);
		isAccessClosed  = result->getBoolValue(0,3);
		banCreationTime = result->getLongValue(0,4);
	}
}

int CBan::getId() const
{
	return id;
}

int CBan::getUserId() const
{
	return userId;
}

long CBan::getBanCreadtionTime() const
{
	return banCreationTime;
}

long CBan::getROTime() const
{
	return roTime;
}

bool CBan::getIsAccessClosed() const
{
	return isAccessClosed;
}
