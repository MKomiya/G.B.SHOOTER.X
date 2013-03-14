//
//  ResultScene.h
//  G.B.SHOOTER.X
//
//  Created by M.Komiya on 2013/01/23.
//
//

#ifndef __Result_Scene_H__
#define __Result_Scene_H__

#include "cocos2d.h"

class ResultScene : public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene* scene(int score);
    virtual bool init();
    
    CREATE_FUNC(ResultScene);
    
    void goNextScene();
    void setScore(int score);
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    
private:
    int resultScore;
    cocos2d::CCString* string;
    cocos2d::CCLabelBMFont* pLabel;
};

#endif /* defined(__MyProject__ResultScene__) */
