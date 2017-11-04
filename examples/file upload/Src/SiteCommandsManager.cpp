#include "SiteCommandsManager.h"

#include "Attach.h"
#include "SiteUser.h"

CSiteCommandsManager::CSiteCommandsManager()
{

}

CSiteCommandsManager::~CSiteCommandsManager()
{

}

CCommandResult CSiteCommandsManager::processCommand(CFCGIRequest* currRequest)
{
	CFCGIRequestHandler* request = currRequest->getRequestForModify();
	const std::string command = request->get.get("command", "");
	CCommandResult result;
	result.setData("Not valid user");
	if (command.size() == (size_t)0)
	{
		result.setIsValid(false);
		result.setData("Not valid command");
		return result;
	}

	if (command == "login")    return loginCommand(currRequest);
	if (command == "sendfile") return fileUploadCommand(currRequest);
	if (command == "delfile")  return fileDelCommand(currRequest);

	result.setIsValid(false);
	result.setData("Not valid command");
	return result;
}

CCommandResult CSiteCommandsManager::loginCommand(CFCGIRequest* currRequest) const
{
	CFCGIRequestHandler* request = currRequest->getRequestForModify();
	CCommandResult commandResult;
	commandResult.setData("Not valid input data");

	std::string login = request->post.get("login", "");
	std::string pass  = request->post.get("pass", "");
	std::string newUserStr = request->post.get("newuser", "off");
	bool isNewUser = newUserStr == "newuser" ? 1 : 0;

	if (login.length() == (size_t)0 || pass.length() == (size_t)0) return commandResult;

	CSessionManager* sessionManager = CManagers::getInstance()->getSessionManager();
	if (isNewUser)
	{
		sessionManager->registerUser(currRequest, login, pass);
		CLog::getInstance()->addInfo("new user:" + login + " " + pass);
	}
	else
	{
		sessionManager->loginUser(currRequest, login, pass);
		CLog::getInstance()->addInfo("login:" + login + " " + pass);
	}

	std::string returnPage = "upload";
	commandResult.setIsSuccess(true);
	commandResult.setData("<meta http-equiv='refresh' content='0; url=?page=" + returnPage + "' />");
	return commandResult;
}

CCommandResult CSiteCommandsManager::fileUploadCommand(CFCGIRequest* currRequest) const
{
	CFCGIRequestHandler* request = currRequest->getRequestForModify();
	CCommandResult commandResult;
	commandResult.setData("Not valid input data");

	std::string userText = request->post.get("userText", "");

	if (!isUserIdentity(currRequest))
	{
		commandResult.setData("Not valid user");
		return commandResult;
	}

	if (isUserReadOnly(currRequest))
	{
		commandResult.setData("User in read only mode");
		return commandResult;
	}
	CSiteUser* currUser = dynamic_cast<CSiteUser*>(currRequest->getUserForModify());
	if (!currUser || !currUser->getIsValid())
	{
		commandResult.setData("Not valid user");
		return commandResult;
	}

	currUser->attachFile(handleFiles(currRequest));

	std::string returnPage = "upload";
	commandResult.setIsSuccess(true);
	commandResult.setData("<meta http-equiv='refresh' content='0; url=?page=" + returnPage + "' />");
	return commandResult;
}

CCommandResult CSiteCommandsManager::fileDelCommand(CFCGIRequest* currRequest) const
{	
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	std::shared_ptr<CDBRequest> dbRequest(dbManager->createDBRequest());
	CFCGIRequestHandler* request = currRequest->getRequestForModify();
	CCommandResult commandResult;
	commandResult.setData("Not valid input data");

	std::string fileIdStr = request->post.get("id", "");
	int fileId = 0;
	try
	{
		fileId = std::stoi(fileIdStr);
	}
	catch (const std::exception&)
	{
		fileId = 0;
		commandResult.setData("Not valid file id");
	}
	if (fileId == 0) return commandResult;

	if (!isUserIdentity(currRequest))
	{
		commandResult.setData("Not valid user");
		return commandResult;
	}

	if (isUserReadOnly(currRequest))
	{
		commandResult.setData("User in read only mode");
		return commandResult;
	}
	CSiteUser* currUser = dynamic_cast<CSiteUser*>(currRequest->getUserForModify());
	if (!currUser || !currUser->getIsValid())
	{
		commandResult.setData("Not valid user");
		return commandResult;
	}

	dbRequest->deleteRequest("Files", "`id` = " + valueToString(fileId));
	
	std::string returnPage = "upload";
	commandResult.setIsSuccess(true);
	commandResult.setData("<meta http-equiv='refresh' content='0; url=?page=" + returnPage + "' />");
	return commandResult;
}


const std::string CSiteCommandsManager::handleFiles(CFCGIRequest* currRequest) const
{
	int filesCnt = 1;
	CConfigHelper* gs = CConfigHelper::getInstance();

	std::string postParamName = "postfile";
	std::string attachStr = "";
	for (int i = 0; i < filesCnt; i++)
	{
		const CFileDescriptor* file = currRequest->getRequestForModify()->files->getFile(postParamName);
		if (file == NULL)
		{
			CLog::getInstance()->addError("NULL file: " + postParamName);
			continue;
		}

		const std::string fileName = file->getFullFileName();
		const std::string fileType = file->getFileMIME();
		long fileSize = file->getFileSize();
		const char* fileData = file->getFileData();
		
		if (fileSize == 0 || fileName == "" || fileData == 0 || (long)fileSize > gs->getLongParamValue("maxFileSize", 10485760l)) continue;
		CLog::getInstance()->addInfo(fileSize);

		std::string newFileName = md5(fileData, fileSize);
		std::string filePath = CManagers::getInstance()->getResourceManager()->saveFile(newFileName, fileType, fileSize, fileData);
		if (filePath.length() == 0) continue;

		int imgWidth = 256;
		int imgHeight = 256;

		attachStr += filePath + ";" + valueToString(fileSize) + ";" + valueToString(imgWidth) + ";" + valueToString(imgHeight);
		if (i < filesCnt - 1) attachStr += ";";
	}

	return attachStr;
}
