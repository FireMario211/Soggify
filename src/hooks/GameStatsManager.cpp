#include <algorithm>
#include <defs/utils.hpp>
#include <Geode/modify/GameStatsManager.hpp>

bool equal(const char* a, const char* b) {
    if (a == b) return true;             // same pointer or both nullptr -> equal
    if (!a || !b) return false;          // one is null, other not -> not equal
    return strcmp(a, b) == 0;
}


// ty wylie https://wyliemaster.github.io/gddocs/#/resources/client/gamesave/GS_Value?id=gs-value-structure
// 6 = star
// 28 = moons
class $modify(SoggifyHookGSM, GameStatsManager) {
    void incrementStat(const char *key, int amount) {
        GameStatsManager::incrementStat(key, amount);
        if (Mod::get()->getSettingValue<bool>("ery-mode")) return;
        if (Mod::get()->hasSavedValue("quests")) {
            auto quests = Mod::get()->getSavedValue<std::vector<int>>("quests");
            auto allQuests = sogutils::getQuests();
            for (auto questID : quests) {
                if (questID < 0 || questID > allQuests.size() - 1) continue; // ok when would this actually happen
                auto quest = allQuests.at(questID);
                auto value = Mod::get()->getSavedValue<int>(fmt::format("q_{}", questID), 0);
                bool increaseValue = false;
                if ((equal(key, "28") || equal(key, "6")) && quest.type == QuestType::StarMoon) { // StarMoon
                    log::debug("Increasing quest for {} (StarMoon) by +{} (before {})", questID, amount, value);
                    increaseValue = true;
                }
                if (equal(key, "5") && quest.type == QuestType::Demon) { // Demon
                    log::debug("Increasing quest for {} (Demon) by +{} (before {})", questID, amount, value);
                    increaseValue = true;
                }
                if (increaseValue) {
                    Mod::get()->setSavedValue(fmt::format("q_{}", questID), std::clamp(value + amount, 0, (int)quest.maxProgress));
                }
            }
        }
    }
};
