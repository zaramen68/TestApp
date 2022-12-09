#pragma once

#include <string>
#include <vector>

struct Entity {
    std::string Guid;
    std::string Id;
    bool IsFull;
    std::string Name;
    std::string ParentId;
    unsigned int RowVer;
    std::string loadStamp;
};

struct EList {
    std::vector<Entity> e_line;
};

struct SPEList {
    std::vector<std::shared_ptr<Entity>> e_line;
};

void make_entities_list(const char* data_file_name, SPEList& entities);