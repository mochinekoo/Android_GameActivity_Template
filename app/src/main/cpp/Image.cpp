//
// Created by user on 2026/11/08.
//

#include <android/imagedecoder.h>
#include "Image.h"
#include "ShaderManager.h"
#include <vector>
#include "mochinekoengine.h"

using namespace MochinekoEngine;

void Image::Init() {
    InitShader();
}

void Image::Update() {

}

void Image::Draw() {
    glUseProgram(GetShaderManager()->getShaderProgram());

    const GLfloat identity[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };

    GLint projectionLocation = glGetUniformLocation(GetShaderManager()->getShaderProgram(), "uProjection");

    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, identity);

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

void Image::InitShader() {
    float vertices[] = {
            -0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f
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
