//
// Created by user on 2026/11/11.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_BASESCENE_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_BASESCENE_H


class BaseScene {
private:
protected:
    std::string name_;
public:

    BaseScene(const std::string& sceneName) {
        name_ = sceneName;
    }

    BaseScene() {}

    std::string GetName() const {  return name_; }

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual void Release() = 0;

};


#endif //ANDROID_GAMEACTIVITY_TEMPLATE_BASESCENE_H
