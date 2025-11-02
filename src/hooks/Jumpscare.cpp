#include <defs/utils.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(CLCustom, CreatorLayer) {
    void step1() {
        onOnlineLevels(nullptr);
    }
    bool init() {
        if (!CreatorLayer::init()) return false;
        runAction(CCSequence::create(
            CCDelayTime::create(0.1F),
            CCCallFunc::create(this, callfunc_selector(CLCustom::step1)),
            nullptr
        ));
        sogutils::toggleLayerHook("CreatorLayer::init", false);
        return true;
    }
};

class $modify(LSLCustom, LevelSearchLayer) {   
    void step1() {
        if ((rand() % 100) < 90) {
            m_searchInput->setString("68668045");
        } else {
            m_searchInput->setString("86407629");
        }
    }
    void step2() {
        onSearch(nullptr);
    }
    bool init(int p0) {
        if (!LevelSearchLayer::init(p0)) return false;
        log::debug("event! go!");
        runAction(CCSequence::create(
            CCDelayTime::create(0.1F),
            CCCallFunc::create(this, callfunc_selector(LSLCustom::step1)),
            CCDelayTime::create(0.1F),
            CCCallFunc::create(this, callfunc_selector(LSLCustom::step2)),
            nullptr
        ));
        sogutils::toggleLayerHook("LevelSearchLayer::init", false);
        return true;
    }
};

class $modify(LBLCustom, LevelBrowserLayer) {
    void step1() {
        if (auto levelCell = typeinfo_cast<LevelCell*>(m_list->m_listView->m_tableView->m_contentLayer->getChildren()->objectAtIndex(0))) {
            log::debug("Found level, going now...");
            levelCell->onClick(nullptr);
        }
    }
    void loadLevelsFinished(cocos2d::CCArray *p0, const char *p1, int p2) {
        LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
        if (p0->count() == 0) return;
        runAction(CCSequence::create(
            CCDelayTime::create(0.1F),
            CCCallFunc::create(this, callfunc_selector(LBLCustom::step1)),
            nullptr
        ));
        sogutils::toggleLayerHook("LevelBrowserLayer::loadLevelsFinished", false);
    }
};

class $modify(LILCustom, LevelInfoLayer) {
    struct Fields {
        CCAction* loopAction = nullptr;
    };
    void step1() {
        CCDirector::sharedDirector()->setFastMenu(GameManager::get()->getGameVariable("0168"));
        auto MDM = MusicDownloadManager::sharedState();
        bool songDownloaded = MDM->isSongDownloaded(m_level->m_songID) || (m_level->m_songID == 0);
        if (!songDownloaded) return;
        log::debug("Start playing!");
        if (PlayLayer::get() != nullptr) {
            return stopAction(m_fields->loopAction);
        }
        onPlay(nullptr);
    }
    void levelDownloadFinished(GJGameLevel *p0) {
        LevelInfoLayer::levelDownloadFinished(p0);
        if (m_songWidget->m_downloadBtn->isVisible()) {
            m_songWidget->onDownload(nullptr);
        }
        if (m_fields->loopAction != nullptr) return;
        m_fields->loopAction = CCRepeatForever::create(CCSequence::create(
            CCDelayTime::create(3.0F),
            CCCallFunc::create(this, callfunc_selector(LILCustom::step1)),
            nullptr
        ));
        runAction(m_fields->loopAction);
        sogutils::toggleLayerHook("LevelInfoLayer::levelDownloadFinished", false);
    }
    
    void updateLabelValues() {
        LevelInfoLayer::updateLabelValues();
        if (m_fields->loopAction != nullptr) return;
        m_fields->loopAction = CCRepeatForever::create(CCSequence::create(
            CCDelayTime::create(1.0F),
            CCCallFunc::create(this, callfunc_selector(LILCustom::step1)),
            nullptr
        ));
        runAction(m_fields->loopAction);
        sogutils::toggleLayerHook("LevelInfoLayer::updateLabelValues", false);
    }
    
};

$on_mod(Loaded) {
    sogutils::toggleLayerHook("CreatorLayer::init", false);
    sogutils::toggleLayerHook("LevelSearchLayer::init", false);
    sogutils::toggleLayerHook("LevelBrowserLayer::loadLevelsFinished", false);
    sogutils::toggleLayerHook("LevelInfoLayer::updateLabelValues", false);
    sogutils::toggleLayerHook("LevelInfoLayer::levelDownloadFinished", false);
}
