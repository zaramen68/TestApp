#pragma once

#include <string>
#include <vector>

// я выбрас структуру, не класс.
// позволил себе некоторую вольность с типами.
// Ќе совсем пон€тно, почему в json Id хран€тьс€, как строки. Ёто ведь, как правило числа.
// ѕоэтому, € позволил себе сделать их в структуре и Ѕƒ числами.
//  роме того ParentId €вно намекает на некую неоднородность и св€зность данных, поэтому € позволил себе 
// немного усложнить Ѕƒ и сделать две таблицы: дл€ корневы вещей и их потомков.

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