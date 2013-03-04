//
//  HelloWorldScene.cpp
//  G.B.SHOOTER.X
//
//  Created by M.Komiya on 2013/01/22.
//
//

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ResultScene.h"
#include "TitleScene.h"

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
    
    if(isPausing)
    {
        isPausing = false;
        return ;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCLabelBMFont* ready = CCLabelBMFont::create("Ready...", "TextImageFont2.fnt");
    CCBlink* blinkAction1 = CCBlink::create(0.3f, 2);
    CCBlink* blinkAction2 = CCBlink::create(0.3f, 5);
    CCBlink* blinkAction3 = CCBlink::create(0.2f, 10);
    CCCallFuncN* changeSpriteAction = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::changeReadySprite));
    CCCallFuncN* start = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::gameStart));
    
    ready->setPosition(ccp(winSize.width/2, winSize.height/2));
    ready->runAction(CCSequence::create(blinkAction1, blinkAction2, blinkAction3, changeSpriteAction, CCDelayTime::create(0.3f), start, NULL));
    
    this->addChild(ready);
}

void HelloWorld::gameLogic(float dt)
{
    if(!isEndless)
    {
        if(gameTimer > 0) gameTimer -= 1;
        CCString* str = CCString::createWithFormat("time:%d", gameTimer);
        timerLabel->setString(str->getCString());
    }
    else
    {
        CCString* str = CCString::create("time:∞");
        timerLabel->setString(str->getCString());
    }
    
    this->addTarget();
    
    if(gameTimer == 0)
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCLabelBMFont* pFinish = CCLabelBMFont::create("FINISH!!", "TextImageFont2.fnt", 32);
        pFinish->setPosition(ccp(winSize.width/2, winSize.height/2));
        this->addChild(pFinish);
        this->runAction(CCSequence::create(CCActionInterval::create(1.0f), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::goNextScene)), NULL));
    }
}

void HelloWorld::stoppingGameLogic(float dt)
{
    if(stopTimer > 0) stopTimer -= 1;
    
    if(stopTimer == 0)
    {
        // フラグ解除
        isStopping = false;

        float speed = 1.0;
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        // 全ての弾オブジェクトに移動属性を持たせる
        CCObject *bulletobj, *targetobj;
        CCARRAY_FOREACH(bulletArray, bulletobj)
        {
            CCSprite* bullet = (CCSprite*)bulletobj;
            CCMoveBy* actionMove = CCMoveBy::create(speed,ccp(winSize.width, 0.0f));
            CCCallFuncN* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
            bullet->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
            bullet->resumeSchedulerAndActions();
        }
        
        // 全ての敵オブジェクトのアクションを再開する
        CCARRAY_FOREACH(targetArray, targetobj)
        {
            CCSprite* target = (CCSprite*)targetobj;
            target->resumeSchedulerAndActions();
        }
        
        // スケジューラー再登録
        this->schedule(schedule_selector(HelloWorld::gameLogic), 0.1);
        
        // 停止中スケジューラーを解放
        this->unschedule(schedule_selector(HelloWorld::stoppingGameLogic));
    }
}

void HelloWorld::spriteMoveFinished(CCNode* sender)
{
    CCSprite* sprite = (CCSprite*)sender;
    
    if(sprite->getTag() >= 10)
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
    int enemyType = arc4random() % 10;
    
    target->setPosition(ccp(winSize.width + (target->getContentSize().width/2), actualY));
    target->setTag(10);
    this->addChild(target);
    targetArray->addObject(target);
    
    // 10%の確率で赤色敵が出てくるようにする
    if( enemyType == 1 )
    {
        target->setTag(11);
        target->setColor(ccc3(255, 64, 64));
    }
    // 10%の確率で緑色敵が出てくるようにする
    else if( enemyType == 2 )
    {
        target->setTag(12);
        target->setColor(ccc3(64, 255, 64));
    }
    
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
    CCString* str = CCString::createWithFormat("score:%d", score);
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
    
    // 停止中でないなら移動アクションを持たせる
    if(!isStopping)
    {
        CCMoveBy* actionMove = CCMoveBy::create(speed,ccp(winSize.width, 0.0f));
        CCCallFuncN* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
        bullet->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
    }
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
                score += 100 * (target->getTag() == 11 ? 2 : 1);
                gameTimer += 30 * (!isEndless && target->getTag() == 12 ? 1 : 0);
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("destroy.wav");
                addEffect(target->getPosition());
                CCParticleSystemQuad* particle = CCParticleSystemQuad::create("BurstParticle.plist");
                particle->setPosition(target->getPosition());
                particle->setAutoRemoveOnFinish(true);
                this->addChild(particle);
                
                // 特殊敵だった場合はラベルエフェクトを作る
                if(target->getTag() >= 11)
                {
                    CCString* str = CCString::create(target->getTag() == 11 ? "Double Score!" : target->getTag() == 12 ? "Time +30!" : "Error");
                    CCLabelBMFont* font = CCLabelBMFont::create(str->getCString(), "TextImageFont2.fnt");
                    font->setPosition(target->getPosition());
                    font->setScale(0.5f);
                    font->setOpacity(0);
                    this->addChild(font);
                
                    CCFadeIn* fade = CCFadeIn::create(0.1f);
                    CCMoveBy* move = CCMoveBy::create(0.1f, ccp(0.0f, 10.f));
                    font->runAction(CCSequence::create(CCSpawn::create(fade, move, NULL), CCDelayTime::create(0.3f), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished)), NULL));
                }
                
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
    CCString* str = CCString::createWithFormat("score:%d", score);
    CCString* timeStr = CCString::createWithFormat("time:%d", gameTimer);
    
    scoreLabel = CCLabelBMFont::create(str->getCString(), "TextImageFont2.fnt");
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    scoreLabel->setAnchorPoint(ccp(0, 0.5f) );
    scoreLabel->setPosition( ccp(16, size.height - 16) );
    this->addChild(scoreLabel, 1);

    timerLabel = CCLabelBMFont::create(timeStr->getCString(), "TextImageFont2.fnt");
    timerLabel->setAnchorPoint( ccp(0, 0.5f) );
    timerLabel->setPosition( ccp(18, scoreLabel->getPositionY() - timerLabel->getContentSize().height/2 - 16));
    this->addChild(timerLabel, 1);
}

bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSprite* pBack = CCSprite::create("back.jpg");
    pBack->setColor(ccc3(128, 128, 128));
    this->addChild(pBack, 0);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    this->isPausing = false;
    this->isShooting = false;
    this->isStopping = false;
    this->stopTimer = 0;
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    prevPoint.setPoint(0.0f, 0.0f);

    CCLabelBMFont* pBMFontTime = CCLabelBMFont::create("stop time", "TextImageFont2.fnt");
    CCLabelBMFont* pBMPauseFont = CCLabelBMFont::create("pause", "TextImageFont2.fnt");
    CCMenuItemLabel* stopItem = CCMenuItemLabel::create(pBMFontTime, this, menu_selector(HelloWorld::setStopTime));
    CCMenuItemLabel* pauseItem = CCMenuItemLabel::create(pBMPauseFont, this, menu_selector(HelloWorld::pause));
    CCMenu* pMenu = CCMenu::create(stopItem, pauseItem, NULL);

    stopItem->setPosition(ccp(size.width - pBMFontTime->getContentSize().width/2 - 16, pBMFontTime->getContentSize().height/2 + 8));
    pauseItem->setPosition(ccp(size.width - pBMPauseFont->getContentSize().width, size.height - pBMPauseFont->getContentSize().height));
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
    
    return true;
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
    CCUserDefault::sharedUserDefault()->flush();
    CCDirector::sharedDirector()->replaceScene(ResultScene::scene());
}

void HelloWorld::setStopTime()
{
    // 停止フラグを立てる
    this->isStopping = true;
    this->stopTimer = 10;
    
    // 敵が出てこないようにする
    this->unschedule(schedule_selector(HelloWorld::gameLogic));
    
    // 止まるオブジェクトは反転描画させる
    ccBlendFunc bf = ccBlendFunc();
    bf.src = GL_ONE_MINUS_DST_COLOR;
    bf.dst = GL_ZERO;
    
    // 次に、現在画面内にあるオブジェクトのアクションを止める
    CCObject *bulletobj, *targetobj;
    CCARRAY_FOREACH(bulletArray, bulletobj)
    {
        CCSprite* bullet = (CCSprite*)bulletobj;
        bullet->pauseSchedulerAndActions();
        bullet->setBlendFunc(bf);
    }
    CCARRAY_FOREACH(targetArray, targetobj)
    {
        CCSprite* target = (CCSprite*)targetobj;
        target->pauseSchedulerAndActions();
        target->setBlendFunc(bf);
    }
    
    // 時間停止中のスケジューラー登録
    this->schedule(schedule_selector(HelloWorld::stoppingGameLogic), 0.1f);
}

void HelloWorld::pause()
{
    isPausing = true;
    CCDirector::sharedDirector()->pushScene(PauseScene::scene());
}

void HelloWorld::changeReadySprite(CCNode* pSender)
{
    CCLabelBMFont* label = (CCLabelBMFont*)pSender;
    label->setString("Go!");
}

void HelloWorld::gameStart(CCNode* pSender)
{
    CCLabelBMFont* label = (CCLabelBMFont*)pSender;
    this->removeChild(label, true);
    
    this->schedule(schedule_selector(HelloWorld::gameLogic), 0.1);
    this->schedule(schedule_selector(HelloWorld::playerLogic), 0.05);
    this->schedule(schedule_selector(HelloWorld::collision));
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game1.mp3", true);
}

CCScene* PauseScene::scene()
{
    CCScene *scene = CCScene::create();
    PauseScene *layer = PauseScene::create();
    
    scene->addChild(layer);
    return scene;
}

bool PauseScene::init()
{
    if( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCLabelBMFont* pBMFont = CCLabelBMFont::create("Pause Mode", "TextImageFont2.fnt");
    CCLabelBMFont* pBMBack = CCLabelBMFont::create("back to game", "TextImageFont2.fnt");
    CCLabelBMFont* pBMReturn = CCLabelBMFont::create("return to title", "TextImageFont2.fnt");
    CCMenuItemLabel* backItem = CCMenuItemLabel::create(pBMBack, this, menu_selector(PauseScene::backToGame));
    CCMenuItemLabel* returnItem = CCMenuItemLabel::create(pBMReturn, this, menu_selector(PauseScene::returnToTitle));
    CCMenu* menu = CCMenu::create(backItem, returnItem, NULL);
    pBMFont->setScale(1.5f);
    pBMFont->setPosition(ccp(winSize.width/2, winSize.height/2));
    backItem->setPosition(ccp(winSize.width/2, winSize.height/2 - pBMFont->getContentSize().height - pBMBack->getContentSize().height));
    returnItem->setPosition(ccp(winSize.width/2, backItem->getPositionY() - pBMReturn->getContentSize().height));
    menu->setPosition( CCPointZero );
    this->addChild(pBMFont);
    this->addChild(menu);
    
    return true;
}

void PauseScene::backToGame()
{
    CCDirector::sharedDirector()->popScene();
}

void PauseScene::returnToTitle()
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CCDirector::sharedDirector()->replaceScene(TitleScene::scene());
}