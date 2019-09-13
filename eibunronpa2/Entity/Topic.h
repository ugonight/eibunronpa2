#pragma once

#include <vector>
#include <string>

class Topic
{
private:
	// �p���̃��X�g
	std::vector<std::string> mSentences;
	// �P��̃��X�g
	std::vector<std::string> mWords;
	// ���{���̃��X�g
	std::vector<std::string> mJapaneses;
	// �����̕��т̒P�ꃊ�X�g
	std::vector<std::string> mCorrectWords;

public:
	Topic();
	~Topic();

	// �p����ǉ�����
	void addSentences(std::string sentence);
	// �P���ǉ�����
	void addWords(std::string word);
	// ���{����ǉ�����
	void addJapaneses(std::string japanese);

	// �P����V���b�t������
	void shuffleWords();

	// �p�����擾����
	std::string getSentence(int index);
	// �P����擾����
	std::string getWord(int index);
	// ���{�����擾����
	std::string getJapanese(int index);
	// �p�������擾����
	int getSentenceCount();
	// �P�ꐔ���擾����
	int getWordCount();
	// �P��̌��̐����擾����
	int getSentenceHoleCount(int page, std::string pattern);
	// ���𐔂��擾����
	int correctAnswerNum(std::vector <std::string> answer);

};

