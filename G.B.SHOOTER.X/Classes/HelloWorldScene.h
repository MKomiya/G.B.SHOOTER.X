#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    
    void addTarget();
    void spriteMoveFinished(CCNode* sender);
    void gameLogic(float dt);
    
    void addBullet();
    void playerLogic(float dt);
    
    void addEffect(cocos2d::CCPoint startpoint);
    
    void collision(float dt);
    void buttonCallback(CCNode* pSender);
    
    void goNextScene();
    
private:
    cocos2d::CCSprite* player;
    cocos2d::CCPoint prevPoint;
    bool isShooting, isCharging;
    cocos2d::CCArray* targetArray;
    cocos2d::CCArray* bulletArray;
    int score, gameTimer;
    cocos2d::CCLabelTTF* scoreLabel, *timerLabel;
    
    void _initScore();
};

#endif // __HELLOWORLD_SCENE_H__
