#include "Game.h"
#include "../Input.h"
#include "../FileReader.h"
#include "../Entity/Topic.h"
#include "../Parts/Button.h"
#include "../Parts/FuncDraw.h"
#include "../Parts/Text.h"
#include "../Parts/SentenceText.h"
#include "../Utils.h"
#include "../FileReader.h"
#include "DxLib.h"

#include <string>
#include <regex>
#include <algorithm>
#include <functional>
#include <Windows.h>

Game::Game() : mOpacity(0), mReadHowto(false), mCurrentTopicNum(0), mNextSequence(false), mCurrentSentenceNum(0), mCurrentSequenceNum(0), mTransitionResult(false), mTransitionTop(false), mStopSentence(false)
{
	mGHhowto = LoadGraph("images/howtoplay.png");
	mGHWordsFrame = LoadGraph("images/wordsFrame.png");
	mGHLeft = LoadGraph("images/left.png");
	mGHRight = LoadGraph("images/right.png");

	mFHTopicTitle = CreateFontToHandle("ＭＳ Ｐゴシック", 40, 3, DX_FONTTYPE_ANTIALIASING);
	mFHWords = CreateFontToHandle("ＭＳ Ｐゴシック", 40, 3, DX_FONTTYPE_ANTIALIASING);
	mFHJapanese = CreateFontToHandle("ＭＳ Ｐゴシック", 30, 2, DX_FONTTYPE_ANTIALIASING);
	mFHStatus = CreateFontToHandle("ＭＳ Ｐゴシック", 20, 2, DX_FONTTYPE_ANTIALIASING);
	mFHResult = CreateFontToHandle("ＭＳ Ｐゴシック", 60, 3, DX_FONTTYPE_ANTIALIASING);


	// SHWords = MakeScreen(570, 60, FALSE);
}


Game::~Game()
{
	DeleteGraph(mGHhowto);
	DeleteGraph(mGHWordsFrame);
	DeleteGraph(mGHLeft);
	DeleteGraph(mGHRight);
	DeleteFontToHandle(mFHTopicTitle);
	DeleteFontToHandle(mFHWords);
	DeleteFontToHandle(mFHJapanese);
	DeleteFontToHandle(mFHStatus);
	DeleteFontToHandle(mFHResult);
	// DeleteGraph(SHWords);
}

void Game::init() {
	// ファイル読み込み
	mTopicList = FileReader::LoadTopic(mFilePath);

#pragma region 操作説明
	mSequenceList.push_back(std::make_tuple(
		[this]() {
		// 初期化
	},
		[this]() {
		// 更新

		if (Input::getMouseClickL() || Input::getKeyPush(KEY_INPUT_RETURN)) {
			mReadHowto = true;
		}
		if (mReadHowto) {
			if (mOpacity > 0) {
				mOpacity -= 255.0 / 30.0;
			}
			else {
				// mSequenceList.erase(mSequenceList.begin());
				nextSequence();
			}
		}
		else {
			if (mOpacity < 255) {
				mOpacity += 255.0 / 30.0;
			}
		}
	}, [this]() {
		// 描画

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, mOpacity);
		DrawGraph(0, 0, mGHhowto, TRUE);
	}));
#pragma endregion

#pragma region 単語一覧確認
	mSequenceList.push_back(std::make_tuple([this]() {
		// 初期化

		// 単語一覧フレーム
		{
			auto button = new Button(400, 300, 400, 300);
			button->setLineTickness(2.0f);
			button->setName("listFrame");
			button->setColor(GetColor(255, 255, 255));
			button->setUpdateFunc([this](Button* button) {
				if (button->getState() == 0) {
					// 枠移動
					auto src = button->getRect();
					auto dst = std::make_tuple<float, float, float, float>(400 - 300, 300 - 200, 400 + 300, 300 + 200);

					if (std::get<0>(src) > std::get<0>(dst)) {
						std::get<0>(src) -= 300.0f / 60.0f * 2.0f;
					}
					if (std::get<1>(src) > std::get<1>(dst)) {
						std::get<1>(src) -= 200.0f / 60.0f * 2.0f;
					}
					if (std::get<2>(src) < std::get<2>(dst)) {
						std::get<2>(src) += 300.0f / 60.0f * 2.0f;
					}
					if (std::get<3>(src) < std::get<3>(dst)) {
						std::get<3>(src) += 200.0f / 60.0f * 2.0f;
					}

					button->setRect(src);

					if (mOpacity >= 255.0f) {
						button->setState(1);
					}
				}
				else if (button->getState() == 2) {
					// 枠移動
					auto src = button->getRect();
					auto dst = std::make_tuple<float, float, float, float>(400, 300, 400, 300);

					if (std::get<0>(src) < std::get<0>(dst)) {
						std::get<0>(src) += 300.0f / 60.0f * 2.0f;
					}
					if (std::get<1>(src) < std::get<1>(dst)) {
						std::get<1>(src) += 200.0f / 60.0f * 2.0f;
					}
					if (std::get<2>(src) > std::get<2>(dst)) {
						std::get<2>(src) -= 300.0f / 60.0f * 2.0f;
					}
					if (std::get<3>(src) > std::get<3>(dst)) {
						std::get<3>(src) -= 200.0f / 60.0f * 2.0f;
					}

					button->setRect(src);

					if (mOpacity <= 0) {
						removeAllParts();
						nextSequence();
					}
				}
			});
			button->setDrawFunc([this](Button* button) {
				// 線
				auto rect = button->getRect();
				float y = 300.0f - (300.0f - 155.0f) / (300.0f - 100.0f) * (300.f - std::get<1>(rect)); // 中央からの距離で何とか算出したい
				// y = 155.0f / 100.0f * std::get<1>(rect);
				DrawLineAA(std::get<0>(rect), y, std::get<2>(rect), y, GetColor(255, 255, 255), 2.0f);
			});
			button->setClickFunc([this](Button* button) {
				if (button->getState() == 1) {
					button->setState(2);
				}
			});
			addParts(button);
		}

		// 文字
		{
			// 単語一覧の文字列
			std::string line = "", word = "";
			mWordsString = "";
			word = mTopicList[mCurrentTopicNum]->getWord(0);
			for (int i = 0; !word.empty(); i++) {
				line += mTopicList[mCurrentTopicNum]->getWord(i) + "  ";
				if (GetDrawStringWidthToHandle(line.c_str(), line.size(), mFHWords) > 500) {
					mWordsString += line + "\n";
					line = "";
				}
				word = mTopicList[mCurrentTopicNum]->getWord(i);
			}
			mWordsString += line;

			// 文字
			mOpacity = 0.0f;
			auto topicName = new FuncDraw([this]() {
				auto button = getPartsByName("listFrame");
				int state = button->getState();

				if (state == 0) {
					if (mOpacity < 255.0f) {
						mOpacity += 255.0f / 60.0f;
					}
				}if (state == 2) {
					if (mOpacity > 0.0f) {
						mOpacity -= 255.0f / 60.0f;
					}
				}

			}, [this]() {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, mOpacity);
				Utils::DrawFormatStringCenteringToHandle(400, 110, GetColor(255, 255, 255), mFHTopicTitle, "Topic%d Words", mCurrentTopicNum + 1);
				DrawFormatStringToHandle(110, 160, GetColor(255, 255, 0), mFHWords, mWordsString.c_str());
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			});
			addParts(topicName);
		}


	}, [this]() {
		// 更新

	}, [this]() {
		// 描画

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}));
#pragma endregion

#pragma region ゲームスタート

	mSequenceList.push_back(std::make_tuple([this]() {
		// 初期化

		// 穴を作る
		mHoles.clear();
		for (int i = 0;i < mTopicList[mCurrentTopicNum]->getWordCount();i++) {
			mHoles.push_back("");
		}
		//mJapaneseX = 1200.0f;
		//mSentenceTime = 0;
		//// 5文字1秒くらいで表示する
		//mSentenceDstTime = mTopicList[mCurrentTopicNum]->getSentence(0).size() / 5.0f * 60.0f;
		//mSentenceFlowDirc = GetRand(3);
		mSelecedWord = "";
		mWordsPage = 0;

		// 単語群
		std::string word = "";
		int x = 190;
		word = mTopicList[mCurrentTopicNum]->getWord(0);
		mWordTextList.clear();
		for (int i = 0; !word.empty(); i++) {
			word = mTopicList[mCurrentTopicNum]->getWord(i);

			auto text = new Text(x, 515, word, mFHWords);
			text->setName("word" + i);
			text->setClickFunc([this](Text* text) {
				// 他のテキストの選択状態を初期化する
				for (auto txt : mWordTextList) {
					txt->setState(0);
					txt->setColor(GetColor(255, 255, 255));
				}
				// 自分を選択状態にする
				text->setState(1);
				text->setColor(GetColor(255, 255, 0));
				mSelecedWord = text->getText();
			});
			mWordTextList.push_back(text);

			// 入りきらないのは画面外に隠す
			if (x + text->getWidth() > 760) {
				text->setPosition(800, 600);
			}
			addParts(text);

			x += (int)text->getWidth() + mWordSpace;
		}
		// 切り替えボタン
		{
			auto button = new Button(165.0f, 510.0f, 190.0f, 570.0f);
			button->setName("left");
			button->setImage(mGHLeft);
			button->setLineTickness(0.0f);
			button->setClickFunc([this](Button* button) { mWordsPage--; syncWords(); });
			button->setPosition(800, 600); // 最初は隠しておく
			addParts(button);
			button = new Button(760.0f, 510.0f, 780.0f, 570.0f);
			button->setName("right");
			button->setImage(mGHRight);
			button->setLineTickness(0.0f);
			button->setClickFunc([this](Button* button) { mWordsPage++; syncWords(); });
			if (x < 760) {
				button->setPosition(800, 600); // おさまってるときは隠しておく
			}
			addParts(button);
		}

		// 英文
		std::string sentence = "";
		sentence = mTopicList[mCurrentTopicNum]->getSentence(0);
		mSentenceTextList.clear();
		for (int i = 0; !sentence.empty(); i++) {
			sentence = mTopicList[mCurrentTopicNum]->getSentence(i);

			auto text = new SentenceText(sentence, mFHWords);
			text->setName("sentence" + std::to_string(i));
			// ターゲットがクリックされたら
			text->setClickTargetFunc([this](SentenceText::WordText* target) {
				// ターゲットの文字列を設定する
				target->setTargetWord(mSelecedWord);

				// ターゲットのセット情報を記録する
				int num = 0;
				for (int i = 0; i < mCurrentSentenceNum; i++) {
					num += mSentenceTextList[i]->getTargetCount();
				}
				num += target->getTargetNum();
				mHoles[num] = mSelecedWord;
				mWordsPage = 0;
				syncWords();
			});
			text->setHoldTargetFunc([this](SentenceText::WordText* target) {
				target->setTargetWord("");

				// ターゲットのセット情報を記録する
				int num = 0;
				for (int i = 0; i < mCurrentSentenceNum; i++) {
					num += mSentenceTextList[i]->getTargetCount();
				}
				num += target->getTargetNum();
				mHoles[num] = "";
				mWordsPage = 0;
				syncWords();
			});
			auto shardText = std::shared_ptr<SentenceText>(text);
			mSentenceTextList.push_back(shardText);
			addParts(shardText);
		}

		nextSentence(0);

	}, [this]() {
		// 更新

		// 結果発表に遷移するとき
		if (mTransitionResult) {
			// フェードアウト
			mOpacity -= 255.0f / 30.0f;
			if (mOpacity <= 0) {
				mTransitionResult = false;
				nextSequence();
			}
		}
		else {
			// フェードイン
			if (mOpacity < 255) {
				mOpacity += 255.0 / 30.0;
			}
		}

		// 日本語訳移動
		if (mJapaneseX > 400) {
			mJapaneseX -= 800.0f / 30.0f;
		}
		// 英文の表示時間を過ぎたら
		else if (mSentenceTime > mSentenceDstTime)
		{
			mJapaneseX -= 800.0f / 30.0f;
		}

		// 英文移動
		if (!mStopSentence) {
			auto sentenceRect = mSentenceTextList[mCurrentSentenceNum]->getPosition();
			switch (mSentenceFlowDirc) {
			case 0:
				// 下から上
				if (std::get<1>(sentenceRect) > 300.0f) {
					mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect), std::get<1>(sentenceRect) - 300.0f / 30.0f);
				}
				else if (std::get<1>(sentenceRect) <= 300.0f) {
					mSentenceTime++;
					mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect), std::get<1>(sentenceRect) - 0.5f);

					if (mSentenceTime > mSentenceDstTime)
					{
						mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect), std::get<1>(sentenceRect) - (300.0f - (0.5f * mSentenceDstTime)) / 30.0f);
					}
				}

				break;
			case 1:
				// 上から下
				if (std::get<1>(sentenceRect) < 100.0f) {
					mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect), std::get<1>(sentenceRect) + 100.0f / 30.0f);
				}
				else if (std::get<1>(sentenceRect) >= 100.0f) {
					mSentenceTime++;
					mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect), std::get<1>(sentenceRect) + 0.5f);

					if (mSentenceTime > mSentenceDstTime)
					{
						mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect), std::get<1>(sentenceRect) + (600.0f - (0.5f * mSentenceDstTime + 100.0f)) / 30.0f);
					}
				}
				break;
			case 2: {
				// 右から左

				int dstX = 800.0f - mSentenceTextList[mCurrentSentenceNum]->getWidth();
				if (std::get<0>(sentenceRect) > dstX) {
					mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect) - mSentenceTextList[mCurrentSentenceNum]->getWidth() / 30.0f, std::get<1>(sentenceRect));
				}
				else if (std::get<0>(sentenceRect) <= dstX) {
					mSentenceTime++;
					mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect) - 0.5f, std::get<1>(sentenceRect));

					if (mSentenceTime > mSentenceDstTime)
					{
						mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect) - (800.0f - (0.5f * mSentenceDstTime + dstX)) / 30.0f, std::get<1>(sentenceRect));
					}
				}

				break;
			}
			case 3: {
				// 左から右
				int dstX = 0;
				if (std::get<0>(sentenceRect) < dstX) {
					mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect) + mSentenceTextList[mCurrentSentenceNum]->getWidth() / 30.0f, std::get<1>(sentenceRect));
				}
				else if (std::get<0>(sentenceRect) >= dstX) {
					mSentenceTime++;
					mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect) + 0.5f, std::get<1>(sentenceRect));

					if (mSentenceTime > mSentenceDstTime)
					{
						mSentenceTextList[mCurrentSentenceNum]->setPosition(std::get<0>(sentenceRect) + (800.0f - (0.5f * mSentenceDstTime + dstX)) / 30.0f, std::get<1>(sentenceRect));
					}
				}

				break;
			}
			}
		}

		// 次の英文
		if (mSentenceTime > mSentenceDstTime + 30.0f) {
			// たまに画面外に流れていかない時があるので調整
			mSentenceTextList[mCurrentSentenceNum]->setPosition(800, 600);

			mCurrentSentenceNum++;

			if (mCurrentSentenceNum >= mTopicList[mCurrentTopicNum]->getSentenceCount()) {
				// 穴が全部埋まっているか
				if (std::find(mHoles.begin(), mHoles.end(), "") == mHoles.end()) {
					mCurrentSentenceNum--;
					mTransitionResult = true;
				}
				else {
					mCurrentSentenceNum = 0;
					nextSentence(mCurrentSentenceNum);
				}
			}
			else {
				nextSentence(mCurrentSentenceNum);
			}
		}

#pragma region キー操作
		// スペースキー
		if (Input::getKeyState(KEY_INPUT_SPACE)) {	
			// 英文を止める
			mStopSentence = true;
		}
		else {
			mStopSentence = false;
		}

		// Ctrlキー
		if (Input::getKeyPush(KEY_INPUT_LCONTROL) || Input::getKeyPush(KEY_INPUT_RCONTROL)) { 
			// 文をスキップする
			mSentenceTextList[mCurrentSentenceNum]->setPosition(800, 600);
			++mCurrentSentenceNum;
			if (mCurrentSentenceNum >= mTopicList[mCurrentTopicNum]->getSentenceCount()) {
				mCurrentSentenceNum = 0;
			}
			nextSentence(mCurrentSentenceNum);
		}

#pragma endregion

	}, [this]() {
		// 描画

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, mOpacity);
		// 単語群枠
		DrawGraph(0, 0, mGHWordsFrame, TRUE);

		// 文字群
		// SetDrawScreen(SHWords);
		// DrawFormatStringToHandle(190, 515, GetColor(255, 255, 0), mFHWords, mWordsString.c_str());
		// SetDrawScreen(DX_SCREEN_BACK);

		// 文字群スクリーン
		// DrawGraph(190, 510, SHWords, TRUE);

		// 日本語
		std::string japanese = newLineString(mTopicList[mCurrentTopicNum]->getJapanese(mCurrentSentenceNum), mFHJapanese, 750);
		Utils::DrawFormatStringCenteringToHandle(mJapaneseX, 400, GetColor(255, 255, 255), mFHJapanese, japanese.c_str());

		// ステータス表示
		DrawFormatStringToHandle(10, 10, GetColor(255, 255, 255), mFHStatus, "Topic %d \nSentence %d / %d \nTarget %d / %d",
			mCurrentTopicNum + 1,
			mCurrentSentenceNum + 1, mTopicList[mCurrentTopicNum]->getSentenceCount(),
			std::count_if(mHoles.begin(), mHoles.end(), [](std::string hole) { return hole != ""; }), mHoles.size());
	}));

#pragma endregion

#pragma region 結果発表
	mSequenceList.push_back(std::make_tuple([this]() {
		// 初期化
		if (getPartsByName("buttonNext") == nullptr) {
			auto button = new Button(400 - 120, 400 - 50, 400 + 120, 400 + 50);
			button->setLineTickness(2.0f);
			button->setName("buttonNext");
			button->setClickFunc(std::bind(&Game::nextTopic, this));
			button->setText("");
			addParts(button);
		}
		else {
			auto button = ((Button*)getPartsByName("buttonNext"));
			button->setPosition(400 - 120, 400 - 50);
			button->setText("");
		}

	}, [this]() {
		// 更新

		if (mTransitionTop) {
			// フェードアウト
			if (mOpacity > 0.0f) {
				mOpacity -= 255.0 / 60.0;
			}
			else {
				popScene();
			}
		}
		else {
			// フェードイン
			if (mOpacity < 255) {
				mOpacity += 255.0 / 60.0;
			}
			else {
				std::string text = "";
				if (mTopicList[mCurrentTopicNum]->getWordCount() == mTopicList[mCurrentTopicNum]->correctAnswerNum(mHoles)) {
					if (mCurrentTopicNum == mTopicList.size() - 1) {
						text = "トップへ";
					}
					else {
						text = "次へ";
					}
				}
				else {
					text = "もう一度";
				}

				((Button*)getPartsByName("buttonNext"))->setText(text, mFHResult);
			}
		}
	}, [this]() {
		// 描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, mOpacity);

		int result = 0;
		if (mOpacity < 255 && !mTransitionTop) {
			result = GetRand(mTopicList[mCurrentTopicNum]->getWordCount());
		}
		else {
			result = mTopicList[mCurrentTopicNum]->correctAnswerNum(mHoles);
		}

		Utils::DrawFormatStringCenteringToHandle(300, 200, GetColor(255, 255, 255), mFHResult, "正解数 : ");
		Utils::DrawFormatStringCenteringToHandle(450, 200, GetColor(255, 255.0f / 2.0f, 255.0f / 2.0f), mFHResult, std::to_string(result).c_str());
		Utils::DrawFormatStringCenteringToHandle(500, 200, GetColor(255, 255, 255), mFHResult, ("  / " + std::to_string(mTopicList[mCurrentTopicNum]->getWordCount())).c_str());

	}));
#pragma endregion

	//mSequenceList.push_back(std::make_tuple([this]() {
	//	// 初期化
	//}, [this]() {
	//	// 更新
	//}, [this]() {
	//	// 描画
	//}));

}

void Game::update() {
	if (mSequenceList.size() > 0) {
		// 次のシーケンスの初期化
		if (mNextSequence) {
			std::get<0>(mSequenceList[mCurrentSequenceNum])();
			mNextSequence = false;
		}

		std::get<1>(mSequenceList[mCurrentSequenceNum])();
	}
}

void Game::draw() {
	if (mSequenceList.size() > 0)
		std::get<2>(mSequenceList[mCurrentSequenceNum])();
}

void Game::setFilePath(std::string filePath)
{
	mFilePath = filePath;
}

void Game::nextSequence() {
	// mSequenceList.erase(mSequenceList.begin());
	mCurrentSequenceNum++;

	mNextSequence = true;
}

void Game::nextSentence(int next) {
	mJapaneseX = 1200.0f;
	mSentenceTime = 0;
	// x文字1秒くらいで表示する
	mSentenceDstTime = mTopicList[mCurrentTopicNum]->getSentence(next).size() / 7.0f * 60.0f;
	mSentenceFlowDirc = GetRand(3);

	if (next > 0) {
		// リソースが解放されちゃうし、別に消さなくてよくね？って感じなのでコメントアウト
		// removePartsByName("sentence" + std::to_string(next - 1));
	}

	auto text = mSentenceTextList[next];

	switch (mSentenceFlowDirc) {
	case 0:
		// 下から上
		text->setCenterPosition(400, 600 + 20);
		break;
	case 1:
		// 上から下
		text->setCenterPosition(400, 0 - 20);
		break;
	case 2:
		// 右から左
		text->setCenterPosition(800 + text->getWidth() / 2.0f, GetRand(250) + 50);
		break;
	case 3:
		// 左から右
		text->setCenterPosition(0 - text->getWidth() / 2.0f, GetRand(250) + 50);
		break;
	}

	// 無かったら画面に追加
	// addParts(text.get());
}

void Game::nextTopic() {
	if (mTopicList[mCurrentTopicNum]->getWordCount() == mTopicList[mCurrentTopicNum]->correctAnswerNum(mHoles)) {
		if (mCurrentTopicNum == mTopicList.size() - 1) {
			// トップに戻る
			// popScene();
			mTransitionTop = true;
		}
		else {
			// 次のトピック
			removeAllParts();
			mCurrentTopicNum++;
			mCurrentSequenceNum -= 2;	// 単語一覧に戻る
			mNextSequence = true;
			mTransitionResult = false;
			mCurrentSentenceNum = 0;
		}
	}
	else {
		// もう一度
		mCurrentSequenceNum--;
		((Button*)getPartsByName("buttonNext"))->setPosition(800, 600);
		mTransitionResult = false;
		mCurrentSentenceNum = 0;
		nextSentence(mCurrentSentenceNum);
	}

}

void Game::syncWords() {
	int x = 0;
	// int x = 190;

	std::vector<std::vector<Text*>> lines;
	std::vector<Text*> line;

	for (auto text : mWordTextList) {
		if (std::find(mHoles.begin(), mHoles.end(), text->getText()) == mHoles.end()) {

			x += (int)text->getWidth() + mWordSpace;
			if (x > 760 - 190) {
				lines.push_back(line);
				std::vector<Text*> newline;
				line = newline;
				x = 0;
			}

			line.push_back(text);
			text->setPosition(800, 600); // とりあえず全部外に出す

			//text->setPosition(x, 515);

			//// 入りきらないのは画面外に隠す
			//if (x + text->getWidth() > 760) {
			//	text->setPosition(800, 600);
			//	continue;
			//}

			//x += (int)text->getWidth() + mWordSpace;
		}
		else {
			// 使われているのは画面外に隠す
			text->setPosition(800, 600);
		}
		text->setState(0);
		text->setColor(GetColor(255, 255, 255));
		mSelecedWord = "";
	}
	lines.push_back(line);

	// ページ内のテキストを画面に出す
	x = 0;
	int num = 0;
	for (auto text : lines[mWordsPage]) {
		text->setPosition(190 + x, 515);
		x += (int)text->getWidth() + mWordSpace;

		// 最初の単語を選択しておく
		if (num == 0) {
			text->setState(1);
			text->setColor(GetColor(255, 255, 0));
			mSelecedWord = text->getText();
		}
		num++;
	}

	// ボタン表示・非表示
	auto left = (Button*)getPartsByName("left");
	auto right = (Button*)getPartsByName("right");
	if (mWordsPage == 0) {
		left->setPosition(800, 600);
	}
	else {
		left->setPosition(165, 510);
	}
	if (mWordsPage >= lines.size() - 1 || lines.size() == 1) {
		right->setPosition(800, 600);
	}
	else {
		right->setPosition(760, 510);
	}

}

std::string Game::newLineString(std::string source, int fontHandle, int maxWidth) {
	std::string str = source;
	std::string result = "";
	std::string line = "";

	while (!str.empty())
	{
		if (IsDBCSLeadByte(str[0]) == 0)
		{
			line += str.substr(0, 1);
			str.erase(0, 1);
		}
		else
		{
			line += str.substr(0, 2);
			str.erase(0, 2);
		}

		if (GetDrawStringWidthToHandle(line.c_str(), line.size(), fontHandle) > maxWidth) {
			result += line + "\n";
			line = "";
		}
	}
	result += line;

	return result;
}
