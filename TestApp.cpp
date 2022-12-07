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
����� ������� �� ����� data.json ������������� ������ � ��
�������� ����������:
* ��� ������ ������ ������� ������ ������ (struct, class) � ������������ ��������� ��� ����� ���������
* ��������������� ������ � �������
* �������� � �� �� ��������

����������:
��� �������������� ������������� ������������ ����������, ����� �� �����
rapidjson - https://github.com/Tencent/rapidjson/
json.hpp - https://github.com/nlohmann/json

��� ���������� Visual studio 2019-2022 community edition ����� ����������

*/
