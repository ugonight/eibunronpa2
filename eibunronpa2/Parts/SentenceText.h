#pragma once
#include "Parts.h"
#include "Text.h"

#include <vector>

class Text;

// 英文一文当たりのテキスト
class SentenceText :
	public Parts
{
public:
	// 空白ごとのテキスト（穴も含める）
	class WordText : public Text
	{
		bool mIsTarget;
		float mRX, mRY;
		std::string mTargetWord = "";	// ターゲットに当てはめたワード
		int mTargetNum = -1;	// 文中のターゲット番号

	public:
		int mHoldTime;

		WordText(float x, float y, std::string text, int fontHandle);
		
		void setRelativePosition(float x, float y);
		void setIsTarget(bool istarget);
		void setTargetWord(std::string word);
		void setTargetNum(int num);

		std::pair<float, float> getRelativePosition();
		bool getIsTarget();
		int getTargetNum();
		std::string getTargetWord();
	};

private:
	const float MAX_WIDTH = 600.0;

	std::vector<WordText*> mWordList;

	float mX, mY;
	float mW, mH;
	int mFontHandle;
	std::string mSentence;
	std::function<void(WordText*)> mClickTargetFunc;
	std::function<void(WordText*)> mHoldTargetFunc;
	int mTargetCount;

	std::vector<std::string> splitString(std::string string, std::string separator);
	void syncWordsPos();

public:
	SentenceText(std::string sentence, int fonthandle);
	virtual ~SentenceText();

	void update() override;
	void draw() override;

	void setPosition(float x, float y);
	void setCenterPosition(float x, float y);
	void setClickTargetFunc(std::function<void(WordText*)> clickFunc);
	void setHoldTargetFunc(std::function<void(WordText*)> holdFunc);

	int getWidth();
	int getHeight();
	std::pair<float, float> getPosition();
	int getTargetCount();
};

