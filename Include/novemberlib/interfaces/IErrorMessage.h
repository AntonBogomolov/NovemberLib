#ifndef IERRORMESSAGE_H
#define IERRORMESSAGE_H

#include "../utils/config.h"

#include <string>

class IErrorMessage
{
	public:
		void setErrorCode(const std::string& errorCode);
		void setErrorMessage(const std::string& errorMessage);

		const std::string getErrorCode() const;
		const std::string getErrorMessage() const;

		IErrorMessage();
		virtual ~IErrorMessage();
	protected:
		std::string errorMessage;
		std::string errorCode;
	private:
};

#endif // IERRORMESSAGE_H
