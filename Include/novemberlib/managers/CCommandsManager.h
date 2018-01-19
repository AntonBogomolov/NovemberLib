#ifndef CCOMMANDSMANAGER_H
#define CCOMMANDSMANAGER_H

#include "../interfaces/IUncopyable.h"

#include <string>
#include <vector>

class CFCGIRequest;

class CCommandResult
{
	public:
		static const int CR_TEXT = 0;
		static const int CR_JSON = 1;
		static const int CR_CSV  = 2;
		static const int CR_BIN  = 3;

		const std::string getTextData() const;
		std::vector<uint8_t>* getBinData() const;
		const char* getData() const;
		const std::string getHeader() const;
		int getType() const;
		bool getIsValid() const;
		bool getIsSuccess() const;
		long getDataSize() const;

		void setType(const int type);
		void setBinData(std::vector<uint8_t>* binData);
		void setData(const std::string& data);
		void appendData(const std::string& data);
		void appendHeader(const std::string& headerData);
		void setIsValid(const bool isValid);
		void setIsSuccess(const bool isSuccess);

		void destroyData();

		CCommandResult();
		CCommandResult(const CCommandResult& result);
		CCommandResult(const std::string& data, const bool isValid, const int type = CR_TEXT);
		CCommandResult(std::vector<uint8_t>* binData, const bool isValid);
		~CCommandResult();
	private:
		std::string header;
		std::string data;
		std::vector<uint8_t>* binData;
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
