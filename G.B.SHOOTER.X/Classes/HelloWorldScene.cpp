#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ResultScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HelloWorld::gameLogic(float dt)
{
    if(gameTimer > 0) gameTimer -= 1;
    CCString* str = CCString::createWithFormat("残り時間：%d", gameTimer);
    timerLabel->setString(str->getCString());
    
    this->addTarget();
    
    if(gameTimer == 0)
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCLabelTTF* pFinish = CCLabelTTF::create("FINISH!!", "Thonburi", 32);
        pFinish->setPosition(ccp(winSize.width/2, winSize.height/2));
        this->addChild(pFinish);
        this->runAction(CCSequence::create(CCActionInterval::create(1.0f), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::goNextScene)), NULL));
    }
}

void HelloWorld::spriteMoveFinished(CCNode* sender)
{
    CCSprite* sprite = (CCSprite*)sender;
    
    if(sprite->getTag() == 1)
    {
        targetArray->fastRemoveObject(sprite);
    }
    else if(sprite->getTag() == 2 || sprite->getTag() == 3)
    {
        bulletArray->removeObject(sprite);
    }
    
    this->removeChild(sprite, true);
}

void HelloWorld::addTarget()
{
    CCSprite* target = CCSprite::create("enemy.png");
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int minY = target->getContentSize().height/2;
    int maxY = winSize.height - minY;
    int rangeY = maxY - minY;
    int actualY = (arc4random()% rangeY)+ minY;
    
    target->setPosition(ccp(winSize.width + (target->getContentSize().width/2), actualY));
    target->setTag(1);
    targetArray->addObject(target);
    this->addChild(target);
    
    int minDuration =2.0;
    int maxDuration =4.0;
    int rangeDuration = maxDuration - minDuration;
    int actualDuration =(arc4random()% rangeDuration)+ minDuration;
    
    CCMoveTo* actionMove = CCMoveTo::create(actualDuration, ccp(-target->getContentSize().width/2, actualY));
    CCCallFuncN* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    target->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
}

void HelloWorld::playerLogic(float dt)
{
    CCString* str = CCString::createWithFormat("Score:%d", score);
    scoreLabel->setString(str->getCString());
    
    if(isShooting)
    {
        this->addBullet();
    }
}

void HelloWorld::addBullet()
{
    CCSprite* bullet = CCSprite::create("bullet.png");
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint startpoint = player->getPosition();
    
    bullet->setPosition(startpoint);
    bullet->setTag(2);
    this->addChild(bullet);
    bulletArray->addObject(bullet);
    
    float speed = 1.0;
    
    CCMoveBy* actionMove = CCMoveBy::create(speed,ccp(winSize.width, 0.0f));
    CCCallFuncN* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    bullet->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
}

void HelloWorld::addEffect(CCPoint startpoint)
{
    CCSprite* effectSprite = CCSprite::create("effect.png", CCRectMake(0, 0, 16, 16));
    effectSprite->setPosition(startpoint);
    this->addChild(effectSprite);
    
    CCArray* animationFrameArrays = CCArray::create();
    for(int i=0; i<9; ++i)
    {
        CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(effectSprite->getTexture(), CCRectMake(16*i, 0, 16, 16));
        animationFrameArrays->addObject(frame);
    }
     
    CCAnimation* effect = CCAnimation::createWithSpriteFrames(animationFrameArrays, 0.02f);
    CCAnimate* actionEffect = CCAnimate::create(effect);
    CCCallFuncN* removeAction = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    effectSprite->runAction(CCSequence::create(actionEffect, removeAction, NULL));
}

void HelloWorld::collision(float dt)
{
    CCObject *bulletobj = NULL, *targetobj = NULL;
    CCARRAY_FOREACH(bulletArray, bulletobj)
    {
        CCSprite* bullet = (CCSprite*)bulletobj;
        CCRect bulletRect = CCRectMake(bullet->getPositionX() - bullet->getContentSize().width/2,
                                       bullet->getPositionY() - bullet->getContentSize().height/2,
                                       bullet->getContentSize().width,
                                       bullet->getContentSize().height);
        CCARRAY_FOREACH(targetArray, targetobj)
        {
            CCSprite* target = (CCSprite*)targetobj;
            CCRect targetRect = CCRectMake(target->getPositionX() - target->getContentSize().width/2,
                                           target->getPositionY() - target->getContentSize().height/2,
                                           target->getContentSize().width,
                                           target->getContentSize().height);
            if(targetRect.intersectsRect(bulletRect))
            {
                score += 100;
                addEffect(target->getPosition());
                targetArray->removeObject(target, false);
                this->removeChild(target, true);
                if(bullet->getTag() == 2)
                {
                    bulletArray->removeObject(bullet, false);
                    this->removeChild(bullet, true);
                }
            }
        }
    }
}

void HelloWorld::_initScore()
{
    this->score = 0;
    this->gameTimer = 60;
    CCString* str = CCString::createWithFormat("Score:%d", score);
    CCString* timeStr = CCString::createWithFormat("残り時間：%d", gameTimer);
    
    scoreLabel = CCLabelTTF::create(str->getCString(), "Thonburi", 24);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    scoreLabel->setPosition( ccp(scoreLabel->getContentSize().width, size.height - 16) );
    this->addChild(scoreLabel, 1);
    
    timerLabel = CCLabelTTF::create(timeStr->getCString(), "Thonburi", 24);
    timerLabel->setPosition( ccp(scoreLabel->getContentSize().width + timerLabel->getContentSize().width, size.height - 16));
    this->addChild(timerLabel, 1);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    this->isShooting = false;
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    prevPoint.setPoint(0.0f, 0.0f);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
    CCMenuItemFont *chargeItem = CCMenuItemFont::create("charge",
                                                        this,
                                                        menu_selector(HelloWorld::buttonCallback));
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
    chargeItem->setPosition( ccp(chargeItem->getContentSize().width/2 + 64, chargeItem->getContentSize().height/2) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, chargeItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    /*
    CCLabelTTF* pLabel = CCLabelTTF::create("Score:", "Thonburi", 24);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp(pLabel->getContentSize().width, size.height - 16) );
    
    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    */
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    this->_initScore();

    // add "HelloWorld" splash screen"
    player = CCSprite::create("player.png");

    // position the sprite on the center of the screen
    player->setPosition( ccp(size.width/2, size.height/2) );

    // add the sprite as a child to this layer
    this->addChild(player, 0);
    
    this->schedule(schedule_selector(HelloWorld::gameLogic), 0.1);
    this->schedule(schedule_selector(HelloWorld::playerLogic), 0.2);
    this->schedule(schedule_selector(HelloWorld::collision));
    
    targetArray = CCArray::create();
    bulletArray = CCArray::create();
    targetArray->retain();
    bulletArray->retain();
    
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::buttonCallback(CCNode* pSender)
{
    CCSprite* bullet = CCSprite::create("bigbullet.png");
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint startpoint = player->getPosition();
    
    bullet->setPosition(startpoint);
    bullet->setTag(3);
    this->addChild(bullet);
    bulletArray->addObject(bullet);
    
    float speed = 1.0;
    
    CCMoveBy* actionMove = CCMoveBy::create(speed,ccp(winSize.width, 0.0f));
    CCCallFuncN* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    bullet->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
}

bool HelloWorld::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
    prevPoint = pTouch->getLocation();
    isShooting = true;
    return true;
}

void HelloWorld::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
{
    CCPoint mover;
    CCPoint point = pTouch->getLocation();
    float moveX = player->getPositionX() + point.x - prevPoint.x;
    float moveY = player->getPositionY() + point.y - prevPoint.y;
    mover.setPoint(moveX, moveY);
    player->setPosition(mover);
    prevPoint = point;
}

void HelloWorld::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
{
    isShooting = false;
}

void HelloWorld::goNextScene()
{
    CCUserDefault::sharedUserDefault()->setIntegerForKey("score", score);
    CCDirector::sharedDirector()->replaceScene(ResultScene::scene());
}

