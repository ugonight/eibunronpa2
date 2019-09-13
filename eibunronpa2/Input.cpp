#include "Input.h"

#include <DxLib.h>

Input* Input::instance = nullptr;

int Input::mMouseX = 0;
int Input::mMouseY = 0;
bool Input::mMouseClickL = false;
bool Input::mMouseClickR = false;
bool Input::mMouseLeaveL = false;
bool Input::mMouseLeaveR = false;
bool Input::mMouseInputL = false;
bool Input::mMouseInputR = false;
char* Input::mKeyStateBuf;
bool* Input::mKeyPushBuf;

Input& Input::get_instance()
{
	return *instance;
}

void Input::create()
{
	if (!instance) {
		instance = new Input;
	}

	mKeyStateBuf = new char[256];
	mKeyPushBuf = new bool[256];
}

void Input::destroy()
{
	delete instance;
	instance = nullptr;

	delete[] mKeyStateBuf;
	delete[] mKeyPushBuf;
}

void Input::update() {
	GetMousePoint(&mMouseX, &mMouseY);

	bool inputL = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0, inputR = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;

	//  クリック判定
	mMouseClickL = (!mMouseInputL && inputL);
	mMouseClickR = (!mMouseInputR && inputR);

	mMouseLeaveL = (mMouseInputL && !inputL);
	mMouseLeaveR = (mMouseInputR && !inputR);

	mMouseInputL = inputL;
	mMouseInputR = inputR;

	// 	キーボードのすべてのキーの押下状態を取得する
	char Buf[256];
	GetHitKeyStateAll(Buf);

	// プッシュ判定
	for (int i = 0; i < 256; i++) {
		mKeyPushBuf[i] = !mKeyStateBuf[i] && Buf[i];
		mKeyStateBuf[i] = Buf[i];
	}
}

std::pair<int, int> Input::getMousePos() {
	return std::make_pair(mMouseX, mMouseY);
}

bool Input::getMouseClickL() {
	return mMouseClickL;
}

bool Input::getMouseClickR() {
	return mMouseClickR;
}

bool Input::getMouseInputL() {
	return mMouseInputL;
}

bool Input::getMouseInputR() {
	return mMouseInputR;
}

bool Input::getMouseLeaveL() {
	return mMouseLeaveL;
}

bool Input::getMouseLeaveR() {
	return mMouseLeaveR;
}

int Input::getKeyState(int key) {
	if (key >= 0 && key < 256) {
		return mKeyStateBuf[key];
	}
	else {
		return -1;
	}
}

bool Input::getKeyPush(int key) {
	if (key >= 0 && key < 256) {
		return mKeyPushBuf[key];
	}
	else {
		return -1;
	}
}