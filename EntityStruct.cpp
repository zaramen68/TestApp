#include <iostream>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "entity.h"


void make_entities_list(const char* data_file_name, SPEList& entities)
{
    std::ifstream data_file(data_file_name);
    if (!data_file.is_open()) {
        std::cerr << "Unable to open file\n";
        throw 1;
    }

    json j_son;
    data_file >> j_son;
    data_file.close();

    if (!j_son.is_array()) { return; }

    j_son >> entities;

}