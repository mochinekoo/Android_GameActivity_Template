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
#include "ShaderManager.h"
#include "Image.h"
#include "mochinekoengine.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_android.h"

namespace {
    Image* image_ = nullptr;
}

using namespace MochinekoEngine;

DebugScene::~DebugScene() {
}

void DebugScene::Init() {
    image_ = Image::Load(GetApp(), "android_robot.png");
    image_->Init();
    initialized_ = true;
}

void DebugScene::Update() {

}

void DebugScene::Draw() {
    image_->Update();
    image_->Draw();

    //imgui
    ImGui::Begin("Debug");
    ImGui::End();
}
