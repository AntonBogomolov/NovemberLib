#include "../../Include/novemberlib/interfaces/IErrorMessage.h"

IErrorMessage::IErrorMessage()
{
	errorCode = "";
	errorMessage = "";
}

IErrorMessage::~IErrorMessage()
{
	//dtor
}

void IErrorMessage::setErrorCode(const std::string& errorCode)
{
	this->errorCode = errorCode;
}

void IErrorMessage::setErrorMessage(const std::string& errorMessage)
{
	this->errorMessage = errorMessage;
}

const std::string IErrorMessage::getErrorCode() const
{
	return errorCode;
}

const std::string IErrorMessage::getErrorMessage() const
{
	return errorMessage;
}
