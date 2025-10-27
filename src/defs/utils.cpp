#include "utils.hpp"

std::vector<ShopCategory> s_categories = {
    {
        1,
        "SOG",
    },
    {
        2,
        "Games"
    },
    {
        3,
        "funny"
    }
};

std::vector<ShopItem> s_items = {
    {
        1,
        "sog name",
        "Sogs the menu title!",
        0,
        1
    },
    {
        2,
        "sog icons",
        "Sogs all of the cube icons! (ONLY cube, not ship...)",
        10,
        1
    },
    {
        3,
        "sog buttons",
        "Changes all buttons to be soggy! (May break mods...)",
        250,
        1
    },
    {
        4,
        "sog background",
        "changes all backgrounds to be sog.",
        100,
        1
    },
    {
        5,
        "sog rain",
        "<cb>ITS RAINING SOG</c> (go to mod settings to change settings for this)",
        250,
        3
    },
    {
        7,
        "jumpscare",
        "because <co>halloween</c>",
        2,
        2
    },
    {
        8,
        "minigames",
        "to pass time, play some *generic* sog-themed mini games!!!",
        51,
        2
    },
    {
        9,
        "nuke",
        "funny nuke, also <cr>USING IT IS IRREVERSABLE!</c>",
        2000,
        3
    },
    {
        6,
        "SOG EVERYTHING",
        "you will regret this.",
        //500,
        999999,
        3
    },
    {
        10,
        "nothing",
        "does <cp>ABSOLUTELY NOTHING</c>, please dont buy this. This is just a placeholder.",
        10000,
        3
    },
    /*{
        11,
        "battle sog!!",
        "you will have a bad time",
        250,
        2
    },*/
    {
        10,
        "nothing",
        "does <cp>ABSOLUTELY NOTHING</c>, please dont buy this. This is just a placeholder.",
        10000,
        2
    },
    {
        10,
        "nothing",
        "does <cp>ABSOLUTELY NOTHING</c>, please dont buy this. This is just a placeholder.",
        10000,
        2
    },
};
std::vector<ShopItem> s_itemsBuf = {};

std::vector<Quest> s_quests = {
    {
        "Soggy Finder",
        "Find Sog somewhere!",
        {50, 100},
        1,
        QuestType::FindSog
    },
    {
        "Star Finder",
        "Collect 3 stars/moons.",
        {10, 50},
        3,
        QuestType::StarMoon
    },
    {
        "Star Collector",
        "Collect 10 stars/moons.",
        {50, 70},
        10,
        QuestType::StarMoon
    },
    {
        "Star Master",
        "Collect 25 stars/moons.",
        {100, 500},
        25,
        QuestType::StarMoon
    },
    {
        "Demon Grinder",
        "Beat a demon level.",
        {100, 500},
        1,
        QuestType::Demon
    },
    {
        "Apple Eater",
        "Gain 10 points in Snake",
        {30, 70},
        10,
        QuestType::MinigameS
    },
    {
        "Apple Muncher",
        "Gain 50 points in Snake",
        {100, 250},
        50,
        QuestType::MinigameS
    },
    {
        "Apple Devourer",
        "Gain 100 points in Snake",
        {300, 500},
        100,
        QuestType::MinigameS
    },
    {
        "Pong!",
        "Win 10 points in Pong",
        {100, 120},
        10,
        QuestType::MinigameP
    },
};

std::vector<ShopCategory> sogutils::getCategories() {
    return s_categories;
}
std::vector<ShopItem> sogutils::getItems() {
    if (Mod::get()->hasSavedValue("nuke02")) {
        if (s_itemsBuf.size() == 0) {
            for (auto item : s_items) {
                item.price *= 3;
                s_itemsBuf.push_back(item);
            }
        }
        return s_itemsBuf;
    } else {
        return s_items;
    }
}
std::vector<Quest> sogutils::getQuests() {
    return s_quests;
}
