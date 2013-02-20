//
//  TitleScene.cpp
//  G.B.SHOOTER.X
//
//  Created by M.Komiya on 2013/01/22.
//
//

#include "TitleScene.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* TitleScene::scene()
{
    CCScene* scene = CCScene::create();
    
    TitleScene* layer = TitleScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool TitleScene::init()
{
    if( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* pSprite = CCSprite::create("title.png");
    CCLabelBMFont* pLabel1 = CCLabelBMFont::create("start", "TextImageFont.fnt", 32);
    CCLabelBMFont* pLabel2 = CCLabelBMFont::create("endless", "TextImageFont.fnt");
    CCMenuItemLabel* pLabelItem1 = CCMenuItemLabel::create(pLabel1, this, menu_selector(TitleScene::goNextScene));
    CCMenuItemLabel* pLabelItem2 = CCMenuItemLabel::create(pLabel2, this, menu_selector(TitleScene::goNextEndlessModeScene));
    CCMenu* pMenu = CCMenu::create(pLabelItem1, pLabelItem2, NULL);
    
    pSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
    pLabelItem1->setPosition(ccp(winSize.width/2, 70));
    pLabelItem2->setPosition(ccp(winSize.width/2, 30));
    pMenu->setPosition( CCPointZero );
    
    this->addChild(pSprite);
    this->addChild(pMenu);
    
    return true;
}

void TitleScene::goNextScene()
{
    CCScene* pScene = HelloWorld::scene();
    CCDirector::sharedDirector()->setDepthTest(true);
    CCTransitionCrossFade* fade = CCTransitionCrossFade::create(1.0f, pScene);
    CCDirector::sharedDirector()->replaceScene(fade);
}

void TitleScene::goNextEndlessModeScene()
{
    CCScene* pScene = HelloWorld::scene(true);
    CCDirector::sharedDirector()->setDepthTest(true);
    CCTransitionCrossFade* fade = CCTransitionCrossFade::create(1.0f, pScene);
    CCDirector::sharedDirector()->replaceScene(fade);
}