//
// Created by user on 2026/11/08.
//

#include <android/asset_manager.h>
#include <string>
#include <game-activity/GameActivityLog.h>
#include "ShaderManager.h"

ShaderManager::~ShaderManager() {

}

void ShaderManager::Init() {
    GLint vertexShader = LoadFile(GL_VERTEX_SHADER, "Vertex.glsl");
    GLint fragmentShader = LoadFile(GL_FRAGMENT_SHADER, "Fragment.glsl");

    float vertices[] = {
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f
    };

    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);
    glLinkProgram(shaderProgram_);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint resultFunc = GL_FALSE;
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &resultFunc);

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

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

GLint ShaderManager::LoadFile(GLenum type, const char *path) {
    AAsset* asset = AAssetManager_open(app_->activity->assetManager, path, AASSET_MODE_BUFFER);
    if (asset == nullptr) {
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "アセットを取得できませんでした(%s)", path);
    }
    const auto length = AAsset_getLength(asset);
    std::string shader((size_t) length, '\0');
    AAsset_read(asset, shader.data(), length);
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%sの状態: %s", path, shader.c_str());
    AAsset_close(asset);

    return LoadCode(type, shader.c_str());
}

GLint ShaderManager::LoadCode(GLenum type, const char *source) {
    GLint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint resultFunc = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &resultFunc);
    return shader;
}


