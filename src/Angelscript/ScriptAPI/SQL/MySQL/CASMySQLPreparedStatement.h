#ifndef ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLPREPAREDSTATEMENT_H
#define ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLPREPAREDSTATEMENT_H

#include <atomic>
#include <cstdint>
#include <string>
#include <vector>

#include <mysql.h>

#include <Angelscript/util/CASBaseClass.h>

#include "../IASSQLASyncCommand.h"

#include "CASMySQLBind.h"

class asIScriptFunction;

class CASDateTime;
class CASTime;
class CASMySQLConnection;

class CASMySQLPreparedStatement final : public IASSQLASyncCommand, public CASAtomicRefCountedBaseClass
{
protected:
	friend class CASMySQLResultSet;

public:
	CASMySQLPreparedStatement( CASMySQLConnection* pConnection, const char* const pszStatement );
	~CASMySQLPreparedStatement();

	void AddRef() const override final
	{
		CASAtomicRefCountedBaseClass::AddRef();
	}

	void Release() const override final
	{
		if( InternalRelease() )
			delete this;
	}

	void Execute() override;

	/**
	*	@return Whether the statement is valid.
	*/
	bool IsValid() const;

	/**
	*	@return The current parameter count. May not match the query's parameter count.
	*/
	int GetParamCount() const;

	/**
	*	Binds a null parameter.
	*	@param iIndex Parameter index.
	*/
	void BindNull( uint32_t uiIndex );

	/**
	*	Binds a boolean.
	*	@param iIndex Parameter index.
	*	@param bValue Value.
	*/
	void BindBoolean( uint32_t uiIndex, bool bValue );

	/**
	*	Binds an 8 bit signed integer.
	*	@param iIndex Parameter index.
	*	@param iValue Value.
	*/
	void BindSigned8( uint32_t uiIndex, int8_t iValue );

	/**
	*	Binds an 8 bit unsigned integer.
	*	@param iIndex Parameter index.
	*	@param uiValue Value.
	*/
	void BindUnsigned8( uint32_t uiIndex, uint8_t uiValue );

	/**
	*	Binds a 16 bit signed integer.
	*	@param iIndex Parameter index.
	*	@param iValue Value.
	*/
	void BindSigned16( uint32_t uiIndex, int16_t iValue );

	/**
	*	Binds a 16 bit unsigned integer.
	*	@param iIndex Parameter index.
	*	@param uiValue Value.
	*/
	void BindUnsigned16( uint32_t uiIndex, uint16_t uiValue );

	/**
	*	Binds a 32 bit signed integer.
	*	@param iIndex Parameter index.
	*	@param iValue Value.
	*/
	void BindSigned32( uint32_t uiIndex, int32_t iValue );

	/**
	*	Binds a 32 bit unsigned integer.
	*	@param iIndex Parameter index.
	*	@param uiValue Value.
	*/
	void BindUnsigned32( uint32_t uiIndex, uint32_t uiValue );

	/**
	*	Binds a 64 bit signed integer.
	*	@param iIndex Parameter index.
	*	@param iValue Value.
	*/
	void BindSigned64( uint32_t uiIndex, int64_t iValue );

	/**
	*	Binds a 64 bit unsigned integer.
	*	@param iIndex Parameter index.
	*	@param uiValue Value.
	*/
	void BindUnsigned64( uint32_t uiIndex, uint64_t uiValue );

	/**
	*	Binds a 32 bit float.
	*	@param iIndex Parameter index.
	*	@param flValue Value.
	*/
	void BindFloat( uint32_t uiIndex, float flValue );

	/**
	*	Binds a 64 bit float.
	*	@param iIndex Parameter index.
	*	@param flValue Value.
	*/
	void BindDouble( uint32_t uiIndex, double flValue );

	/**
	*	Binds a string.
	*	@param iIndex Parameter index.
	*	@param szString String.
	*/
	void BindString( uint32_t uiIndex, const std::string& szString );

	/**
	*	Binds a date instance.
	*	@param iIndex Parameter index.
	*	@param date Date instance.
	*/
	void BindDate( uint32_t uiIndex, const CASDateTime& date );

	/**
	*	Binds a time instance.
	*	@param iIndex Parameter index.
	*	@param time Time instance.
	*/
	void BindTime( uint32_t uiIndex, const CASTime& time );

	/**
	*	Binds a date time instance.
	*	@param iIndex Parameter index.
	*	@param dateTime Date time instance.
	*/
	void BindDateTime( uint32_t uiIndex, const CASDateTime& dateTime );

	bool ExecuteStatement( asIScriptFunction* pResultSetCallback = nullptr, asIScriptFunction* pCallback = nullptr );

	CASMySQLConnection* GetConnection() { return m_pConnection; }

private:
	/**
	*	Checks if the bind list size is large enough for the given index. Resizes it if needed.
	*	@return Whether the list could be resized.
	*/
	bool CheckBindListSize( uint32_t uiIndex );

private:
	CASMySQLConnection* m_pConnection = nullptr;

	std::string m_szStatement;

	std::vector<MYSQL_BIND> m_Binds;
	//Declare these after binds so they destruct first. They might access the binds - Solokiller
	std::vector<CASMySQLBind> m_Variables;

	asIScriptFunction* m_pCallback = nullptr;

	std::atomic<bool> m_bHandledResultSet = false;

private:
	CASMySQLPreparedStatement( const CASMySQLPreparedStatement& ) = delete;
	CASMySQLPreparedStatement& operator=( const CASMySQLPreparedStatement& ) = delete;
};

#endif //ANGELSCRIPT_SCRIPTAPI_SQL_MYSQL_CASMYSQLPREPAREDSTATEMENT_H