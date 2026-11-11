//
// Created by user on 2026/11/08.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_DEBUGSCENE_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_DEBUGSCENE_H

#include <EGL/egl.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "BaseScene.h"

class DebugScene : public BaseScene {
private:
public:

    inline DebugScene()
        : BaseScene("DebugScene") {
    }

    ~DebugScene();

    void Init() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};


#endif //ANDROID_GAMEACTIVITY_TEMPLATE_DEBUGSCENE_H
