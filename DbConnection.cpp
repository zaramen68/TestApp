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
				errMsg = "�� ������� ������������ � ��";
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

void DbConnection::ExecuteQueryBind(const char* table_name, std::shared_ptr<Entity> entity){

	sqlite3_stmt *stmt;
	std::string sql_req = "INSERT INTO " + std::string(table_name) +
		" (guid, id, is_full, name, parent_id, rewver, load_stamp) VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7);";

	sqlite3_prepare_v2(_db, sql_req.c_str(), -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, entity->Guid.c_str(), entity->Guid.length(), SQLITE_STATIC);
	sqlite3_bind_int(stmt, 2, atoi(entity->Id.c_str()));
	sqlite3_bind_int(stmt, 3, static_cast<int>(entity->IsFull));
	sqlite3_bind_text(stmt, 4, entity->Name.c_str(), entity->Name.length(), SQLITE_STATIC);
	sqlite3_bind_int(stmt, 5, atoi(entity->ParentId.c_str()));
	sqlite3_bind_int(stmt, 6, entity->RowVer);
	sqlite3_bind_text(stmt, 7, entity->loadStamp.c_str(), entity->loadStamp.length(), SQLITE_STATIC);



	int rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		printf("ERROR inserting data: %s\n", sqlite3_errmsg(_db));
		return;
	}

	sqlite3_finalize(stmt);
}
