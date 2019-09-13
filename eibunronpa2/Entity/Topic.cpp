#include "Topic.h"

#include <algorithm>
#include <random>

Topic::Topic()
{
}


Topic::~Topic()
{
}

// 英文を追加する
void Topic::addSentences(std::string sentence) {
	mSentences.push_back(sentence);
}
// 単語を追加する
void Topic::addWords(std::string word) {
	mWords.push_back(word);
	mCorrectWords.push_back(word);
}
// 日本語訳を追加する
void Topic::addJapaneses(std::string japanese) {
	mJapaneses.push_back(japanese);
}

// 単語をシャッフルする
void Topic::shuffleWords() {
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(mWords.begin(), mWords.end(), engine);
}

// 英文を取得する
std::string Topic::getSentence(int index) {
	if (index < mSentences.size()) {
		return mSentences[index];
	}
	else {
		return "";
	}
}
// 単語を取得する
std::string Topic::getWord(int index) {
	if (index < mWords.size()) {
		return mWords[index];
	}
	else {
		return "";
	}
}
// 日本語訳を取得する
std::string Topic::getJapanese(int index) {
	if (index < mJapaneses.size()) {
		return mJapaneses[index];
	}
	else {
		return "";
	}
}
// 英文数を取得する
int Topic::getSentenceCount() {
	return mSentences.size();
}
// 単語数を取得する
int Topic::getWordCount() {
	return mWords.size();
}
// 単語の穴の数を取得する
int Topic::getSentenceHoleCount(int page, std::string pattern) {
	return 0;
}
// 正解数を取得する
int Topic::correctAnswerNum(std::vector <std::string> answer) {
	int result = 0;
	for (int i = 0; i < mCorrectWords.size();i++) {
		if (answer[i] == mCorrectWords[i]) {
			result++;
		}
	}

	return result;

}
