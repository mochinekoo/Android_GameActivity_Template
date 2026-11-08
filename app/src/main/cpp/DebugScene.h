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

    android_app* app_;
    EGLDisplay display_;
    EGLSurface surface_;
    EGLContext context_;
    EGLint width_;
    EGLint height_;
public:

    inline DebugScene(android_app* pApp) {
        app_ = pApp;
        display_ = EGLDisplay();
        surface_ = EGLSurface();
        context_ = EGLContext();
        width_ = 0;
        height_ = 0;
        initialized_ = false;
    }

    ~DebugScene();

    void Init();
    void Update();
    void Draw();

    bool getInitialized() const { return initialized_; }
    android_app* getApp() { return app_; }
    EGLDisplay getDisplay() const { return display_; }
    EGLSurface getSurface() const { return surface_; }
    EGLContext getContext() const { return context_; }
    EGLint getWidth() const { return width_; }
    EGLint getHeight() const { return height_; }
};


#endif //ANDROID_GAMEACTIVITY_TEMPLATE_DEBUGSCENE_H
