//
//  ResultScene.cpp
//  G.B.SHOOTER.X
//
//  Created by M.Komiya on 2013/01/23.
//
//

#include "ResultScene.h"
#include "TitleScene.h"

using namespace cocos2d;

CCScene* ResultScene::scene(int score)
{
    CCScene* scene = CCScene::create();
    ResultScene* layer = ResultScene::create();
    layer->setScore(score);
    
    scene->addChild(layer);
    
    return scene;
}

bool ResultScene::init()
{
    if( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    
    string = CCString::createWithFormat("ResultScore:%d", resultScore);
    pLabel = CCLabelBMFont::create(string->getCString(), "TextImageFont.fnt");
    
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

void ResultScene::setScore(int score)
{
    resultScore = score;
    string->initWithFormat("ResultScore:%d", resultScore);
    pLabel->setString(string->getCString());
}