//
// Created by user on 2026/11/11.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_SCENEMANAGER_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_SCENEMANAGER_H

#include <string>
#include <map>
#include "BaseScene.h"

class SceneManager {
private:
    std::map<std::string, BaseScene*> sceneMap_;
    BaseScene* currentScene_;
public:

    SceneManager() {
        sceneMap_ = std::map<std::string, BaseScene*>();
        currentScene_ = nullptr;
    };
    ~SceneManager();

    void Init();
    void Update();
    void Draw();
    void Release();

    BaseScene* GetCurrentScene() const { return currentScene_;};
    bool AddScene(BaseScene* scene);
    bool ChangeScene(const std::string& sceneName);

};


#endif //ANDROID_GAMEACTIVITY_TEMPLATE_SCENEMANAGER_H
