<<<<<<< HEAD
#include "SiteUser.h"

#include "Attach.h"

CSiteUser::CSiteUser() : CDefaultUser()
{

}

CSiteUser::~CSiteUser()
{
	for (auto it = attachments.begin(); it != attachments.end(); ++it)
	{
		CAttachment* currAttach = (*it);
		delete currAttach;
	}
}

void CSiteUser::afterFillUserData()
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

	std::string attachStr = "";

	const CDBRequestResult* result = dbRequest->selectRequest(CDBValues("attach,id"), "Files", "`userId` = " + valueToString(userId));
	if (dbRequest->getIsLastQuerySuccess() && result != NULL  && result->getRowsCnt() > 0)
	{
		for (int row = 0; row < result->getRowsCnt(); ++row)
		{
			attachStr += result->getStringValue(row, 0, "");
			attachStr += ";";
			attachStr += valueToString(result->getIntValue(row, 1, 0));
			attachStr += ";";
		}
	}

	std::vector<CAttachment*>* processedAttachs = CAttachment::processAttachmentString(attachStr);
	if (processedAttachs == NULL) return;

	for (auto it = processedAttachs->begin(); it != processedAttachs->end(); ++it)
	{
		CAttachment* currAttach = (*it);
		attachments.push_back(currAttach);
	}
	delete processedAttachs;

}

void CSiteUser::attachFile(const std::string& attachStr)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

	CDBValues dbValues;
	dbValues.addValue("userId", (int)userId);
	dbValues.addValue("attach", attachStr);
	dbRequest->insertRequest(dbValues, "Files");
}

const std::vector<CAttachment*>* CSiteUser::getAttachments() const
{
	return &attachments;
}
=======
#include "SiteUser.h"

#include "Attach.h"

CSiteUser::CSiteUser() : CDefaultUser()
{

}

CSiteUser::~CSiteUser()
{
	for (auto it = attachments.begin(); it != attachments.end(); ++it)
	{
		CAttachment* currAttach = (*it);
		delete currAttach;
	}
}

void CSiteUser::afterFillUserData()
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

	std::string attachStr = "";

	const CDBRequestResult* result = dbRequest->selectRequest(CDBValues("attach,id"), "Files", "`userId` = " + valueToString(userId));
	if (dbRequest->getIsLastQuerySuccess() && result != NULL  && result->getRowsCnt() > 0)
	{
		for (int row = 0; row < result->getRowsCnt(); ++row)
		{
			attachStr += result->getStringValue(row, 0, "");
			attachStr += ";";
			attachStr += valueToString(result->getIntValue(row, 1, 0));
			attachStr += ";";
		}
	}

	std::vector<CAttachment*>* processedAttachs = CAttachment::processAttachmentString(attachStr);
	if (processedAttachs == NULL) return;

	for (auto it = processedAttachs->begin(); it != processedAttachs->end(); ++it)
	{
		CAttachment* currAttach = (*it);
		attachments.push_back(currAttach);
	}
	delete processedAttachs;

}

void CSiteUser::attachFile(const std::string& attachStr)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());

	CDBValues dbValues;
	dbValues.addValue("userId", (int)userId);
	dbValues.addValue("attach", attachStr);
	dbRequest->insertRequest(dbValues, "Files");
}

const std::vector<CAttachment*>* CSiteUser::getAttachments() const
{
	return &attachments;
}
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
