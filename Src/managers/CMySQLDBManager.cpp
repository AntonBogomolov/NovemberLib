#include "../../Include/novemberlib/managers/CMySQLDBManager.h"

#include "../../Include/novemberlib/utils/utils.h"
#include "../../Include/novemberlib/utils/CLog.h"
#include "../../Include/novemberlib/managers/CManagers.h"

CMySQLDBManager::CMySQLDBManager()
{
	isConnInit = false;
	MYSQLConnection = new MYSQL();
	dbConnection = new CMySQLDBConnection(MYSQLConnection);
}

CMySQLDBManager::~CMySQLDBManager()
{
	delete dbConnection;
}

void CMySQLDBManager::initDB(const std::string& dbName, const std::string& userName, const std::string& pass, const std::string& host)
{
	if(!getIsConnInit())
	{
		CLog* log = CLog::getInstance();
		if(!mysql_init(MYSQLConnection))
		{
			log->addError("Error: can't create MySQL-descriptor");
			return;
		}

		if(!mysql_real_connect(MYSQLConnection, host.c_str(), userName.c_str(), pass.c_str(), dbName.c_str(), 0, NULL, 0))
		{
			log->addError("Error: can't connect to MySQL server");
			return;
		}

		this->userName = userName;
		this->pass = pass;
		this->dbName = dbName;
		this->host = host;

		isConnInit = true;

		mysql_query(MYSQLConnection, "SET NAMES utf8");
	}
}

void CMySQLDBManager::closeDBConnection()
{
	if(getIsConnInit())
    {
        mysql_close(MYSQLConnection);
        isConnInit = false;
    }
}

CDBRequest* CMySQLDBManager::createDBRequest(const std::string& requestString) const
{
	if(getIsConnInit())
	{
		CMySQLDBConnection* mySQLConn = dynamic_cast<CMySQLDBConnection*>(dbConnection);

		CDBRequest* request = new CMySQLDBRequest(mySQLConn);
		request->createRequest(requestString);
		return request;
	}
	else return NULL;
}

CDBRequest* CMySQLDBManager::createDBRequest() const
{
	if(getIsConnInit())
	{
		CMySQLDBConnection* mySQLConn = dynamic_cast<CMySQLDBConnection*>(dbConnection);
		CDBRequest* request = new CMySQLDBRequest(mySQLConn);
		return request;
	}
	else return NULL;
}

const std::string CMySQLDBManager::getEscapeString(const std::string& str)
{
	std::string retStr = "";
	//if(str.length() > 100000) return retStr;

	char* tmpStr = new char[str.length() * 2];
	int len = mysql_real_escape_string(MYSQLConnection, tmpStr, str.c_str(), str.length());

	tmpStr[len] = '\0';
	retStr = tmpStr;
	delete [] tmpStr;

	return retStr;
}

/////////////////////////////////////////////////////////////////////////////////////

CMySQLDBRequestResult::CMySQLDBRequestResult(MYSQL_RES* result)
{
	sqlResult = NULL;
	if(result)
	{
		sqlResult = result;
		rowsCnt   = (int)mysql_num_rows(sqlResult);
		fieldsCnt = mysql_num_fields(sqlResult);

		for(unsigned int i = 0; i < sqlResult->row_count; ++i)
		{
			sqlRows.push_back(mysql_fetch_row(sqlResult));
		}
	}
}

CMySQLDBRequestResult::~CMySQLDBRequestResult()
{
	if(sqlResult != NULL) mysql_free_result(sqlResult);
	sqlResult = NULL;
}

int CMySQLDBRequestResult::getFiledsCnt() const
{
	return fieldsCnt;
}

int CMySQLDBRequestResult::getRowsCnt() const
{
	return rowsCnt;
}

const std::string CMySQLDBRequestResult::getStringValue(const int rowIndex, const int fieldIndex, const std::string& defValue) const
{
	if(rowIndex   >= getRowsCnt()) 	 return defValue;
	if(fieldIndex >= getFiledsCnt()) return defValue;

	std::string value = defValue;
	try
	{
		if(sqlRows[rowIndex][fieldIndex]) value = sqlRows[rowIndex][fieldIndex];
	}
	catch(...)
	{
		value = defValue;
		CLog::getInstance()->addError("getStringValue exception, row="+valueToString(rowIndex)+" col="+valueToString(fieldIndex));
	}
	return value;
}

int  CMySQLDBRequestResult::getIntValue(const int rowIndex, const int fieldIndex, const int defValue) const
{
	if(rowIndex   >= getRowsCnt()) 	 return defValue;
	if(fieldIndex >= getFiledsCnt()) return defValue;

	int value = defValue;
	try
	{
		if(sqlRows[rowIndex][fieldIndex]) value = std::stoi(sqlRows[rowIndex][fieldIndex]);
	}
	catch(...)
	{
		value = defValue;
		CLog::getInstance()->addError("getIntValue exception, row="+valueToString(rowIndex)+" col="+valueToString(fieldIndex));
	}
	return value;
}

long CMySQLDBRequestResult::getLongValue(const int rowIndex, const int fieldIndex, const long defValue) const
{
	if(rowIndex   >= getRowsCnt()) 	 return defValue;
	if(fieldIndex >= getFiledsCnt()) return defValue;

	long value = defValue;
	try
	{
		if(sqlRows[rowIndex][fieldIndex]) value = std::stol(sqlRows[rowIndex][fieldIndex]);
	}
	catch(...)
	{
		value = defValue;
		CLog::getInstance()->addError("getLongValue exception, row="+valueToString(rowIndex)+" col="+valueToString(fieldIndex));
	}
	return value;
}

bool CMySQLDBRequestResult::getBoolValue(const int rowIndex, const int fieldIndex, const bool defValue) const
{
	if(rowIndex   >= getRowsCnt()) 	 return defValue;
	if(fieldIndex >= getFiledsCnt()) return defValue;

	bool value = defValue;
	try
	{
		if(sqlRows[rowIndex][fieldIndex]) value = std::stoi(sqlRows[rowIndex][fieldIndex]) == 1;
	}
	catch(...)
	{
		value = defValue;
		CLog::getInstance()->addError("getBoolValue exception, row="+valueToString(rowIndex)+" col="+valueToString(fieldIndex));
	}
	return value;
}

/////////////////////////////////////////////////////////////////////////////////////

CMySQLDBRequest::CMySQLDBRequest(const CMySQLDBConnection* dbConnection) : CDBRequest(dbConnection)
{

}

CMySQLDBRequest::~CMySQLDBRequest()
{

}

const CDBRequestResult* CMySQLDBRequest::createRequest(const std::string& requestString)
{
	CLog* log = CLog::getInstance();

	freeLastResult();
	isLastQuerySuccess = false;
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	if(!dbManager || !dbManager->getIsConnInit() || !dbConnection) return NULL;
	const CMySQLDBConnection* conn = dynamic_cast<const CMySQLDBConnection*>(dbConnection);

	CSyncHelper::getInstance()->getDBMutex()->lock();
	if(mysql_query(conn->getMySQLConnection(), requestString.c_str()) != 0)
	{
		std::string errorText = "Error: can't execute SQL-query: " + requestString + "\n";
		log->addError(errorText);
		isLastQuerySuccess = false;

		CSyncHelper::getInstance()->getDBMutex()->unlock();
		return NULL;
	}
	res = new CMySQLDBRequestResult(mysql_store_result(conn->getMySQLConnection()));
	CSyncHelper::getInstance()->getDBMutex()->unlock();

	log->addInfo("SQL-QUERY: " + requestString);
	isLastQuerySuccess = true;

	return res;
}

const CDBRequestResult* CMySQLDBRequest::selectRequest(const CDBValues& fields, const std::string& from, const std::string& where, const std::string& options)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	if(!dbManager || !dbManager->getIsConnInit() || !dbConnection) return NULL;
	std::string requestStr = "SELECT ";

	const std::vector<const CDBValue*>* fieldsNames = fields.getValues();
	if((int)fieldsNames->size() == 0 || (*(fieldsNames->begin()))->getFieldName() == "*")
	{
		requestStr += '*';
	}
	else
	{
		for(auto it = fieldsNames->begin(); it != fieldsNames->end(); ++it)
		{
			const CDBValue* currField = (*it);

			if(it != fieldsNames->begin()) requestStr += ',';
			requestStr += '`';
			requestStr += currField->getFieldName();
			requestStr += '`';
		}
	}
	requestStr += ' ';

	requestStr += "FROM ";
	requestStr += '`';
	requestStr += from;
	requestStr += '`';
	requestStr += ' ';

	if((int)where.size() > 0)
	{
		requestStr += "WHERE ";
		requestStr += where;
		requestStr += ' ';
	}

	requestStr += options;
	requestStr += ';';

	return createRequest(requestStr);
}

const CDBRequestResult* CMySQLDBRequest::deleteRequest(const std::string& from, const std::string& where, const std::string& options)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	if(!dbManager || !dbManager->getIsConnInit() || !dbConnection) return NULL;
	std::string requestStr = "DELETE ";

	requestStr += "FROM ";
	requestStr += '`';
	requestStr += from;
	requestStr += '`';
	requestStr += ' ';

	if((int)where.size() > 0)
	{
		requestStr += "WHERE ";
		requestStr += where;
		requestStr += ' ';
	}

	requestStr += options;
	requestStr += ';';

	return createRequest(requestStr);
}

const CDBRequestResult* CMySQLDBRequest::insertRequest(const CDBValues& fieldsAndValues, const std::string& into, const std::string& options)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	if(!dbManager || !dbManager->getIsConnInit() || !dbConnection) return NULL;
	std::string requestStr = "INSERT ";

	requestStr += "INTO ";
	requestStr += '`';
	requestStr += into;
	requestStr += '`';
	requestStr += ' ';

	const std::vector<const CDBValue*>* fields = fieldsAndValues.getValues();
	if((int)fields->size() == 0) return NULL;

	requestStr += '(';
	for(auto it = fields->begin(); it != fields->end(); ++it)
	{
		const CDBValue* currField = (*it);

		if(it != fields->begin()) requestStr += ',';
		requestStr += '`';
		requestStr += currField->getFieldName();
		requestStr += '`';
	}
	requestStr += ')';
	requestStr += ' ';

	requestStr += "VALUES ";
	requestStr += '(';
	for(auto it = fields->begin(); it != fields->end(); ++it)
	{
		const CDBValue* currField = (*it);

		if(it != fields->begin()) requestStr += ',';
		if(currField->getIsTextValue())requestStr += '\'';
		requestStr += currField->getValue();
		if(currField->getIsTextValue())requestStr += '\'';
	}
	requestStr += ')';
	requestStr += ' ';

	requestStr += options;
	requestStr += ';';

	return createRequest(requestStr);
}

const CDBRequestResult* CMySQLDBRequest::updateRequest(const std::string& from, const std::string& values , const std::string& where, const std::string& options)
{
	CDBManager* dbManager = CManagers::getInstance()->getDBManager();
	if(!dbManager || !dbManager->getIsConnInit() || !dbConnection) return NULL;
	std::string requestStr = "UPDATE ";

	requestStr += '`';
	requestStr += from;
	requestStr += '`';
	requestStr += ' ';

	requestStr += "SET ";
	requestStr += values;
	requestStr += ' ';

	if((int)where.size() > 0)
	{
		requestStr += "WHERE ";
		requestStr += where;
		requestStr += ' ';
	}

	requestStr += options;
	requestStr += ';';

	return createRequest(requestStr);
}
