#include "Topic.h"

#include <algorithm>
#include <random>

Topic::Topic()
{
}


Topic::~Topic()
{
}

// �p����ǉ�����
void Topic::addSentences(std::string sentence) {
	mSentences.push_back(sentence);
}
// �P���ǉ�����
void Topic::addWords(std::string word) {
	mWords.push_back(word);
	mCorrectWords.push_back(word);
}
// ���{����ǉ�����
void Topic::addJapaneses(std::string japanese) {
	mJapaneses.push_back(japanese);
}

// �P����V���b�t������
void Topic::shuffleWords() {
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(mWords.begin(), mWords.end(), engine);
}

// �p�����擾����
std::string Topic::getSentence(int index) {
	if (index < mSentences.size()) {
		return mSentences[index];
	}
	else {
		return "";
	}
}
// �P����擾����
std::string Topic::getWord(int index) {
	if (index < mWords.size()) {
		return mWords[index];
	}
	else {
		return "";
	}
}
// ���{�����擾����
std::string Topic::getJapanese(int index) {
	if (index < mJapaneses.size()) {
		return mJapaneses[index];
	}
	else {
		return "";
	}
}
// �p�������擾����
int Topic::getSentenceCount() {
	return mSentences.size();
}
// �P�ꐔ���擾����
int Topic::getWordCount() {
	return mWords.size();
}
// �P��̌��̐����擾����
int Topic::getSentenceHoleCount(int page, std::string pattern) {
	return 0;
}
// ���𐔂��擾����
int Topic::correctAnswerNum(std::vector <std::string> answer) {
	int result = 0;
	for (int i = 0; i < mCorrectWords.size();i++) {
		if (answer[i] == mCorrectWords[i]) {
			result++;
		}
	}

	return result;

}
