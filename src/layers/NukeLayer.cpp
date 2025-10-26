#include "NukeLayer.hpp"
#include <defs/utils.hpp>

NukeLayer* NukeLayer::create() {
    auto ret = new NukeLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CCScene* NukeLayer::scene() {
    auto layer = NukeLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

void NukeLayer::playSound() {
    m_sog->removeFromParentAndCleanup(true);
    FMODAudioEngine::sharedEngine()->playMusic("BOOM.mp3"_spr, false, 0, 0);
    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();
    Build<CCLabelBMFont>::create("the true soggy mods were", "bigFont.fnt")
        .scale(0.7f)
        .pos(size.width / 2, (size.height / 2) + 10)
        .opacity(0)
        .runAction(CCSequence::create(
            CCDelayTime::create(2.5f),
            CCFadeIn::create(2.0f),
            CCDelayTime::create(2.0f),
            CCFadeOut::create(2.0f),
            nullptr
        ))
        .parent(this);
    // for some reason its crashing if i just .children so we just do this ig
    Build<CCLabelBMFont>::create("the friends we made along the way", "bigFont.fnt")
        .scale(0.7f)
        .pos(size.width / 2, (size.height / 2) - 10)
        .opacity(0)
        .runAction(CCSequence::create(
            CCDelayTime::create(2.5f),
            CCFadeIn::create(2.0f),
            CCDelayTime::create(2.0f),
            CCFadeOut::create(2.0f),
            CCCallFunc::create(this, callfunc_selector(NukeLayer::endScene)),
            nullptr
        ))
        .parent(this);
}
void NukeLayer::endScene() {
    CCArray* dialogArr = CCArray::create();
    DialogObject* soggyDialog = DialogObject::create("soggy", "<s200>WHY DID YOU NUKE SOG</s>", 2, 1.f, false, {255, 255, 255});
    soggyDialog->setTag(1004268);
    DialogObject* soggyDialog2 = DialogObject::create("soggy", "<d010>.<d010>.<d010>. ok for that I will <cr>NUKE</c> your <cy>sog points</c> and remove <cy>EVERYTHING</c> and make all items <co>more expensive</c>.<d050> <cr>GOODBYE</c>", 2, 0.8f, false, {255, 255, 255});
    soggyDialog2->setTag(1004251);
    dialogArr->addObject(soggyDialog);
    dialogArr->addObject(soggyDialog2);
    DialogLayer* dialogLayer = DialogLayer::createWithObjects(dialogArr, 2);
    this->addChild(dialogLayer);
    dialogLayer->animateInRandomSide();
}

bool NukeLayer::init() {
    if (!CCLayer::init()) return false;
    Build<CCSprite>::create("SOGGY.png"_spr).store(m_sog).pos(CCDirector::sharedDirector()->getWinSize() / 2).parent(this);
    sogutils::toggleLayerHook("DialogLayer::displayDialogObject", true);
    sogutils::toggleLayerHook("DialogLayer::onClose", true);
    Mod::get()->setSavedValue("nuke01", true);
    //1004268
    //1004251
    return true;
}

void NukeLayer::onEnterTransitionDidFinish() {
    CCLayer::onEnterTransitionDidFinish();
    CCArray* animFrames = CCArray::createWithCapacity(21);
    if (!animFrames) return;
    auto director = CCDirector::sharedDirector();
    auto size = director->getWinSize();
    for (int i = 247; i < 269; i++) {
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fmt::format("frame_0{}.png"_spr, i).c_str());
        if (frame) {
            animFrames->addObject(frame);
        }
    }
    if (animFrames->count() == 0) return;
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.16f);
    Build<CCSprite>::createSpriteName("frame_0247.png"_spr)
        .anchorPoint(0, 0)
        .parent(this)
        .opacity(0)
        .runAction(CCSequence::create(
            CCDelayTime::create(0.75f),
            CCSpawn::create(CCFadeIn::create(0.1f), CCAnimate::create(animation), CCSequence::createWithTwoActions(CCDelayTime::create(0.16f), CCCallFunc::create(this, callfunc_selector(NukeLayer::playSound))), nullptr),
            CCFadeOut::create(1.25f),
            nullptr
        ))
        .scaleToMatch(size);
}
