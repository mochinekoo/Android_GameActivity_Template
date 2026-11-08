//
// Created by user on 2026/11/09.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_MOCHINEKOENGINE_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_MOCHINEKOENGINE_H

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "ShaderManager.h"

namespace MochinekoEngine {

    android_app* GetApp();
    ShaderManager* GetShaderManager();

    bool CanShutdown();
    void Shutdown();
}

#endif //ANDROID_GAMEACTIVITY_TEMPLATE_MOCHINEKOENGINE_H
