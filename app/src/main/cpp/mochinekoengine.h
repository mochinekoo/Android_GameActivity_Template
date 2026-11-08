//
// Created by user on 2026/11/09.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_MOCHINEKOENGINE_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_MOCHINEKOENGINE_H

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "ShaderManager.h"

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

    android_app* GetApp();
    ShaderManager* GetShaderManager();

    bool CanShutdown();
    void Shutdown();
}

#endif //ANDROID_GAMEACTIVITY_TEMPLATE_MOCHINEKOENGINE_H
