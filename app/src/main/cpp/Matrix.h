//
// Created by user on 2026/11/09.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_MATRIX_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_MATRIX_H

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>

struct Matrix4x4 {
    float m11, m12, m13, m14;
    float m21, m22, m23, m24;
    float m31, m32, m33, m34;
    float m41, m42, m43, m44;

    Matrix4x4() {
        Identity();
    }

    Matrix4x4 Identity() {
        m11 = 1, m12 = 0, m13 = 0, m14 = 0,
        m21 = 0, m22 = 1, m23 = 0, m24 = 0,
        m31 = 0, m32 = 0, m33 = 1, m34 = 0,
        m41 = 0, m42 = 0, m43 = 0, m44 = 1;
        return *this;
    }

    Matrix4x4 Translation(float x, float y, float z) {
        m11 = 1, m12 = 0, m13 = 0, m14 = 0,
        m21 = 0, m22 = 1, m23 = 0, m24 = 0,
        m31 = 0, m32 = 0, m33 = 1, m34 = 0,
        m41 = x, m42 = y, m43 = z, m44 = 1;
        return *this;
    }

    Matrix4x4 RotationX(float angle) {
        m11 = 1, m12 = 0, m13 = 0, m14 = 0,
        m21 = 0, m22 = std::cos(angle), m23 = -std::sin(angle), m24 = 0,
        m31 = 0, m32 = std::sin(angle), m33 = std::cos(angle), m34 = 0,
        m41 = 0, m42 = 0, m43 = 0, m44 = 1;
        return *this;
    }

    Matrix4x4 RotationY(float angle) {
        m11 = std::cos(angle), m12 = 0, m13 = std::sin(angle), m14 = 0,
        m21 = 0, m22 = 1, m23 = 0, m24 = 0,
        m31 = -std::sin(angle), m32 = 0, m33 = std::cos(angle), m34 = 0,
        m41 = 0, m42 = 0, m43 = 0, m44 = 1;
        return *this;
    }

    Matrix4x4 RotationZ(float angle) {
        m11 = std::cos(angle), m12 = -std::sin(angle), m13 = 0, m14 = 0,
        m21 = std::sin(angle), m22 = std::cos(angle), m23 = 0, m24 = 0,
        m31 = 0, m32 = 0, m33 = 1, m34 = 0,
        m41 = 0, m42 = 0, m43 = 0, m44 = 1;
        return *this;
    }

    Matrix4x4 Scale(float x, float y, float z) {
        m11 = x, m12 = 0, m13 = 0, m14 = 0,
        m21 = 0, m22 = y, m23 = 0, m24 = 0,
        m31 = 0, m32 = 0, m33 = z, m34 = 0,
        m41 = 0, m42 = 0, m43 = 0, m44 = 1;
        return *this;
    }

    GLfloat* build() {
        return &m11;
    }
};


#endif //ANDROID_GAMEACTIVITY_TEMPLATE_MATRIX_H
