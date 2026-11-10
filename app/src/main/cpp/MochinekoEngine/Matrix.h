//
// Created by user on 2026/11/09.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_MATRIX_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_MATRIX_H

#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include "mochinekoengine.h"

struct Matrix4x4 {
    float m11, m12, m13, m14;
    float m21, m22, m23, m24;
    float m31, m32, m33, m34;
    float m41, m42, m43, m44;

    Matrix4x4() {
        Identity();
    }

    Matrix4x4& Identity() {
        m11 = 1, m12 = 0, m13 = 0, m14 = 0,
        m21 = 0, m22 = 1, m23 = 0, m24 = 0,
        m31 = 0, m32 = 0, m33 = 1, m34 = 0,
        m41 = 0, m42 = 0, m43 = 0, m44 = 1;
        return *this;
    }

    Matrix4x4& Translation(float x, float y, float z) {
        m11 = 1, m12 = 0, m13 = 0, m14 = 0,
        m21 = 0, m22 = 1, m23 = 0, m24 = 0,
        m31 = 0, m32 = 0, m33 = 1, m34 = 0,
        m41 = x, m42 = y, m43 = z, m44 = 1;
        return *this;
    }

    Matrix4x4& RotationX(float angle) {
        m11 = 1, m12 = 0, m13 = 0, m14 = 0,
        m21 = 0, m22 = std::cos(angle), m23 = -std::sin(angle), m24 = 0,
        m31 = 0, m32 = std::sin(angle), m33 = std::cos(angle), m34 = 0,
        m41 = 0, m42 = 0, m43 = 0, m44 = 1;
        return *this;
    }

    Matrix4x4& RotationY(float angle) {
        m11 = std::cos(angle), m12 = 0, m13 = std::sin(angle), m14 = 0,
        m21 = 0, m22 = 1, m23 = 0, m24 = 0,
        m31 = -std::sin(angle), m32 = 0, m33 = std::cos(angle), m34 = 0,
        m41 = 0, m42 = 0, m43 = 0, m44 = 1;
        return *this;
    }

    Matrix4x4& RotationZ(float angle) {
        m11 = std::cos(angle), m12 = -std::sin(angle), m13 = 0, m14 = 0,
        m21 = std::sin(angle), m22 = std::cos(angle), m23 = 0, m24 = 0,
        m31 = 0, m32 = 0, m33 = 1, m34 = 0,
        m41 = 0, m42 = 0, m43 = 0, m44 = 1;
        return *this;
    }

    Matrix4x4& Scale(float x, float y, float z) {
        m11 = x, m12 = 0, m13 = 0, m14 = 0,
        m21 = 0, m22 = y, m23 = 0, m24 = 0,
        m31 = 0, m32 = 0, m33 = z, m34 = 0,
        m41 = 0, m42 = 0, m43 = 0, m44 = 1;
        return *this;
    }

    Matrix4x4& Perspective(float fov, float aspect, float nearZ, float farZ) {
        float f = 1.0f / std::tan(fov * 0.5f);
        m11 = f / aspect; m12 = 0; m13 = 0; m14 = 0;
        m21 = 0; m22 = f; m23 = 0; m24 = 0;
        m31 = 0; m32 = 0; m33 = (farZ + nearZ) / (nearZ - farZ); m34 = -1;
        m41 = 0; m42 = 0; m43 = (2.0f * farZ * nearZ) / (nearZ - farZ); m44 = 0;
        return *this;
    }

    Matrix4x4& Orthographic(float left, float right, float bottom, float top, float nearZ, float farZ) {
        m11 = 2.0f / (right - left); m12 = 0; m13 = 0; m14 = 0;
        m21 = 0; m22 = 2.0f / (top - bottom); m23 = 0; m24 = 0;
        m31 = 0; m32 = 0; m33 = -2.0f / (farZ - nearZ); m34 = 0;
        m41 = -(right + left) / (right - left); m42 = -(top + bottom) / (top - bottom); m43 = -(farZ + nearZ) / (farZ - nearZ); m44 = 1;
        return *this;
    }

    Matrix4x4& LookAt(const Location& eye, const Location& target, const Location& upDirection) {
        float fx = target.x - eye.x, fy = target.y - eye.y, fz = target.z - eye.z;
        float fl = std::sqrt(fx * fx + fy * fy + fz * fz);
        fx /= fl, fy /= fl, fz /= fl;
        float sx = fy * upDirection.z - fz * upDirection.y,
                sy = fz * upDirection.x - fx * upDirection.z,
                sz = fx * upDirection.y - fy * upDirection.x;
        float sl = std::sqrt(sx * sx + sy * sy + sz * sz);
        sx /= sl, sy /= sl, sz /= sl;
        float ux = sy * fz - sz * fy, uy = sz * fx - sx * fz, uz = sx * fy - sy * fx;
        m11 = sx, m12 = ux, m13 = -fx, m14 = 0,
        m21 = sy, m22 = uy, m23 = -fy, m24 = 0,
        m31 = sz, m32 = uz, m33 = -fz, m34 = 0,
        m41 = -(sx * eye.x + sy * eye.y + sz * eye.z), m42 = -(ux * eye.x + uy * eye.y + uz * eye.z), m43 = fx * eye.x + fy * eye.y + fz * eye.z, m44 = 1;
        return *this;
    }


    GLfloat* build() {
        return &m11;
    }
};


#endif //ANDROID_GAMEACTIVITY_TEMPLATE_MATRIX_H
