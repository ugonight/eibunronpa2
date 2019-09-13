#include "Title.h"

#include "../Utils.h"
#include "../Input.h"
#include "../Parts/Parts.h"
#include "../Parts/FuncDraw.h"
#include "../Parts/Button.h"
#include "Game.h"

#include <Windows.h>
#include <DxLib.h>
#include <math.h>


Title::Title() : mTime(0), mSelectIndex(0), mOpacity(255.0f), mToplay(false)
{
	mFHDescription = CreateFontToHandle("ＭＳ Ｐゴシック", 40, 3, DX_FONTTYPE_ANTIALIASING);
	mFHFileName = CreateFontToHandle("ＭＳ Ｐゴシック", 30, 3, DX_FONTTYPE_EDGE, -1, 2);
}


Title::~Title()
{
	DeleteFontToHandle(mFHDescription);
	DeleteFontToHandle(mFHFileName);
}

void Title::init() {
	char cdir[255];
	GetCurrentDirectory(255, cdir);
	std::string cdirs = cdir;
	mFileList = Utils::get_file_path_in_dir(cdirs + "\\questions", "txt");

	int Cr = GetColor(255, 255, 255);

	// 描画するもの

	// 説明
	auto func = new FuncDraw([]() {

	}, [this, Cr]() {
		Utils::DrawFormatStringCenteringToHandle(400, 100, Cr, mFHDescription, "ファイルを選択してください。");
	});
	addParts(func);

	int fx1 = 400 - 300, fy1 = 200, fx2 = 400 + 300, fy2 = 200 + 300;

	// 選択
	Button* button = new Button(fx1, fy1, fx2, fy1 + 30 * (mSelectIndex + 1));
	button->setName("selectButton");
	button->setColor(Cr);
	button->setFillFlag(true);
	button->setUpdateFunc([this, fx1, fy1, fx2, fy2](Button* btn) {
		// 点滅
		float t = 2.0f * 3.14 / 100.0 * (float)mTime;
		int bright = sin(t) * 255.0f / 2.0f + 255.0f / 2.0f;
		// ((Button*)getPartsByName("selectButton"))->setColor(GetColor(bright, bright, bright));
		btn->setColor(GetColor(bright, bright, bright));

		// 選択移動
		auto rect = btn->getRect();
		float dsty = fy1 + 30 * (mSelectIndex);
		if (std::get<1>(rect) < dsty) {
			rect = std::make_tuple(std::get<0>(rect), std::get<1>(rect) + 2, std::get<2>(rect), std::get<3>(rect) + 2);
		}
		else if (std::get<1>(rect) > dsty) {
			rect = std::make_tuple(std::get<0>(rect), std::get<1>(rect) - 2, std::get<2>(rect), std::get<3>(rect) - 2);
		}
		btn->setRect(rect);
	});
	// mPartsList.push_back(std::shared_ptr<Button>(button));
	addParts(button);

	// 枠
	func = new FuncDraw([this, fx1, fy1, fx2, fy2]() {
		// mPartsList.push_back(std::make_shared<FuncDraw>([this, fx1, fy1, fx2, fy2]() {
			// 選択切り替え
			// クリック
		if (Input::getMouseInputL()) {
			int x = Input::getMousePos().first, y = Input::getMousePos().second;
			// 枠内にあるか
			if (x >= fx1 && x <= fx2 && y >= fy1 && y <= fy2) {
				int index = (int)((y - fy1) / 30.0);
				if (index < mFileList.size()) {
					mSelectIndex = index;
				}
			}
		}
		// キー
		if (Input::getKeyPush(KEY_INPUT_UP)) {
			if (mSelectIndex > 0) {
				mSelectIndex--;
			}
		}
		else if (Input::getKeyPush(KEY_INPUT_DOWN)) {
			if (mSelectIndex < mFileList.size() - 1) {
				mSelectIndex++;
			}
		}

	}, [this, Cr, fx1, fy1, fx2, fy2]() {
		DrawBoxAA(fx1, fy1 + 2, fx2, fy2 + 2, Cr, false, 2.0f);
	});
	addParts(func);

	// ファイル一覧
	int i = 0;
	for (auto file : mFileList) {

		// mPartsList.push_back(std::make_shared<FuncDraw>([]() {
		func = new FuncDraw([]() {

		}, [this, Cr, fx1, fy1, fx2, fy2, i, file]() {
			DrawStringToHandle(fx1 + 3, fy1 + 2 + 30 * i, file.c_str(), Cr, mFHFileName);
		});
		addParts(func);

		i++;
	}

	// 決定ボタン
	button = new Button(400 - 100, 520, 400 + 100, 520 + 50);
	button->setColor(Cr);
	button->setFillFlag(false);
	button->setLineTickness(2.0f);
	button->setText("決定", mFHDescription);
	button->setClickFunc([this](Button* button) {
		mToplay = true;
	});
	button->setUpdateFunc([this](Button*) {
		// ゲーム開始画面へ遷移
		if (mToplay) {
			if (mOpacity <= 0) {
				auto game = new Game();
				{
					char cdir[255];
					GetCurrentDirectory(255, cdir);
					std::string cdirs = cdir;
					game->setFilePath(cdirs + "\\questions\\" + mFileList[mSelectIndex]);
				}
				pushScene(game);
				mToplay = false;
				mOpacity = 255.0f;
			}
			else {
				mOpacity -= 255.0 / 60.0;
			}
		}
	});
	// mPartsList.push_back(std::shared_ptr<Button>(button));
	addParts(button);
}

void Title::update() {
	mTime++;
}

void Title::draw() {
	int Cr = GetColor(255, 255, 255);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, mOpacity);
}

