#pragma once

#include <vector>
#include <string>

class Topic
{
private:
	// 英文のリスト
	std::vector<std::string> mSentences;
	// 単語のリスト
	std::vector<std::string> mWords;
	// 日本語訳のリスト
	std::vector<std::string> mJapaneses;
	// 正解の並びの単語リスト
	std::vector<std::string> mCorrectWords;

public:
	Topic();
	~Topic();

	// 英文を追加する
	void addSentences(std::string sentence);
	// 単語を追加する
	void addWords(std::string word);
	// 日本語訳を追加する
	void addJapaneses(std::string japanese);

	// 単語をシャッフルする
	void shuffleWords();

	// 英文を取得する
	std::string getSentence(int index);
	// 単語を取得する
	std::string getWord(int index);
	// 日本語訳を取得する
	std::string getJapanese(int index);
	// 英文数を取得する
	int getSentenceCount();
	// 単語数を取得する
	int getWordCount();
	// 単語の穴の数を取得する
	int getSentenceHoleCount(int page, std::string pattern);
	// 正解数を取得する
	int correctAnswerNum(std::vector <std::string> answer);

};

