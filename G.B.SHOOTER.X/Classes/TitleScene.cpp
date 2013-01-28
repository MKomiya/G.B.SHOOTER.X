//
//  TitleScene.cpp
//  MyProject
//
//  Created by S_Wyvern on 2013/01/22.
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
    pSprite->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(pSprite);
    
    CCLabelBMFont* pLabel1 = CCLabelBMFont::create("start", "TextImageFont.fnt", 32);
    CCLabelBMFont* pLabel2 = CCLabelBMFont::create("endless", "TextImageFont.fnt");
    CCMenuItemLabel* pLabelItem1 = CCMenuItemLabel::create(pLabel1, this, menu_selector(TitleScene::goNextScene));
    CCMenuItemLabel* pLabelItem2 = CCMenuItemLabel::create(pLabel2, this, menu_selector(TitleScene::goNextEndlessModeScene));
    pLabelItem1->setPosition(ccp(winSize.width/2, 70));
    pLabelItem2->setPosition(ccp(winSize.width/2, 30));
    
    CCMenu* pMenu = CCMenu::create(pLabelItem1, pLabelItem2, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu);
    
    return true;
}

void TitleScene::goNextScene()
{
    CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void TitleScene::goNextEndlessModeScene()
{
    CCDirector::sharedDirector()->replaceScene(HelloWorld::scene(true));
}