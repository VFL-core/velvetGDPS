#pragma once

#include <Geode/utils/web.hpp>
#include <string>
#include <vector>

struct IDListDemon {
    int id = 0;
    int position = 0;
    std::string name;

    // Added constructor to match your .cpp initialization loop
    IDListDemon(int p_id, int p_position, std::string p_name) 
        : id(p_id), position(p_position), name(std::move(p_name)) {}

    bool operator==(const IDListDemon& other) const {
        return id == other.id && position == other.position;
    }
};

struct IDDemonPack {
    std::string name;
    std::string tierName;
    std::vector<int> levels;
    double points = 0.0;
    int tier = 0;

    // Added constructor to match your AREDL pack parsing logic
    IDDemonPack(std::string p_name, std::string p_tierName, std::vector<int> p_levels, double p_points, int p_tier)
        : name(std::move(p_name)), tierName(std::move(p_tierName)), levels(std::move(p_levels)), points(p_points), tier(p_tier) {}
};

namespace IntegratedDemonlist {
    extern std::vector<IDListDemon> aredl;
    extern std::vector<IDDemonPack> aredlPacks;
    extern std::vector<IDListDemon> pemonlist; // Keeps tracking data for your Velvet list
    extern bool aredlLoaded;
    extern bool pemonlistLoaded;

    void loadAREDL(geode::async::TaskHolder<geode::utils::web::WebResponse>&, geode::Function<void()>, geode::CopyableFunction<void(int)>);
    void loadAREDLPacks(geode::async::TaskHolder<geode::utils::web::WebResponse>&, geode::Function<void()>, geode::CopyableFunction<void(int)>);
    void loadPemonlist(geode::async::TaskHolder<geode::utils::web::WebResponse>&, geode::Function<void()>, geode::CopyableFunction<void(int)>);
}
