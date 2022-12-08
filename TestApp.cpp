// TestApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "libs/sqlite3/sqlite3.h"
#include "DbConnection.h"
#include "entity.h"

constexpr const char* data_file_name = "data.json";
constexpr const char* sql_create_parent_table = "CREATE TABLE IF NOT EXISTS parent_categories(" 
    "grid TEXT,"
    "id INTEGER,"
    "is_full BOOLEAN NOT NULL CHECK(is_full IN(0, 1)),"
    "name TEXT,"
    "rewver INTEGER,"
    "load_stamp TEXT,"
    "PRIMARY KEY(id, name));";

constexpr const char* sql_create_categories_table = "CREATE TABLE IF NOT EXISTS categories("
    "grid TEXT,"
    "id INTEGER,"
    "is_full BOOLEAN NOT NULL CHECK(is_full IN(0, 1)),"
    "name TEXT,"
    "parent_id INTEGER,"
    "rewver INTEGER,"
    "load_stamp TEXT,"
    "PRIMARY KEY(id, name),"
    "FOREIGN KEY (parent_id)  REFERENCES parent_categories (id);";

int main()
{
    SPEList entities;
    make_entities_list(data_file_name, entities);

    DbConnection::SetDefaultFileName("test.db");
    DbConnection conn;
    if (!conn.Open())
    {
        std::cout << "Test app start error\n";
    }
    std::cout << "Hello from test app!\n";
}

/*
Общее задание из файла data.json импортировать данные в бд
Описание требований:
* для набора данных создать бизнес объект (struct, class) и использовать указатель или умный указатель
* десериализовать данные в объекты
* записать в бд из объектов

Примечания:
Для десериализации рекомендуется использовать библиотеки, любая на выбор
rapidjson - https://github.com/Tencent/rapidjson/
json.hpp - https://github.com/nlohmann/json

Для разработки Visual studio 2019-2022 community edition будет достаточно

*/
