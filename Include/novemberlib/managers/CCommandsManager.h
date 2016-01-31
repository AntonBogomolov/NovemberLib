#ifndef CCOMMANDSMANAGER_H
#define CCOMMANDSMANAGER_H

#include "../interfaces/IUncopyable.h"

#include <string>

class CFCGIRequest;

class CCommandResult
{
	public:
		static const int CR_TEXT = 0;
		static const int CR_JSON = 1;

		const std::string getData() const;
		const std::string getHeader() const;
		int getType() const;
		bool getIsValid() const;
		bool getIsSuccess() const;

		void setType(const int type);
		void setData(const std::string& data);
		void appendData(const std::string& data);
		void setIsValid(const bool isValid);
		void setIsSuccess(const bool isSuccess);

		CCommandResult();
		CCommandResult(const CCommandResult& result);
		CCommandResult(const std::string& data, const bool isValid, const int type = CR_TEXT);
		~CCommandResult();
	private:
		std::string header;
		std::string data;
		int type;
		bool isValid;
		bool isSuccess;
};

class CCommandsManager : IUncopyable
{
	public:
		virtual CCommandResult processCommand(CFCGIRequest* currRequest) = 0;

		virtual bool isUserReadOnly(CFCGIRequest* currRequest) const;
		virtual bool isUserAccessClosed(CFCGIRequest* currRequest) const;
		virtual bool isUserIdentity(CFCGIRequest* currRequest) const;
		virtual bool checkLastChangeTime(CFCGIRequest* currRequest) const;

		bool getIsNeedProcessCommands() const;
		void setIsNeedProcessCommands(const bool value);

		CCommandsManager();
		virtual ~CCommandsManager();
	protected:
		bool isNeedProcessCommands;
	private:
};

#endif // CCOMMANDSMANAGER_H
