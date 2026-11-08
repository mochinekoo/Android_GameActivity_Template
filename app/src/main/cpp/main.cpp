#include <jni.h>

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <game-activity/GameActivity.h>
#include <iostream>
#include <android/log.h>
#include <game-activity/GameActivityLog.h>
#include "DebugScene.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_android.h"

namespace {
    DebugScene* debugScene_ = nullptr;
}

void initImGUI(ANativeWindow* window) {
    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();
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
            debugScene_->Init();
            initImGUI(pApp->window);
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

void android_main(struct android_app *pApp) {
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "起動したよ");
    pApp->onAppCmd = handleApp;
    debugScene_ = new DebugScene(pApp);

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
                pSource->process(pApp, pSource);
            }

            handleEvent(pApp);
            if (debugScene_->getSurface() != EGL_NO_SURFACE
                    && debugScene_->getContext() != EGL_NO_CONTEXT
                    && debugScene_->getInitialized()) {
                debugScene_->Update();
                debugScene_->Draw();
            }
        }
    } while (!pApp->destroyRequested);
}