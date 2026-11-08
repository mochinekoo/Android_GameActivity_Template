//
// Created by user on 2026/11/08.
//

#ifndef ANDROID_GAMEACTIVITY_TEMPLATE_SHADERMANAGER_H
#define ANDROID_GAMEACTIVITY_TEMPLATE_SHADERMANAGER_H

#include <GLES3/gl3.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

class ShaderManager {
private:
    GLuint shaderProgram_;
    GLuint vao_;
    GLuint vbo_;
    android_app* app_;
public:

    ShaderManager(android_app* app) {
        app_ = app;
        shaderProgram_ = 0;
        vao_ = 0;
        vbo_ = 0;
    };
    ~ShaderManager();

    void Init();
    GLint LoadCode(GLenum type, const char* source);
    GLint LoadFile(GLenum type, const char* path);

    GLuint getShaderProgram() const { return shaderProgram_; }
    GLuint getVAO() const { return vao_; }
    GLuint getVBO() const { return vbo_; }

};


#endif //ANDROID_GAMEACTIVITY_TEMPLATE_SHADERMANAGER_H
