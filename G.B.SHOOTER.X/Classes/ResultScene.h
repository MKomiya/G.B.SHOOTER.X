//
//  ResultScene.h
//  MyProject
//
//  Created by S_Wyvern on 2013/01/23.
//
//

#ifndef __Result_Scene_H__
#define __Result_Scene_H__

#include "cocos2d.h"

class ResultScene : public cocos2d::CCLayer
{
public:
    static cocos2d::CCScene* scene();
    virtual bool init();
    
    CREATE_FUNC(ResultScene);
    
    void goNextScene();
    
    virtual bool ccTouchBegan(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch* pTouch, cocos2d::CCEvent* pEvent);
};

#endif /* defined(__MyProject__ResultScene__) */
