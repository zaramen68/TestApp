// TestApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "libs/sqlite3/sqlite3.h"
#include "DbConnection.h"

int main()
{
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
