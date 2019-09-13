#include "SentenceText.h"
#include "../FileReader.h"
#include "../Input.h"

#include <regex>
#include <DxLib.h>

SentenceText::SentenceText(std::string sentence, int fonthandle) : mSentence(sentence), mX(800), mY(600), mW(0), mH(0), mFontHandle(fonthandle)
{
	auto words = splitString(sentence, " ");
	int w = 0, h = 0;
	int targetNum = 0;

	for (auto word : words) {
		auto wordText = new WordText(0, 0, word + " ", fonthandle);
		mWordList.push_back(wordText);

		// 相対位置を調整
		if (w + wordText->getWidth() >= MAX_WIDTH) {
			h += wordText->getHeight();
			w = 0;
		}
		wordText->setRelativePosition(w, h);

		// 移動を全体の動きに合わせる
		wordText->setUpdateFunc([this](Text* text) {
			auto rp = ((WordText*)text)->getRelativePosition();
			text->setPosition(mX + rp.first, mY + rp.second);
		});

		// ターゲットがどうかを判定
		if (std::regex_match(word, std::regex(FileReader::pattern))) {
			// []の隣に,や.があると認識しない
			wordText->setIsTarget(true);
			wordText->setColor(GetColor(255, 255, 0));
			wordText->setTargetNum(targetNum++);
		}

		w += wordText->getWidth();
		if (w > mW) mW = w;
	}
	mH = h + GetFontSizeToHandle(fonthandle);
	mTargetCount = targetNum;
}


SentenceText::~SentenceText()
{

}

SentenceText::WordText::WordText(float x, float y, std::string text, int fontHandle)
	: Text(x, y, text, fontHandle), mIsTarget(false), mHoldTime(0) {

}

void SentenceText::WordText::setRelativePosition(float x, float y) {
	mRX = x;
	mRY = y;
}

void SentenceText::WordText::setIsTarget(bool istarget) {
	mIsTarget = istarget;
}

void SentenceText::WordText::setTargetWord(std::string word) {
	mTargetWord = word;
}

void SentenceText::WordText::setTargetNum(int num) {
	mTargetNum = num;
}

std::pair<float, float> SentenceText::WordText::getRelativePosition() {
	return std::make_pair(mRX, mRY);
}

bool SentenceText::WordText::getIsTarget() {
	return mIsTarget;
}

int SentenceText::WordText::getTargetNum() {
	return mTargetNum;
}

std::string SentenceText::WordText::getTargetWord() {
	return mTargetWord;
}

void SentenceText::update() {
	for (auto word : mWordList) {
		word->update();

		if (word->getIsTarget()) {
			// ターゲットのクリックアクション
			if (Input::getMouseLeaveL() && word->containPoint(Input::getMousePos().first, Input::getMousePos().second) && mClickTargetFunc != nullptr && word->mHoldTime < 500) {
				mClickTargetFunc(word);

				// ターゲットの表示を変える
				if (word->getTargetWord() != "") {
					word->setText("[" + word->getTargetWord() + "] ");
				}
				else {
					word->setText("[----] ");
				}
				syncWordsPos();
			}

			// ホールド
			if (Input::getMouseInputL() && word->containPoint(Input::getMousePos().first, Input::getMousePos().second) && mHoldTargetFunc != nullptr) {
				word->mHoldTime++;
				if (word->mHoldTime == 60) {
					mHoldTargetFunc(word);
					word->setText("[----] ");
					syncWordsPos();
				}
			}
			else {
				word->mHoldTime = 0;
			}
		}
	}
}

void SentenceText::draw() {
	for (auto word : mWordList) {
		word->draw();
	}
}

std::vector<std::string> SentenceText::splitString(std::string string, std::string separator) {
	auto separator_length = separator.length(); // 区切り文字の長さ

	auto list = std::vector<std::string>();

	if (separator_length == 0) {
		list.push_back(string);
	}
	else {
		auto offset = std::string::size_type(0);
		while (1) {
			auto pos = string.find(separator, offset);
			if (pos == std::string::npos) {
				list.push_back(string.substr(offset));
				break;
			}
			list.push_back(string.substr(offset, pos - offset));
			offset = pos + separator_length;
		}
	}

	return list;
}

void SentenceText::setPosition(float x, float y) {
	mX = x;
	mY = y;
}

void SentenceText::setCenterPosition(float x, float y) {
	mX = x - mW / 2.0;
	mY = y - mH / 2.0;
}

int SentenceText::getWidth() {
	return mW;
}

int SentenceText::getHeight() {
	return mH;
}

std::pair<float, float> SentenceText::getPosition() {
	return std::make_pair(mX, mY);
}

void SentenceText::setClickTargetFunc(std::function<void(WordText*)> clickFunc) {
	mClickTargetFunc = clickFunc;
}

void SentenceText::setHoldTargetFunc(std::function<void(WordText*)> holdFunc) {
	mHoldTargetFunc = holdFunc;
}

void SentenceText::syncWordsPos() {
	int w = 0, h = 0;

	for (auto wordText : mWordList) {
		// 相対位置を調整
		if (w + wordText->getWidth() >= MAX_WIDTH) {
			h += wordText->getHeight();
			w = 0;
		}
		wordText->setRelativePosition(w, h);

		w += wordText->getWidth();
		if (w > mW) mW = w;
	}
	mH = h + GetFontSizeToHandle(mFontHandle);
}

int SentenceText::getTargetCount() {
	return mTargetCount;
}