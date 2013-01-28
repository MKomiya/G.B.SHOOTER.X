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
    
    CCLabelBMFont* pLabel = CCLabelBMFont::create("Start", "TextImageFont.fnt", 32);
    CCMenuItemLabel* pLabelItem = CCMenuItemLabel::create(pLabel, this, menu_selector(TitleScene::goNextScene));
    pLabelItem->setPosition(ccp(winSize.width/2, 40));
    
    CCMenu* pMenu = CCMenu::create(pLabelItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu);
    
    return true;
}

void TitleScene::goNextScene()
{
    CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}