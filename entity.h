#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "EntityStruct.h"

using json = nlohmann::json;

// Тут конечно много лишнего. Как функции, так и операторы.  Для работы достаточно чего-то одного
//

void operator >> (const json& js, Entity& entity) {
    js.at("Guid").get_to(entity.Guid);
    js.at("Id").get_to(entity.Id);
    js.at("IsFull").get_to(entity.IsFull);
    js.at("Name").get_to(entity.Name);
    js.at("ParentId").get_to(entity.ParentId);
    js.at("RowVer").get_to(entity.RowVer);
    js.at("loadStamp").get_to(entity.loadStamp);
}

void operator >> (const json& js, EList& e_list) {
    for (auto item : js) {
        Entity new_entity;
        item >> new_entity;
        e_list.e_line.push_back(new_entity);
    }
}


void json_to_entity(const json& js, std::shared_ptr<Entity> entity) {
    js.at("Guid").get_to(entity->Guid);
    js.at("Id").get_to(entity->Id);
    js.at("IsFull").get_to(entity->IsFull);
    js.at("Name").get_to(entity->Name);
    js.at("ParentId").get_to(entity->ParentId);
    js.at("RowVer").get_to(entity->RowVer);
    js.at("loadStamp").get_to(entity->loadStamp);
}

void operator >> (const json& js, std::shared_ptr<Entity> entity) {
    js.at("Guid").get_to(entity->Guid);
    js.at("Id").get_to(entity->Id);
    js.at("IsFull").get_to(entity->IsFull);
    js.at("Name").get_to(entity->Name);
    js.at("ParentId").get_to(entity->ParentId);
    js.at("RowVer").get_to(entity->RowVer);
    js.at("loadStamp").get_to(entity->loadStamp);
}

void operator >> (const json& js, SPEList& e_list) {
    for (auto item : js) {
        std::shared_ptr<Entity> new_entity(new Entity{});
        // json_to_entity(item, new_entity);
        item >> new_entity;
        e_list.e_line.push_back(new_entity);
    }
}

void json_to_struct(const json& js, Entity& entity) {
    js.at("Guid").get_to(entity.Guid);
    js.at("Id").get_to(entity.Id);
    js.at("IsFull").get_to(entity.IsFull);
    js.at("Name").get_to(entity.Name);
    js.at("ParentId").get_to(entity.ParentId);
    js.at("RowVer").get_to(entity.RowVer);
    js.at("loadStamp").get_to(entity.loadStamp);
}
