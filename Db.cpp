#include "Db.h"
#include <exception>
#include "DbConnection.h"
#include <iostream>
#include <thread>
#include <Windows.h>

const int MaxTryCount = 10;
const DWORD TrySleepTime = 2 * 1000;

namespace Db
{
	bool SetTimeout(sqlite3* db, int timeoutSec, bool throwEx)
	{
		if (db == 0)
			return true;

		int result = sqlite3_busy_timeout(db, timeoutSec * 1000);
		if (result != SQLITE_OK) {
			const char* errMsg = sqlite3_errstr(result);
			if (errMsg == 0)
				errMsg = "Не удалось установить таймаут выполнения запроса";

			throw std::exception(errMsg);
			return false;
		}

		return true;
	}

	void HandleError(const std::string& msg, bool throwEx, const std::string sql)
	{
		if (sql.empty())
			std::cout << msg;
		else
			std::cout << "error=" << msg << ", sql = " << sql;

		if (throwEx)
			throw std::exception(msg.c_str());
	}

	sqlite3* OpenConnection(DbConnection* conn, bool throwEx)
	{
		if (conn == 0)
			return 0;

		try {
			return conn->Open();
		}
		catch (const std::exception& ex) {
			HandleError(ex.what(), throwEx, "OpenConnection");
		}

		return 0;
	}

	bool LoadData(
		const char* sql,
		sqlite3* db,
		int (*callback)(void*, int, char**, char**),
		void* arg,
		bool throwEx,
		int timeoutSec)
	{
		DbConnection conn;
		if (db == 0 && !(db = OpenConnection(&conn, throwEx)))
			return false;

		SetTimeout(db, timeoutSec, throwEx);

		int TryCount = 0;
		int result = -1;
		do
		{
			char* errMsg = 0;
			result = sqlite3_exec(
				db,
				sql,
				callback,
				arg,
				&errMsg);

			if (result != SQLITE_OK)
			{
				std::string msg = "Ошибка БД: ";

				if (errMsg != 0) {
					msg.append(errMsg);
					sqlite3_free(errMsg);
				}
				else {
					const char* codeMsg = sqlite3_errstr(result);
					if (codeMsg != 0)
						msg.append(codeMsg);
				}

				if ((result == SQLITE_BUSY) || (result == SQLITE_LOCKED))
				{
					if (TryCount > MaxTryCount)
						HandleError(msg, throwEx, std::string(sql));
					TryCount++;
					std::this_thread::sleep_for(std::chrono::milliseconds(TrySleepTime));
					continue;
				}

				HandleError(msg, throwEx, std::string(sql));

				return false;
			}
		} while (result != SQLITE_OK);

		return true;
	}

	bool ExecuteQuery(const char* sql, sqlite3* db, bool throwEx, int timeoutSec)
	{
		return LoadData(sql, db, 0, 0, throwEx, timeoutSec);
	}

	bool ExecuteQueryA(std::string sql, int timeoutSec /*= 10*/)
	{
		return ExecuteQuery(sql.c_str(), 0, false, timeoutSec);
	}
}