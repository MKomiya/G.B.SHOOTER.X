//
//  ResultScene.cpp
//  MyProject
//
//  Created by S_Wyvern on 2013/01/23.
//
//

#include "ResultScene.h"
#include "TitleScene.h"

using namespace cocos2d;

CCScene* ResultScene::scene()
{
    CCScene* scene = CCScene::create();
    
    ResultScene* layer = ResultScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool ResultScene::init()
{
    if( !CCLayer::init() )
    {
        return false;
    }
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    int resultScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("score");
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCString* string = CCString::createWithFormat("ResultScore:%d", resultScore);
    CCLabelBMFont* pLabel = CCLabelBMFont::create(string->getCString(), "TextImageFont.fnt");
    pLabel->setScale(0.6f);
    pLabel->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(pLabel);
    
    return true;
}

void ResultScene::goNextScene()
{
    CCDirector::sharedDirector()->replaceScene(TitleScene::scene());
}

bool ResultScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    return true;
}

void ResultScene::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    goNextScene();
}