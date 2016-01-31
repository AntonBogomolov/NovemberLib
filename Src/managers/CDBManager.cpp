#include "../../Include/novemberlib/managers/CDBManager.h"

#include "../../Include/novemberlib/managers/CManagers.h"
#include "../../Include/novemberlib/utils/utils.h"
#include "../../Include/novemberlib/utils/CLog.h"

CDBManager::CDBManager()
{
	dbConnection = NULL;
}

CDBManager::~CDBManager()
{
	delete dbConnection;
}

void CDBManager::reconnect()
{
	if(!getIsConnInit())
	{
		closeDBConnection();
		initDB(dbName, userName, pass, host);
	}
}

bool CDBManager::getIsConnInit() const
{
    return isConnInit;
}

const CDBConnection* CDBManager::getDBConnection() const
{
	return dbConnection;
}

/////////////////////////////////////////////////////////////////////////////////////

CDBRequest::CDBRequest(const CDBConnection* dbConnection)
{
	this->dbConnection = dbConnection;
	isLastQuerySuccess = false;
	res = NULL;
}

CDBRequest::~CDBRequest()
{
	delete res;
}

bool CDBRequest::getIsLastQuerySuccess() const
{
	return isLastQuerySuccess;
}

CDBRequestResult* CDBRequest::getLastResult() const
{
	return res;
}

const CDBConnection* CDBRequest::getDBConnection() const
{
	return dbConnection;
}

void CDBRequest::freeLastResult()
{
	delete res;
	res = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////

CDBValue::CDBValue()
{
	fieldName = "*";
	value="";
	isTextValue=true;
}

CDBValue::CDBValue(const std::string& fieldName, const std::string& value, bool isTextValue)
{
	this->fieldName = CManagers::getInstance()->getDBManager()->getEscapeString(fieldName);
	if((int)value.size() == 0)
	{
		this->value = "";
	}
	else
	{
		this->value = CManagers::getInstance()->getDBManager()->getEscapeString(value);
	}
	this->isTextValue = isTextValue;
}

CDBValue::~CDBValue()
{

}

const std::string CDBValue::getFieldName() const
{
	return fieldName;
}

const std::string CDBValue::getValue() const
{
	return value;
}

bool CDBValue::getIsTextValue() const
{
	return isTextValue;
}

/////////////////////////////////////////////////////////////////////////////////////

CDBValues::CDBValues()
{

}

CDBValues::CDBValues(const std::string& fieldNames)
{
	if((int)fieldNames.size() > 1)
	{
		std::vector<std::string> names;
		split(names, fieldNames, ",");
		for(unsigned int i = 0; i < names.size(); ++i)
		{
			addValue(names[i], "");
		}
	}
	else
	{
		addValue(fieldNames, "");
	}
}

CDBValues::~CDBValues()
{
	for(auto it = values.begin(); it != values.end(); ++it)
	{
		delete (*it);
	}
}

void CDBValues::addValue(const std::string& fieldName, const std::string& value,  const bool isNeedToEscape)
{
	values.push_back(new CDBValue(fieldName, value, isNeedToEscape));
}

void CDBValues::addValue(const std::string& fieldName, const int value)
{
	values.push_back(new CDBValue(fieldName, valueToString(value), false));
}

void CDBValues::addValue(const std::string& fieldName, const long value)
{
	values.push_back(new CDBValue(fieldName, valueToString(value), false));
}

void CDBValues::addValue(const CDBValue& value)
{
	CDBValue* dbValue = new CDBValue(value.getFieldName(), value.getValue(), value.getIsTextValue());
	values.push_back(dbValue);
}

void CDBValues::addValue(const CDBValue* value)
{
	CDBValue* dbValue = new CDBValue(value->getFieldName(), value->getValue(), value->getIsTextValue());
	values.push_back(dbValue);
}

const std::vector<const CDBValue*>* CDBValues::getValues() const
{
	return &values;
}
