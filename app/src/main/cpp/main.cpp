#include <jni.h>

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <game-activity/GameActivity.h>
#include <iostream>
#include <android/log.h>
#include <game-activity/GameActivityLog.h>
#include "DebugScene.h"

namespace {
    DebugScene* debugScene_ = nullptr;
}

void handleApp(android_app *pApp, int32_t cmd) {
    switch (cmd) {
        case APP_CMD_INIT_WINDOW: {
            debugScene_->Init();
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

            if (debugScene_->getSurface() != EGL_NO_SURFACE
                    && debugScene_->getContext() != EGL_NO_CONTEXT
                    && debugScene_->getInitialized()) {
                debugScene_->Update();
                debugScene_->Draw();
            }
        }
    } while (!pApp->destroyRequested);
}