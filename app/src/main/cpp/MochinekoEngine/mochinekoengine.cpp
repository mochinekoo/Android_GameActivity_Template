#include <jni.h>

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <game-activity/GameActivity.h>
#include <iostream>
#include <android/log.h>
#include <game-activity/GameActivityLog.h>
#include "DebugScene.h"
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_impl_opengl3.h"
#include "../ImGUI/imgui_impl_android.h"
#include "mochinekoengine.h"
#include "ShaderManager.h"

using namespace MochinekoEngine;

namespace {
    DebugScene* debugScene_ = nullptr;
}

namespace MochinekoEngine {
    inline bool canShutdown_;
    inline android_app* androidApp_;
    inline ShaderManager* shaderManager_;

    inline EGLDisplay display_;
    inline EGLSurface surface_;
    inline EGLContext context_;
    inline EGLint width_;
    inline EGLint height_;

    inline float background_[4];

    inline std::function<void()> mainLoopFunc_;

    void SetMainLoop(std::function<void()> func) {
        mainLoopFunc_ = func;
    }

    bool InitEngine(android_app* app) {
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "起動したよ");
        MochinekoEngine::androidApp_ = app;
        MochinekoEngine::canShutdown_ = false;
        MochinekoEngine::shaderManager_ = new ShaderManager(app);
        MochinekoEngine::background_[0] = 1.0f;
        MochinekoEngine::background_[1] = 1.0f;
        MochinekoEngine::background_[2] = 1.0f;
        MochinekoEngine::background_[3] = 1.0f;

        app->onAppCmd = handleApp;
        debugScene_ = new DebugScene();

        RunMainLoop();
        return true;
    }

    void RunMainLoop() {
        do {
            while (true) {
                int timeout = 0;
                int events = 0;
                android_poll_source* pSource = nullptr;
                int result = ALooper_pollOnce(
                        timeout,
                        nullptr,
                        &events,
                        reinterpret_cast<void**>(&pSource)
                );

                // 0秒の場合は、常にALOOPER_POLL_TIMEOUT（-3）を返す
                switch (result) {
                    case ALOOPER_POLL_ERROR: {
                        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "エラーが発生しました");
                        break;
                    }
                    case ALOOPER_EVENT_ERROR: {
                        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "イベントのエラーが発生しました");
                        break;
                    }
                }

                if (pSource != nullptr) {
                    pSource->process(GetApp(), pSource);
                }

                handleEvent(GetApp());
                if (GetSurface() != EGL_NO_SURFACE && GetContext() != EGL_NO_CONTEXT
                    && debugScene_->getInitialized()) {
                    eglQuerySurface(GetDisplay(), GetSurface(), EGL_WIDTH, GetWidthPointer());
                    eglQuerySurface(GetDisplay(), GetSurface(), EGL_HEIGHT, GetHeightPointer());

                    auto bgColor = getBackground();
                    glViewport(0, 0, GetWidth(), GetHeight());
                    glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
                    glClearDepthf(1.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    ImGui_ImplAndroid_NewFrame();
                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui::NewFrame();

                    debugScene_->Update();
                    debugScene_->Draw();

                    if (mainLoopFunc_ != NULL) {
                        mainLoopFunc_();
                    }

                    ImGui::Render();
                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
                    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                        ImGui::UpdatePlatformWindows();
                        ImGui::RenderPlatformWindowsDefault();
                    }

                    eglSwapBuffers(display_, surface_);
                }
            }
        } while (!GetApp()->destroyRequested || MochinekoEngine::canShutdown_);
    }

    android_app* GetApp() { return androidApp_; }

    ShaderManager* GetShaderManager() { return shaderManager_; }

    bool CanShutdown() { return canShutdown_; }
    void Shutdown() { canShutdown_ = true; }

    float* getBackground() {return background_;}
    void setBackground(float r, float g, float b, float a) {
        background_[0] = r;
        background_[1] = g;
        background_[2] = b;
        background_[3] = a;
    };

    EGLDisplay GetDisplay() { return display_; }
    EGLSurface GetSurface() { return surface_; }
    EGLContext GetContext() { return context_; }
    EGLint GetWidth() { return width_; }
    EGLint* GetWidthPointer() { return &width_; }
    EGLint GetHeight()  { return height_; }
    EGLint* GetHeightPointer() { return &height_;}

    void InitOpenGL() {
        EGLBoolean resultFunc = false;

        MochinekoEngine::display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        resultFunc = eglInitialize(MochinekoEngine::display_ , nullptr, nullptr);
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "eglの初期化: %d", resultFunc);

        constexpr EGLint attribs[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_DEPTH_SIZE, 24,
                EGL_NONE
        };

        EGLConfig config = EGLConfig();
        EGLint numConfig = 0;
        resultFunc = eglChooseConfig(MochinekoEngine::display_ , attribs, &config, 1, &numConfig);
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "eglのConfig: %d", resultFunc);

        EGLint format = 0;
        resultFunc = eglGetConfigAttrib(display_ , config, EGL_NATIVE_VISUAL_ID, &format);
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "getConfigAttrib: %d", resultFunc);

        resultFunc = ANativeWindow_setBuffersGeometry(GetApp()->window, 0, 0, format);
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "setBuffersGeometry: %d", resultFunc);

        const EGLint contextAttribs[] = {
                EGL_CONTEXT_CLIENT_VERSION, 3,
                EGL_NONE
        };
        context_ = eglCreateContext(GetDisplay(), config, EGL_NO_CONTEXT, contextAttribs);

        surface_ = eglCreateWindowSurface(GetDisplay(), config, GetApp()->window, nullptr);
        resultFunc = eglMakeCurrent(GetDisplay(), surface_, GetSurface(), GetContext());
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "サーフェス: %d", resultFunc);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }

    void InitImGUI(ANativeWindow* window) {
        ImGui::CreateContext();
        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(3.0f);
        io.FontGlobalScale = 3.0f;

        ImGui::StyleColorsDark();
        ImGui_ImplAndroid_Init(window);
        ImGui_ImplOpenGL3_Init();
    }

    void handleEvent(android_app* app) {
        auto *inputBuffer = android_app_swap_input_buffers(app);
        if (!inputBuffer) {
            return;
        }
        for (auto i = 0; i < inputBuffer->motionEventsCount; i++) {
            auto &motionEvent = inputBuffer->motionEvents[i];
            auto action = motionEvent.action;
            auto pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                    >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
            auto &pointer = motionEvent.pointers[pointerIndex];
            auto x = GameActivityPointerAxes_getX(&pointer);
            auto y = GameActivityPointerAxes_getY(&pointer);

            auto& io = ImGui::GetIO();
            switch (action & AMOTION_EVENT_ACTION_MASK) {
                case AMOTION_EVENT_ACTION_DOWN:
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%d(Down): %f, %f", pointer.id, x, y);
                    io.AddMousePosEvent(x, y);
                    io.AddMouseButtonEvent(0, true);
                    break;

                case AMOTION_EVENT_ACTION_CANCEL:
                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_POINTER_UP:
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%d(Up): %f, %f", pointer.id, x, y);
                    io.AddMousePosEvent(x, y);
                    io.AddMouseButtonEvent(0, false);
                    break;

                case AMOTION_EVENT_ACTION_MOVE:
                    for (auto index = 0; index < motionEvent.pointerCount; index++) {
                        pointer = motionEvent.pointers[index];
                        x = GameActivityPointerAxes_getX(&pointer);
                        y = GameActivityPointerAxes_getY(&pointer);
                        io.AddMousePosEvent(x, y);
                        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "%d(Down): %f, %f", pointer.id, x, y);
                    }
                    break;
                default:
                    // __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Other Action: %d", action);
            }
        }
        android_app_clear_motion_events(inputBuffer);

        for (auto i = 0; i < inputBuffer->keyEventsCount; i++) {
            auto &keyEvent = inputBuffer->keyEvents[i];
            __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Key: %d", keyEvent.keyCode);
            switch (keyEvent.action) {
                case AKEY_EVENT_ACTION_DOWN:
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Down Key");
                    break;
                case AKEY_EVENT_ACTION_UP:
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Up Key");
                    break;
                case AKEY_EVENT_ACTION_MULTIPLE:
                    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Multiple Key");
                    break;
                default:
                    // __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Other Key Action: %d", keyEvent.action);
            }
        }
        android_app_clear_key_events(inputBuffer);
    }

    void handleApp(android_app *pApp, int32_t cmd) {
        switch (cmd) {
            case APP_CMD_INIT_WINDOW: {
                InitOpenGL();
                GetShaderManager()->Init();
                debugScene_->Init();
                InitImGUI(pApp->window);
                __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "アプリの初期化");
                break;
            }
            case APP_CMD_TERM_WINDOW: {
                if (pApp->userData != nullptr) {

                }
                break;
            }
            case APP_CMD_DESTROY: {
                __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "アプリの破棄");
                break;
            }
        }
    }

}