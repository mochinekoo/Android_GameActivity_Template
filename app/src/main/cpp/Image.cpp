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

    GLfloat projectionMatrix[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };

    GLfloat viewMatrix[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };

    GLfloat translationMatrix[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            transform_.location_.x, transform_.location_.y, transform_.location_.z, 1
    };

    GLfloat scaleMatrix[16] = {
            transform_.scale_.x, 0, 0, 0,
            0, transform_.scale_.y, 0, 0,
            0, 0, transform_.scale_.z, 0,
            0, 0, 0, 1
    };

    GLfloat rotationXMatrix[16] = {
            1, 0, 0, 0,
            0, std::cos(transform_.rotation_.x), -std::sin(transform_.rotation_.x), 0,
            0, std::sin(transform_.rotation_.x), std::cos(transform_.rotation_.x), 0,
            0, 0, 0, 1
    };

    GLfloat rotationYMatrix[16] = {
            std::cos(transform_.rotation_.y), 0, std::sin(transform_.rotation_.y), 0,
            0, 1, 0, 0,
            -std::sin(transform_.rotation_.y), 0, std::cos(transform_.rotation_.y), 0,
            0, 0, 0, 1
    };

    GLfloat rotationZMatrix[16] = {
            std::cos(transform_.rotation_.z), -std::sin(transform_.rotation_.z), 0, 0,
            std::sin(transform_.rotation_.z), std::cos(transform_.rotation_.z), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };

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
