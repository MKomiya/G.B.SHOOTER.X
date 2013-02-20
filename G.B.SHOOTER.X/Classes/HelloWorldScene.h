#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
    // 初期化処理
    virtual bool init();
    virtual void onEnterTransitionDidFinish();

    // CCSceneインスタンスを外部へ渡す
    static cocos2d::CCScene* scene(bool endlessmode=false);

    // create関数のマクロ
    CREATE_FUNC(HelloWorld);
    
    // タッチイベント
    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchMoved(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    
    // オブジェクトの生成
    void addTarget();
    void addBullet();
    void addEffect(cocos2d::CCPoint startpoint);
    
    // スケジューリング
    void gameLogic(float dt);
    void playerLogic(float dt);
    void collision(float dt);
    
    // 各コールバックメソッド
    void spriteMoveFinished(CCNode* sender);
    void buttonCallback(CCNode* pSender);
    void goNextScene();
    void setStopTime();
    
private:
    cocos2d::CCSprite* player;
    cocos2d::CCPoint prevPoint;
    bool isShooting, isCharging, isEndless;
    cocos2d::CCArray* targetArray;
    cocos2d::CCArray* bulletArray;
    int score, gameTimer;
    cocos2d::CCLabelTTF* scoreLabel, *timerLabel;
    
    void _initScore();
};

#endif // __HELLOWORLD_SCENE_H__
