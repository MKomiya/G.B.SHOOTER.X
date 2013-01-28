//
//  TitleScene.h
//  MyProject
//
//  Created by S_Wyvern on 2013/01/22.
//
//

#ifndef MyProject_TitleScene_h
#define MyProject_TitleScene_h

#include "cocos2d.h"

class TitleScene : public cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(TitleScene);
    
    void goNextScene();
    void goNextEndlessModeScene();
};

#endif
