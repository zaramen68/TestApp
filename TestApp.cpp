// TestApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// Пришлось поменять версию решения v143 на v142. У меня VS2019.
//

#include <iostream>
#include "libs/sqlite3/sqlite3.h"
#include "DbConnection.h"
#include "EntityStruct.h"

//
// Как я уже писал в другом файле, структура данных подталкивает на создание двух таблиц.
// Поскольку никаких указаний по архитектуре БД, вроде нет, то я позволил себе эту вольность.
//

constexpr const char* data_file_name = "data.json";
constexpr const char* sql_create_parent_table = "CREATE TABLE IF NOT EXISTS parent_categories("
    "guid TEXT,"
    "id INTEGER,"
    "is_full BOOLEAN NOT NULL CHECK(is_full IN(0, 1)),"
    "name TEXT,"
    "rewver INTEGER,"
    "load_stamp TEXT,"
    "PRIMARY KEY(id, name));";

constexpr const char* sql_create_categories_table = "CREATE TABLE IF NOT EXISTS categories("
    "guid TEXT,"
    "id INTEGER,"
    "is_full BOOLEAN NOT NULL CHECK(is_full IN(0, 1)),"
    "name TEXT,"
    "parent_id INTEGER,"
    "rewver INTEGER,"
    "load_stamp TEXT,"
    "PRIMARY KEY(id, name),"
    "FOREIGN KEY (parent_id)  REFERENCES parent_categories (id));";

int main()
{
    SPEList entities;
    make_entities_list(data_file_name, entities);

    DbConnection::SetDefaultFileName("test.db");
    DbConnection conn;
    if (!conn.Open())
    {
        std::cout << "Test app start error\n";
        return 0;
    }

    conn.ExecuteQuery(sql_create_parent_table);
    conn.ExecuteQuery(sql_create_categories_table);

    for (auto item : entities.e_line){
        //
        // Тут два подхода к запросам в БД: используя ваш функционал и с bind.
        // Используя format строки в С20 веркии можно было бы, наверное, написать красивее.
        // Но у меня C17
        //

        if(item->ParentId != ""){
            conn.ExecuteQueryBind("categories", item);
        } else {
            std::string sql_req ="INSERT INTO parent_categories (guid, id, is_full, name, rewver, load_stamp) VALUES('"+
                item->Guid+"', "+
                item->Id+", "+
                std::to_string(static_cast<int>(item->IsFull))+", '"+
                item->Name+"', "+
                std::to_string(item->RowVer)+", '"+
                item->loadStamp+"');";

            conn.ExecuteQuery(sql_req.c_str());
        }
    }
return 1;

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
