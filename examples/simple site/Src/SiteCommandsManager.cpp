<<<<<<< HEAD
#include "SiteCommandsManager.h"


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

	if (command == "getInfo") return getInfoCommand(currRequest);

	result.setIsValid(false);
	result.setData("Not valid command");
	return result;
}

CCommandResult CSiteCommandsManager::getInfoCommand(CFCGIRequest* currRequest) const
{
	CFCGIRequestHandler* request = currRequest->getRequestForModify();
	CCommandResult commandResult;
	commandResult.setData("Not valid input data");

	std::string userText = request->post.get("userText", "");
	std::string answer;

	if (userText.length() > (size_t)1024) return commandResult;
	if (userText.length() > (size_t)0)
=======
#include "SiteCommandsManager.h"


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

	if (command == "getInfo") return getInfoCommand(currRequest);

	result.setIsValid(false);
	result.setData("Not valid command");
	return result;
}

CCommandResult CSiteCommandsManager::getInfoCommand(CFCGIRequest* currRequest) const
{
	CFCGIRequestHandler* request = currRequest->getRequestForModify();
	CCommandResult commandResult;
	commandResult.setData("Not valid input data");

	std::string userText = request->post.get("userText", "");
	std::string answer;

	if (userText.length() > (size_t)1024) return commandResult;
	if (userText.length() > (size_t)0)
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
	{
		answer = "Hello, ";
		answer += userText;
		answer += '\n';
		answer += "Nice to meet you";
<<<<<<< HEAD

		commandResult.setIsSuccess(true);
		commandResult.setData(answer);
	}
	else
	{
		answer = "Hello, Stranger";
		commandResult.setIsSuccess(true);
		commandResult.setData(answer);
	}

	return commandResult;
=======

		commandResult.setIsSuccess(true);
		commandResult.setData(answer);
	}
	else
	{
		answer = "Hello, Stranger";
		commandResult.setIsSuccess(true);
		commandResult.setData(answer);
	}

	return commandResult;
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
}
