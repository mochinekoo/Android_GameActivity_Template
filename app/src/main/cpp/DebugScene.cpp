//
// Created by user on 2026/11/08.
//

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <GLES3/gl3.h>
#include <memory>
#include <vector>
#include <android/imagedecoder.h>
#include <game-activity/GameActivityLog.h>
#include <android/log.h>

#include "DebugScene.h"

DebugScene::~DebugScene() {
}

void DebugScene::Init() {
    EGLBoolean resultFunc = false;

    display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    resultFunc = eglInitialize(display_, nullptr, nullptr);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "eglの初期化: %d", resultFunc);

    constexpr EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_NONE
    };

    EGLConfig config = EGLConfig();
    EGLint numConfig = 0;
    resultFunc = eglChooseConfig(display_, attribs, &config, 1, &numConfig);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "eglのConfig: %d", resultFunc);

    EGLint format = 0;
    resultFunc = eglGetConfigAttrib(display_, config, EGL_NATIVE_VISUAL_ID, &format);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "getConfigAttrib: %d", resultFunc);

    resultFunc = ANativeWindow_setBuffersGeometry(app_->window, 0, 0, format);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "setBuffersGeometry: %d", resultFunc);

    const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3,
            EGL_NONE
    };
    context_ = eglCreateContext(display_, config, EGL_NO_CONTEXT, contextAttribs);

    surface_ = eglCreateWindowSurface(display_, config, app_->window, nullptr);
    resultFunc = eglMakeCurrent(display_, surface_, surface_, context_);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "サーフェス: %d", resultFunc);

    initialized_ = true;
}

void DebugScene::Update() {

}

void DebugScene::Draw() {
    eglQuerySurface(display_, surface_, EGL_WIDTH, &width_);
    eglQuerySurface(display_, surface_, EGL_HEIGHT, &height_);

    glViewport(0, 0, width_, height_);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(display_, surface_);
}
