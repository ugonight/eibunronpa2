#pragma once

#include "Scene.h"
#include <functional>
#include <tuple>
#include <map>

class Parts;
class Topic;
class Text;
class Button;
class SentenceText;

class Game :
	public Scene
{
private:
	// �s���������E�X�V�E�`�揈��
	std::vector<std::tuple<std::function<void()>, std::function<void()>, std::function<void()>>> mSequenceList;

	std::vector<Topic*> mTopicList;
	std::vector<std::string> mHoles;
	std::string mFilePath;
	int mCurrentSequenceNum;

	std::vector<Text*> mWordTextList;
	std::vector<std::shared_ptr<SentenceText>> mSentenceTextList;
	//std::vector<Text*> mSentenceTextList;
	//std::vector<std::vector<Text*>> mHoleTextList;
	std::string mSelecedWord;

#pragma region �ׂ����ϐ�
	int mGHhowto, mGHWordsFrame, mGHLeft, mGHRight;
	int mFHTopicTitle, mFHWords, mFHJapanese, mFHStatus, mFHResult;
	// int SHWords;

	float mOpacity;
	bool mReadHowto;
	int mCurrentTopicNum, mCurrentSentenceNum;
	std::string mWordsString;
	float mJapaneseX;
	int mSentenceTime, mSentenceDstTime;
	int mSentenceFlowDirc;
	bool mStopSentence;

	const int mWordSpace = 20;
	int mWordsPage;

	bool mNextSequence;
	bool mTransitionResult, mTransitionTop;

#pragma endregion

	void nextSequence();
	void nextSentence(int next);
	void nextTopic();
	void syncWords();	// �P��Q�̕\�����X�V����
	std::string newLineString(std::string source, int fontHandle, int maxWidth); // �w�蕝�ɍ��킹�ĉ��s������������擾����

public:
	Game();
	~Game();

	void init() override;
	void update() override;
	void draw() override;

	void setFilePath(std::string filePath);
};

