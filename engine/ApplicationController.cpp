/*
 * The MIT License (MIT)
 * Copyright (c) 2013 GClue, inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "ApplicationController.h"
#include "GCube.h"
#include "Main.h"

using namespace GCube;

static ApplicationController *_instance = NULL;

/////////////////////////////////////////////////////////////////////////
#pragma mark - Life cycle
	
ApplicationController* ApplicationController::SharedInstance() {
//	LOGD("ApplicationController::SharedInstance()");
	if (!_instance) _instance = new ApplicationController();
	return _instance;
}

void ApplicationController::DestroyInstance() {
	LOGD("ApplicationController::DestroyInstance()");
	SoundPlayer::Dispose();
	delete _instance;
	_instance = NULL;
}

ApplicationController::ApplicationController() {
	LOGD("ApplicationController::ApplicationController()");
	// デフォルトビュー作成
	View_ptr v(new View());
	v->isFullScreen = true;
	v->camera = StandardCamera_ptr(new StandardCamera("DefaultCamera"));
	v->camera->transform.translate(0, 0, 50);
	viewArray.push_back(v);
	// メイン作成
	main = new Main();
}

ApplicationController::~ApplicationController(void) {
	LOGD("ApplicationController::~ApplicationController()");
	delete main;
}


/////////////////////////////////////////////////////////////////////////
#pragma mark - Public method

// シーン切り替え
void ApplicationController::changeScene(const Scene_ptr &nextScene, SceneTransition *transition) {
	// TODO: トランジションサポート
	activeScene = nextScene;
}

// 言語取得
std::string ApplicationController::getLanguage() {
	 return GCGetLanguage();
}

// リソース取得
void ApplicationController::getResource(const char *fileName, std::vector<char>& outData) {
	GCGetResourceData(fileName, outData);
}

// ストレージパスを取得
std::string ApplicationController::getStoragePath(StorageType type) {
	return GCGetStoragePath(type);
}

// ユーザーイベント
int ApplicationController::sendUserEvent(int type, int param1, long long param2, float param3, double param4, const char *param5) {
	LOGD("sendUserEvent(t: %d, 1: %d, 2: %lld, 3:%f, 4:%f, 5:%s)", type, param1, param2, param3, param4, param5);
	return GCSendUserEvent(type, param1, param2, param3, param4, param5);
}


/////////////////////////////////////////////////////////////////////////
#pragma mark - IApplicationEventListener

// 初期化
void ApplicationController::onInit(void) {
	LOGD("ApplicationController::onInit()");
	SoundPlayer::SharedInstance();
	main->onInit();
}

void ApplicationController::onTerminate(void) {
	LOGD("ApplicationController::onTerminate()");
	main->onTerminate();
}

void ApplicationController::onPause(void) {
	LOGD("ApplicationController::onPause()");
	main->onPause();
	SoundPlayer::Pause();
}

void ApplicationController::onResume(void) {
	LOGD("ApplicationController::onResume()");
	main->onResume();
	SoundPlayer::Start();
}

void ApplicationController::onContextChanged(void) {
	LOGD("ApplicationController::onContextChanged()");
	main->onContextChanged();
}

void ApplicationController::onSizeChanged(float width, float height, DeviceOrientation orientation) {
	LOGD("ApplicationController::onSizeChanged(%f, %f, %d)", width, height, orientation);
	screenSize.width = width;
	screenSize.height = height;
	aspect = width / height;
	main->onSizeChanged(width, height, orientation);
}

void ApplicationController::onLowMemory(void) {
	LOGD("ApplicationController::onLowMemory()");
	main->onLowMemory();
}

void ApplicationController::onUpdate(float dt) {
//	LOGD("ApplicationController::onUpdate(%f)", dt);
	// mainに通知
	main->onUpdate(dt);
	// アクティブシーンに通知
	if (activeScene) {
		activeScene->onUpdate(dt);
	}
}

void ApplicationController::onDraw() {
//	LOGD("ApplicationController::onDraw()");
	// 全体の背景色
	glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_SCISSOR_TEST);
	// mainに通知
	main->onDraw();
	if (activeScene) {
		// 各ビューを描画
		std::vector<View_ptr>::iterator it = viewArray.begin();
		while (it != viewArray.end()) {
			View_ptr v = (View_ptr) *it;
			// ビューの背景色
			glClearColor(v->bgColor.r, v->bgColor.g, v->bgColor.b, v->bgColor.a);
			// ビューの座標系を変換
			Rectf rect;
			if (v->isFullScreen) {
				rect = Rectf(0, 0, screenSize.width, screenSize.height);
				v->frame = Rectf(0, 0, 2, 2);
			} else {
				float w = v->frame.size.width / 2.0 * screenSize.width;
				float h = v->frame.size.height / 2.0 * screenSize.height;
				float x = (v->frame.location.x + 1.0) / 2.0 * screenSize.width - w / 2.0;
				float y = (v->frame.location.y + 1.0) / 2.0 * screenSize.height - h / 2.0;
				rect = Rectf(x, y, w, h);
			}
			// クリップ
			glScissor(rect.location.x, rect.location.y, rect.size.width, rect.size.height);
			glViewport(rect.location.x, rect.location.y, rect.size.width, rect.size.height);
			glClear(GL_COLOR_BUFFER_BIT);
			// 描画
			if (v->camera) {
				v->camera->aspect = v->frame.size.width / v->frame.size.height * aspect;
				v->camera->updateProjectionMatrix();
				activeScene->onDraw(*v.get());
			}
			it++;
		}
	}
}
	
// タッチイベント
void ApplicationController::onTouch(TouchAction action, float x, float y, long id, long time) {
	LOGD("***********onTouch[%d](%f,%f)[%d] %u", action, x, y, id, time);
	main->onTouch(action, x, y, id, time);
}

// 傾きセンサイベント
void ApplicationController::onOrientationChanged(float yaw, float pitch, float roll) {
	LOGD("***********onOrientationChanged(%f, %f, %f)", RADIANS_TO_DEGREES(yaw), RADIANS_TO_DEGREES(pitch), RADIANS_TO_DEGREES(roll));
	main->onOrientationChanged(yaw, pitch, roll);
}

// ユーザイベント
int ApplicationController::onUserEvent(int type, int param1, long long param2, float param3, double param4, const char *param5) {
	//LOGD("onUserEvent(t: %d, 1: %d, 2: %lld, 3:%f, 4:%f, 5:%s)", type, param1, param2, param3, param4, param5);
	return main->onUserEvent(type, param1, param2, param3, param4, param5);
}

// デバッグコマンド
void ApplicationController::onDebugCommand(const char *command, int param) {
	//LOGD("onDebugCommand(%s, %d)", command, param);
	main->onDebugCommand(command, param);
}
