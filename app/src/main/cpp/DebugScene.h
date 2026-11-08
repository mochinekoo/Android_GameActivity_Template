//
// Created by user on 2026/11/08.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_DEBUGSCENE_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_DEBUGSCENE_H

#include <EGL/egl.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

class DebugScene {
private:
    bool initialized_;
public:

    inline DebugScene() {
        initialized_ = false;
    }

    ~DebugScene();

    void Init();
    void Update();
    void Draw();

    bool getInitialized() const { return initialized_; }
};


#endif //ANDROID_GAMEACTIVITY_TEMPLATE_DEBUGSCENE_H
