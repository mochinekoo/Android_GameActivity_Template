//
// Created by user on 2026/11/11.
//

#include "SceneManager.h"
#include "DebugScene.h"

SceneManager::~SceneManager() {

}

void SceneManager::Init() {
    sceneMap_.clear();

    auto debugScene = new DebugScene();
    AddScene(debugScene);

    ChangeScene("DebugScene");
}

void SceneManager::Update() {
    if (currentScene_ == nullptr) return;
    currentScene_->Update();
}

void SceneManager::Draw() {
    if (currentScene_ == nullptr) return;
    currentScene_->Draw();
}

void SceneManager::Release() {

}

bool SceneManager::AddScene(BaseScene *scene) {
    if (sceneMap_.find(scene->GetName()) == sceneMap_.end()) {	// シーンが存在しない場合
        sceneMap_[scene->GetName()] = scene;	// シーンをマップに追加する
        return true;
    }
    return false;	// シーンが既に存在する場合はfalseを返す
}

bool SceneManager::ChangeScene(const std::string &sceneName) {
    if (sceneMap_.find(sceneName) != sceneMap_.end()) {	// シーンが見つかった場合
        currentScene_ = sceneMap_[sceneName];		// 現在のシーンを切り替える
        currentScene_->Init();						// 現在のシーンを初期化する
        return true;
    }
    return false;	// シーンが見つからなかった場合はfalseを返す
}
