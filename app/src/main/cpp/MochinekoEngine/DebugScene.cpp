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
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_impl_opengl3.h"
#include "../ImGUI/imgui_impl_android.h"

namespace {
    Image* image_ = nullptr;
}

using namespace MochinekoEngine;

DebugScene::~DebugScene() {
}

void DebugScene::Init() {
    image_ = new Image("android_robot.png");
    image_->Init();
}

void DebugScene::Update() {

}

void DebugScene::Draw() {
    image_->Update();
    image_->Draw();

    //imgui
    auto width = GetWidth();
    auto height = GetHeight();
    ImGui::Begin("Debug");
    ImGui::SliderFloat("LocX", &image_->transform_.location_.x, -width, width);
    ImGui::SliderFloat("LocY", &image_->transform_.location_.y, -height, height);
    ImGui::SliderFloat("LocZ", &image_->transform_.location_.z, -1.0f, 1.0f);
    ImGui::SliderFloat("ScaleX", &image_->transform_.scale_.x, 0, 10.0f);
    ImGui::SliderFloat("ScaleY", &image_->transform_.scale_.y, 0, 10.0f);
    ImGui::SliderFloat("ScaleZ", &image_->transform_.scale_.z, 0, 10.0f);
    ImGui::SliderFloat("RotationX", &image_->transform_.rotation_.x, -1.0f, 1.0f);
    ImGui::SliderFloat("RotationY", &image_->transform_.rotation_.y, -1.0f, 1.0f);
    ImGui::SliderFloat("RotationZ", &image_->transform_.rotation_.z, -1.0f, 1.0f);
    ImGui::End();
}

void DebugScene::Release() {

}
