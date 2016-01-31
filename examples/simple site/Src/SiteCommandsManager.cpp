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
	{
		answer = "Hello, ";
		answer += userText;
		answer += '\n';
		answer += "Nice to meet you";

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
}
