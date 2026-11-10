//
// Created by user on 2026/11/10.
//
#include "MochinekoEngine/mochinekoengine.h"

using namespace MochinekoEngine;

void android_main(struct android_app *pApp) {
    SetMainLoop([] {

    });

    if (!InitEngine(pApp)) {
        // エンジンの初期化に失敗時の処理
    }
}