#ifndef CMYSQLDBMANAGER_H
#define CMYSQLDBMANAGER_H

#include "CDBManager.h"

#include <mysql/mysql.h>

class CMySQLDBConnection : public CDBConnection
{
	public:
		MYSQL* getMySQLConnection() const {return dbConnection;};

		CMySQLDBConnection(MYSQL* mySQLConnection){dbConnection = mySQLConnection;};
		virtual ~CMySQLDBConnection(){delete dbConnection;};
	protected:
		MYSQL* dbConnection;
};

class CMySQLDBRequestResult : public CDBRequestResult
{
	public:
		virtual int getFiledsCnt() const;
		virtual int getRowsCnt() const;

		virtual const std::string getStringValue(const int rowIndex, const int fieldIndex, const std::string& defValue = "") const;
		virtual int  getIntValue(const int rowIndex, const int fieldIndex,  const int defValue = 0) const;
		virtual long getLongValue(const int rowIndex, const int fieldIndex, const long defValue = 0) const;
		virtual bool getBoolValue(const int rowIndex, const int fieldIndex, const bool defValue = false) const;

		CMySQLDBRequestResult(MYSQL_RES* result);
		virtual ~CMySQLDBRequestResult();
	protected:
		std::vector<MYSQL_ROW> sqlRows;
		MYSQL_RES* sqlResult;
};

class CMySQLDBRequest : public CDBRequest
{
public:
    virtual const CDBRequestResult* createRequest(const std::string& requestString);
    virtual const CDBRequestResult* selectRequest(const CDBValues& fields, const std::string& from, const std::string& where = "", const std::string& options = "");
	virtual const CDBRequestResult* deleteRequest(const std::string& from, const std::string& where = "", const std::string& options = "");
	virtual const CDBRequestResult* insertRequest(const CDBValues& fieldsAndValues, const std::string& into, const std::string& options = "");
	virtual const CDBRequestResult* updateRequest(const std::string& from, const std::string& values, const std::string& where = "", const std::string& options = "");

    CMySQLDBRequest(const CMySQLDBConnection* dbConnection);
    virtual ~CMySQLDBRequest();
};

class CMySQLDBManager : public CDBManager
{
	public:
		virtual void initDB(const std::string& dbName, const std::string& userName, const std::string& pass, const std::string& host);
		virtual void closeDBConnection();

		virtual CDBRequest* createDBRequest() const;
		virtual CDBRequest* createDBRequest(const std::string& requestString) const;

		virtual const std::string getEscapeString(const std::string& str);

		CMySQLDBManager();
		virtual ~CMySQLDBManager();
	private:
		MYSQL* MYSQLConnection;
};

#endif // CMYSQLDBMANAGER_H
