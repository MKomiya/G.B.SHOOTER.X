//
//  HelloWorldScene.h
//  G.B.SHOOTER.X
//
//  Created by M.Komiya on 2013/01/22.
//
//

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
    // 時間停止中のゲームスケジューリング
    void stoppingGameLogic(float dt);
    
    /// 各コールバックメソッド
    // スプライトオブジェクトが死んだときの処理
    void spriteMoveFinished(CCNode* sender);
    // ゲームが終わって次のシーンへ行くとき
    void goNextScene();
    // 時間停止の処理
    void setStopTime();
    // ポーズ画面
    void pause();
    // ReadyからGoへスプライトを変化させる
    void changeReadySprite(CCNode* pSender);
    // ゲームスタート
    void gameStart(CCNode* pSender);
    
private:
    cocos2d::CCSprite* player;
    cocos2d::CCPoint prevPoint;
    bool isShooting, isCharging, isEndless, isPausing, isStopping;
    cocos2d::CCArray *targetArray, *bulletArray;
    int score, gameTimer, stopTimer;
    cocos2d::CCLabelBMFont* scoreLabel, *timerLabel;
    
    void _initScore();
};

// ポーズ画面用の簡易シーンクラス
class PauseScene : public cocos2d::CCLayer
{
public:
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(PauseScene);
    
    void backToGame();
    void returnToTitle();
};

#endif // __HELLOWORLD_SCENE_H__
