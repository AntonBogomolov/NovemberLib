#ifndef CDBMANAGER_H
#define CDBMANAGER_H

#include "../interfaces/IUncopyable.h"

#include <string>
#include <vector>
#include <memory>

class CDBConnection
{
	public:
		CDBConnection(){};
		virtual ~CDBConnection(){};
};

class CDBValue
{
	public:
		const std::string getFieldName() const;
		const std::string getValue() const;
		bool getIsTextValue() const;

		CDBValue();
		CDBValue(const std::string& fieldName, const std::string& value, bool isTextValue = true);
		~CDBValue();
	protected:
		std::string fieldName;
		std::string value;
		bool isTextValue;
	private:
};

class CDBValues
{
	public:
		void addValue(const std::string& fieldName, const std::string& value, const bool isNeedToEscape = true );
		void addValue(const std::string& fieldName, const int value);
		void addValue(const std::string& fieldName, const long value);
		void addValue(const CDBValue& value);
		void addValue(const CDBValue* value);

		const std::vector<const CDBValue*>* getValues() const;

		CDBValues();
		CDBValues(const std::string& fieldNames);
		~CDBValues();
	protected:
		std::vector<const CDBValue*> values;
	private:
};

class CDBRequestResult
{
	public:
		virtual int getFiledsCnt() const = 0;
		virtual int getRowsCnt() const = 0;

		virtual const std::string getStringValue(const int rowIndex, const int fieldIndex, const std::string& defValue = "") const = 0;
		virtual int  getIntValue(const int rowIndex, const int fieldIndex,  const int defValue = 0) const = 0;
		virtual long getLongValue(const int rowIndex, const int fieldIndex, const long defValue = 0) const = 0;
<<<<<<< HEAD
		virtual float  getFloatValue(const int rowIndex, const int fieldIndex, const float defValue = 0.0f)  const = 0;
		virtual double getDoubleValue(const int rowIndex, const int fieldIndex, const double defValue = 0.0) const = 0;
=======
>>>>>>> 687e9bc0110018ae2de42439f759cb7ad70af311
		virtual bool getBoolValue(const int rowIndex, const int fieldIndex, const bool defValue = false) const = 0;

		CDBRequestResult(){rowsCnt=0;fieldsCnt=0;};
		virtual ~CDBRequestResult(){};
	protected:
		int rowsCnt;
		int fieldsCnt;

};

class CDBRequest
{
public:
	bool getIsLastQuerySuccess() const;
    virtual const CDBRequestResult* createRequest(const std::string& requestString) = 0;

    void freeLastResult();
    virtual CDBRequestResult* getLastResult() const;
    const CDBConnection* getDBConnection() const;

	virtual const CDBRequestResult* selectRequest(const CDBValues& fields, const std::string& from, const std::string& where = "", const std::string& options = "") = 0;
	virtual const CDBRequestResult* deleteRequest(const std::string& from, const std::string& where = "", const std::string& options = "") = 0;
	virtual const CDBRequestResult* insertRequest(const CDBValues& fieldsAndValues, const std::string& into, const std::string& options = "") = 0;
	virtual const CDBRequestResult* updateRequest(const std::string& from, const std::string& values, const std::string& where = "", const std::string& options = "") = 0;

    CDBRequest(const CDBConnection* dbConnection);
    virtual ~CDBRequest();
protected:
    bool isLastQuerySuccess;

    const CDBConnection* dbConnection;
    CDBRequestResult* res;
};

class CDBManager : public IUncopyable
{
	public:
		virtual void initDB(const std::string& dbName, const std::string& userName, const std::string& pass, const std::string& host) = 0;
		virtual void reconnect();
		virtual void closeDBConnection() = 0;

		bool getIsConnInit() const;
		const CDBConnection* getDBConnection() const;

		virtual CDBRequest* createDBRequest() const = 0;
		virtual CDBRequest* createDBRequest(const std::string& requestString) const = 0;
		virtual const std::string getEscapeString(const std::string& str) = 0;

		CDBManager();
		virtual ~CDBManager();
	protected:
		CDBConnection* dbConnection;
		bool isConnInit;

		std::string dbName;
		std::string userName;
		std::string pass;
		std::string host;
	private:
};

#endif // CDBMANAGER_H
