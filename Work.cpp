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

    EList good_list;
    j_son >> good_list;

    SPEList sp_glist;
    j_son >> sp_glist;
    j_son >> entities;

    for (auto item : j_son) {
        // std::cout << item["Id"] << std::endl;
        if (!item.is_object()) { continue; }
        std::string  id_str;
        int id;

        switch (item.at("Id").type())
        {
        case nlohmann::detail::value_t::string:
            item.at("Id").get_to(id_str);
            id = atoi(id_str.c_str());
            break;
        case nlohmann::detail::value_t::number_integer:
            item.at("Id").get_to(id);
        default:
            break;
        }



        for (json::iterator it = item.begin(); it != item.end(); ++it) {
            std::cout << it.key() << " : " << it.value() << std::endl;
        }
 
    }


}