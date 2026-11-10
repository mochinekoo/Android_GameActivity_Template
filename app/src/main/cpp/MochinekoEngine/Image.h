//
// Created by user on 2026/11/08.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_IMAGE_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_IMAGE_H


#include <android/asset_manager.h>
#include <GLES3/gl3.h>
#include <vector>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "mochinekoengine.h"

class Image {
private:
    GLuint textureID_;
    const char* path_;
    GLfloat width_, height_;
public:
    Transform transform_;

    Image(const char* path) {
            path_ = path;
            transform_ = Transform();
            width_ = 0.0f;
            height_ = 0.0f;
    };

    GLuint getTextureID() const { return textureID_; }

    void Init();
    void Update();
    void Draw();
    void Release();

    void InitAsset();
    void InitShader();
};


#endif //ANDROID_GAMEACTIVITY_TEMPLATE_IMAGE_H
