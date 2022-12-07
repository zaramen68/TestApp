#pragma once

#include "libs/sqlite3/sqlite3.h"
#include <string>
#include <functional>

class DbConnection {
private:
    static std::string _defaultFileName;
    sqlite3* _db = nullptr;
	bool m_isCloseOnDestory = true;
	bool m_isThrowEx = false;

    DbConnection(const DbConnection& conn);

	sqlite3* OpenDB(const char* DbFile);
public:
    static void SetDefaultFileName(const char* fileName)
    {
        _defaultFileName = fileName == 0 ? "" : fileName;
    }

	static std::string GetDBFileName()
	{
		return _defaultFileName;
	}

	bool IsCloseOnDestory() const { return m_isCloseOnDestory; }
	void IsCloseOnDestory(bool val) { m_isCloseOnDestory = val; }

	bool IsThrowEx() const { return m_isThrowEx; }
	void IsThrowEx(bool val) { m_isThrowEx = val; }

	std::function<void(const std::string& msg, bool throwEx, const std::string sql)> OnErrorHandler = nullptr;

    DbConnection()
    {
    }

    operator sqlite3*() const { return _db; }

    sqlite3* Open();
    void Close();

	void ExecuteQuery(const char* sql);
	~DbConnection()
    {
		if (m_isCloseOnDestory)
			Close();
    }
};
