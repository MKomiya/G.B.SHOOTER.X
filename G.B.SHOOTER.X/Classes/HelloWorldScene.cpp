#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ResultScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene(bool endressmode)
{
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();

    layer->isEndless = endressmode;
    
    scene->addChild(layer);
    return scene;
}

void HelloWorld::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
    
    this->schedule(schedule_selector(HelloWorld::gameLogic), 0.1);
    this->schedule(schedule_selector(HelloWorld::playerLogic), 0.2);
    this->schedule(schedule_selector(HelloWorld::collision));
}

void HelloWorld::gameLogic(float dt)
{
    if(!isEndless)
    {
        if(gameTimer > 0) gameTimer -= 1;
        CCString* str = CCString::createWithFormat("残り時間：%d", gameTimer);
        timerLabel->setString(str->getCString());
    }
    else
    {
        CCString* str = CCString::create("残り時間：∞");
        timerLabel->setString(str->getCString());
    }
    
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
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* target = CCSprite::create("enemy.png");
    
    int minY = target->getContentSize().height/2;
    int maxY = winSize.height - minY;
    int rangeY = maxY - minY;
    int actualY = (arc4random()% rangeY)+ minY;
    
    target->setPosition(ccp(winSize.width + (target->getContentSize().width/2), actualY));
    target->setTag(1);
    this->addChild(target);
    targetArray->addObject(target);
    
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
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("destroy.wav");
                score += 100;
                addEffect(target->getPosition());
                CCParticleSystemQuad* particle = CCParticleSystemQuad::create("BurstParticle.plist");
                particle->setPosition(target->getPosition());
                particle->setAutoRemoveOnFinish(true);
                this->addChild(particle);
                
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
    this->gameTimer = -1;
    if(isEndless == false)
    {
        this->gameTimer = 60;
    }
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

bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    this->isShooting = false;
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    prevPoint.setPoint(0.0f, 0.0f);

    CCLabelBMFont* pBMFont = CCLabelBMFont::create("power shot", "TextImageFont.fnt");
    CCMenuItemLabel* chargeItem = CCMenuItemLabel::create(pBMFont, this, menu_selector(HelloWorld::buttonCallback));
    CCLabelBMFont* pBMFontTime = CCLabelBMFont::create("stop time", "TextImageFont.fnt");
    CCMenuItemLabel* stopItem = CCMenuItemLabel::create(pBMFontTime, this, menu_selector(HelloWorld::setStopTime));
    CCMenu* pMenu = CCMenu::create(chargeItem, stopItem, NULL);
    
    pBMFont->setScale(0.5f);
    pBMFontTime->setScale(0.5f);
    chargeItem->setPosition(ccp(size.width, pBMFont->getContentSize().height/2));
    stopItem->setPosition(ccp(size.width - pBMFont->getContentSize().width, pBMFontTime->getContentSize().height/2));
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    this->_initScore();

    player = CCSprite::create("player.png");
    player->setPosition( ccp(size.width/2, size.height/2) );
    this->addChild(player, 0);
    
    targetArray = CCArray::create();
    bulletArray = CCArray::create();
    targetArray->retain();
    bulletArray->retain();
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game1.mp3", true);
    
    return true;
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
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint mover;
    CCPoint point = pTouch->getLocation();
    float moveX = player->getPositionX() + point.x - prevPoint.x;
    float moveY = player->getPositionY() + point.y - prevPoint.y;
    
    moveX = MAX(player->getContentSize().width/2, MIN(winSize.width-player->getContentSize().width/2, moveX));
    moveY = MAX(player->getContentSize().height/2, MIN(winSize.height-player->getContentSize().height/2, moveY));
    
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
    CocosDenshion::SimpleAudioEngine::sharedEngine()->unloadEffect("destroy.wav");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
    
    CCUserDefault::sharedUserDefault()->setIntegerForKey("score", score);
    CCDirector::sharedDirector()->replaceScene(ResultScene::scene());
}

void HelloWorld::setStopTime()
{
    
}

