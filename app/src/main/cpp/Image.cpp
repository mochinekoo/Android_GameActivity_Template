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

using namespace MochinekoEngine;

void Image::Init() {
    InitAsset();
    InitShader();
}

void Image::Update() {

}

void Image::Draw() {
    glUseProgram(GetShaderManager()->getShaderProgram());

    GLfloat* projectionMatrix = Matrix4x4().Orthographic(0, getWidth(), getHeight(), 0, 0, 100).build();
    GLfloat* viewMatrix = Matrix4x4().Identity().build();

    GLfloat* translationMatrix = Matrix4x4().Translation(
            transform_.location_.x,
            transform_.location_.y,
            transform_.location_.z).build();
    GLfloat* scaleMatrix = Matrix4x4().Scale(
            transform_.scale_.x,
            transform_.scale_.y,
            transform_.scale_.z).build();
    GLfloat* rotationXMatrix = Matrix4x4().RotationX(transform_.rotation_.x).build();
    GLfloat* rotationYMatrix = Matrix4x4().RotationY(transform_.rotation_.y).build();
    GLfloat* rotationZMatrix = Matrix4x4().RotationZ(transform_.rotation_.z).build();

    GLint location = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uTranslationMatrix");
    glUniformMatrix4fv(location, 1, GL_FALSE, translationMatrix);

    GLint scale = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uScaleMatrix");
    glUniformMatrix4fv(scale, 1, GL_FALSE, scaleMatrix);

    GLint rotationX = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uRotationXMatrix");
    glUniformMatrix4fv(rotationX, 1, GL_FALSE, rotationXMatrix);

    GLint rotationY = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uRotationYMatrix");
    glUniformMatrix4fv(rotationY, 1, GL_FALSE, rotationYMatrix);

    GLint rotationZ = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uRotationZMatrix");
    glUniformMatrix4fv(rotationZ, 1, GL_FALSE, rotationZMatrix);

    GLint view = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uViewMatrix");
    glUniformMatrix4fv(view, 1, GL_FALSE, viewMatrix);

    GLint projectionLocation = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uProjectionMatrix");
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);

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
