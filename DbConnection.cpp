#include "DbConnection.h"
#include <chrono>
#include <thread>
#include "Db.h"


std::string DbConnection::_defaultFileName;

sqlite3* DbConnection::OpenDB(const char* DbFile)
{
	const int MaxTryCount = 10;
	const int SleepTime = 2000;
	int TryCount = 0;
	int result = -1;
	sqlite3* _db = nullptr;
	do
	{
		result = sqlite3_open_v2(DbFile, &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_SHAREDCACHE, NULL);
		if (result != SQLITE_OK)
		{
			const char* errMsg = sqlite3_errstr(result);
			if (errMsg == 0)
			{
				errMsg = "Не удалось подключиться к БД";
			}

			if ((result == SQLITE_BUSY) || (result == SQLITE_LOCKED))
			{
				if (TryCount > MaxTryCount)
					if (OnErrorHandler != nullptr)
						OnErrorHandler(errMsg, m_isThrowEx, "sqlite3_open_v2");
					else
						if (m_isThrowEx)
							throw std::exception(errMsg);
						else
							break;
					
				TryCount++;
				std::this_thread::sleep_for(std::chrono::milliseconds(SleepTime));
				continue;
			}

			if (OnErrorHandler != nullptr)
				OnErrorHandler(errMsg, m_isThrowEx, "sqlite3_open_v2");
			else
				if (m_isThrowEx)
					throw std::exception(errMsg);
				else
					break;
			return nullptr;
		}
	} while (result != SQLITE_OK);


	Db::SetTimeout(_db);

	//set journal mode
	Db::ExecuteQuery("PRAGMA journal_mode = WAL;", _db);

	//set to store temp db and indexes store in memory
	Db::ExecuteQuery("PRAGMA temp_store = 2;", _db);

	return _db;
}

sqlite3* DbConnection::Open()
{
    if (_db != 0)
        return _db;

    const char* fileName;


    fileName = _defaultFileName.c_str();

	_db = OpenDB(fileName);
	return _db;
}

void DbConnection::Close()
{
    if (_db != 0) {
        sqlite3_close(_db);
        _db = 0;
    }
}

void DbConnection::ExecuteQuery(const char* sql)
{
	Db::ExecuteQuery(sql, _db, true);
}
