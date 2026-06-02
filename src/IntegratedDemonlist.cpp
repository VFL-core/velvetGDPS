#include "IntegratedDemonlist.hpp"
#include <jasmine/web.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

std::vector<IDListDemon> IntegratedDemonlist::aredl;
std::vector<IDDemonPack> IntegratedDemonlist::aredlPacks;
std::vector<IDListDemon> IntegratedDemonlist::pemonlist;
bool IntegratedDemonlist::aredlLoaded = false;
bool IntegratedDemonlist::pemonlistLoaded = false;

void IntegratedDemonlist::loadAREDL(TaskHolder<web::WebResponse>& listener, Function<void()> success, CopyableFunction<void(int)> failure) {
    listener.spawn(
        web::WebRequest().get("https://api.aredl.net/v2/api/aredl/levels"),
        [failure = std::move(failure), success = std::move(success)](web::WebResponse res) mutable {
            if (!res.ok()) return failure(res.code());

            aredlLoaded = true;
            aredl.clear();

            for (auto& level : jasmine::web::getArray(res)) {
                auto legacy = level.get<bool>("legacy");
                if (legacy.isOk() && legacy.unwrap()) continue;

                auto id = level.get<int>("level_id");
                if (!id.isOk()) continue;

                auto position = level.get<int>("position");
                if (!position.isOk()) continue;

                auto name = level.get<std::string>("name");
                if (!name.isOk()) continue;

                IDListDemon demon(id.unwrap(), position.unwrap(), std::move(name).unwrap());

                aredl.insert(std::ranges::upper_bound(aredl, demon, [](const IDListDemon& a, const IDListDemon& b) {
                    return a.position < b.position;
                }), std::move(demon));
            }

            success();
        }
    );
}

void IntegratedDemonlist::loadAREDLPacks(TaskHolder<web::WebResponse>& listener, Function<void()> success, CopyableFunction<void(int)> failure) {
    listener.spawn(
        web::WebRequest().get("https://api.aredl.net/v2/api/aredl/pack-tiers"),
        [failure = std::move(failure), success = std::move(success)](web::WebResponse res) mutable {
            if (!res.ok()) return failure(res.code());

            aredlPacks.clear();

            for (auto& tier : jasmine::web::getArray(res)) {
                auto placement = tier.get<int>("placement");
                if (!placement.isOk()) continue;

                auto tierName = tier.get<std::string>("name");
                if (!tierName.isOk()) continue;

                auto packs = tier.get<std::vector<matjson::Value>>("packs");
                if (!packs.isOk()) continue;

                for (auto& pack : packs.unwrap()) {
                    auto levelsRes = pack.get<std::vector<matjson::Value>>("levels");
                    if (!levelsRes.isOk()) continue;

                    auto name = pack.get<std::string>("name");
                    if (!name.isOk()) continue;

                    auto points = pack.get<double>("points");
                    if (!points.isOk()) continue;

                    std::vector<int> levels;
                    auto packValid = true;
                    for (auto& level : levelsRes.unwrap()) {
                        auto id = level.get<int>("level_id");
                        if (id.isOk()) levels.push_back(id.unwrap());
                        else {
                            packValid = false;
                            break;
                        }
                    }
                    if (!packValid) continue;

                    IDDemonPack demonPack(
                        std::move(name).unwrap(), tierName.unwrap(),
                        std::move(levels), points.unwrap(), placement.unwrap()
                    );

                    aredlPacks.insert(std::ranges::upper_bound(aredlPacks, demonPack, [](const IDDemonPack& a, const IDDemonPack& b) {
                        return a.tier == b.tier ? a.points == b.points ? a.name < b.name : a.points < b.points : a.tier < b.tier;
                    }), std::move(demonPack));
                }
            }

            success();
        }
    );
}

void IntegratedDemonlist::loadPemonlist(TaskHolder<web::WebResponse>& listener, Function<void()> success, CopyableFunction<void(int)> failure) {
    // Dynamically retrieve your raw GitHub endpoint from the mod.json settings block
    std::string targetUrl = Mod::get()->getSettingValue<std::string>("list-api-url");

    listener.spawn(
        web::WebRequest().get(targetUrl),
        [failure = std::move(failure), success = std::move(success)](web::WebResponse res) mutable {
            if (!res.ok()) return failure(res.code());

            pemonlistLoaded = true;
            pemonlist.clear();

            // Navigates directly inside the "list" array of your custom JSON structure
            for (auto& level : jasmine::web::getArray(res, "list")) {
                auto id = level.get<int>("level_id");
                if (!id.isOk()) continue;

                // Grabs the "rank" key from your file to determine placement position
                auto position = level.get<int>("rank");
                if (!position.isOk()) continue;

                auto name = level.get<std::string>("name");
                if (!name.isOk()) continue;

                IDListDemon demon(id.unwrap(), position.unwrap(), std::move(name).unwrap());

                pemonlist.insert(std::ranges::upper_bound(pemonlist, demon, [](const IDListDemon& a, const IDListDemon& b) {
                    return a.position < b.position;
                }), std::move(demon));
            }

            success();
        }
    );
}
