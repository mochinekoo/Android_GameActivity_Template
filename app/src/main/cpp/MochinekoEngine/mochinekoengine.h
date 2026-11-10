//
// Created by user on 2026/11/09.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_MOCHINEKOENGINE_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_MOCHINEKOENGINE_H

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "ShaderManager.h"
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <functional>

struct Location {
    float x, y, z;
};

struct Transform {
    Location location_;
    Location rotation_;
    Location scale_;

    Transform() {
        location_ = {0, 0, 0};
        rotation_ = {0, 0, 0};
        scale_ = {1.0f, 1.0f, 1.0f};
    }
};

struct Vector {
    float x, y, z;
};

struct Vertex {
    Location location_;
    float u_, v_;
};

namespace MochinekoEngine {

    /**
     * エンジンを初期化する関数
     * @param app
     * @return エンジンの初期化に成功した場合は、trueを返す
     */
    bool InitEngine(android_app* app);

    /**
     * OpenGLを初期化する関数
     */
    void InitOpenGL();

    /**
     * ImGUIを初期化する関数
     * @param window
     */
    void InitImGUI(ANativeWindow* window);

    /**
     * エンジン側のメインループの処理
     */
    void RunMainLoop();

    /**
     * ユーザー側のメインループの処理（RunMainLoop内で実行）
     * @param func ラムダ式を書く
     */
    void SetMainLoop(std::function<void()> func);

    /**
     * 実行中のandroid_app構造体を返す関数
     */
    android_app* GetApp();
    ShaderManager* GetShaderManager();

    void handleEvent(android_app* app);
    void handleApp(android_app *pApp, int32_t cmd);

    float* getBackground();

    /**
     * 背景を設定する関数
     */
    void setBackground(float r, float g, float b, float a);

    bool CanShutdown();

    /**
     * エンジンをシャットダウンする関数
     */
    void Shutdown();

    EGLDisplay GetDisplay();
    EGLSurface GetSurface();
    EGLContext GetContext();
    EGLint GetWidth();
    EGLint* GetWidthPointer();
    EGLint GetHeight();
    EGLint* GetHeightPointer();
}

#endif //ANDROID_GAMEACTIVITY_TEMPLATE_MOCHINEKOENGINE_H
