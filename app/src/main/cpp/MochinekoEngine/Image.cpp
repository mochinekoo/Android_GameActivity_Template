//
// Created by user on 2026/11/08.
//

#include <android/imagedecoder.h>
#include "Image.h"
#include "ShaderManager.h"
#include <vector>
#include "mochinekoengine.h"
#include "Matrix.h"
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "../glm/glm.hpp"
#include "../glm/ext/scalar_constants.hpp"
#include "../glm/ext/matrix_clip_space.hpp"
#include "../glm/gtc/type_ptr.hpp"

using namespace MochinekoEngine;

void Image::Init() {
    InitAsset();
    InitShader();
}

void Image::Update() {

}

void Image::Draw() {
    glUseProgram(GetShaderManager()->getShaderProgram());

    glm::mat4 projectionMatrix = glm::ortho(0.0f, (float) GetWidth(), (float) GetHeight(), 0.0f, 100.0f, -100.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 worldMatrix(1.0f);
    worldMatrix = glm::translate(worldMatrix, glm::vec3(transform_.location_.x, transform_.location_.y, transform_.location_.z));
    worldMatrix = glm::rotate(worldMatrix, glm::radians(transform_.rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));
    worldMatrix = glm::rotate(worldMatrix, glm::radians(transform_.rotation_.y), glm::vec3(0, 1, 0));
    worldMatrix = glm::rotate(worldMatrix, glm::radians(transform_.rotation_.x), glm::vec3(1, 0, 0));
    worldMatrix = glm::scale(worldMatrix, glm::vec3(transform_.scale_.x, transform_.scale_.y, transform_.scale_.z));
    glm::mat4 worldViewProjectionMatrix = projectionMatrix * viewMatrix * worldMatrix;

    GLint location = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uWorldMatrix");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(worldMatrix));
    GLint view = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uViewMatrix");
    glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    GLint projectionLocation = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uProjectionMatrix");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, getTextureID());

    GLint textureLocation = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uTexture");

    glUniform1i(textureLocation, 0);

    glBindVertexArray(GetShaderManager()->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Image::Release() {

}

void Image::InitAsset() {

    ///
    /// アセットから取得
    ///
    AAsset* asset = AAssetManager_open(GetApp()->activity->assetManager, path_, AASSET_MODE_BUFFER);
    AImageDecoder* imageDecoder = nullptr;
    AImageDecoder_createFromAAsset(asset, &imageDecoder);
    AImageDecoder_setAndroidBitmapFormat(imageDecoder, ANDROID_BITMAP_FORMAT_RGBA_8888);
    const AImageDecoderHeaderInfo* headerInfo = nullptr;
    headerInfo = AImageDecoder_getHeaderInfo(imageDecoder);
    width_ = AImageDecoderHeaderInfo_getWidth(headerInfo);
    height_ = AImageDecoderHeaderInfo_getHeight(headerInfo);
    unsigned int stride = AImageDecoder_getMinimumStride(imageDecoder);
    std::vector<uint8_t> imageData = std::vector<uint8_t>(height_ * stride);
    AImageDecoder_decodeImage(imageDecoder, imageData.data(), stride, imageData.size());

    ///
    /// 画像を作成
    ///
    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
            GL_TEXTURE_2D,  // テクスチャの種類
            0, // ミップマップレベル
            GL_RGBA, // フォーマット
            width_, // テクスチャの幅
            height_, // テクスチャの高さ
            0, // ボーダー
            GL_RGBA, // フォーマット
            GL_UNSIGNED_BYTE,
            imageData.data() // 画像のデータ
    );
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Image::InitShader() {
    float vertices[] = {
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 左上
            0.0f, height_, 0.0f, 0.0f, 1.0f, // 左下
            width_, 0.0f, 0.0f, 1.0f, 0.0f,   // 右上

            width_, 0.0f, 0.0f, 1.0f, 0.0f,   // 右上
            0.0f, height_, 0.0f, 0.0f, 1.0f, // 左下
            width_, height_, 0.0f, 1.0f, 1.0f   // 右下
    };
    glBindVertexArray(GetShaderManager()->getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, GetShaderManager()->getVBO());

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float),
                          nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float),
                          reinterpret_cast<void*>(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
