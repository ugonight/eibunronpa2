#include "DxLib.h"
#include "Control.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetGraphMode(800, 600, 16);
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK); //ウィンドウモード変更と初期化と裏画面設定

	auto control = new Control();
	control->init();

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
		control->update();
		control->draw();
	}

	delete control;

	DxLib_End(); // DXライブラリ終了処理
	return 0;
}