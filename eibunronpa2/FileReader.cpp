#include "FileReader.h"
#include "Entity/Topic.h"

#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include <DxLib.h>

const std::string FileReader::pattern = "\\[.+?\\]";

FileReader::FileReader()
{
}


FileReader::~FileReader()
{
}

std::vector<Topic*> FileReader::LoadTopic(std::string file) {
	std::vector<Topic*> topics;
	auto topic = new Topic();

	std::ifstream ifs(file);
	if (ifs.fail())
	{
		std::cerr << "失敗" << std::endl;
		return topics;
	}

	// ファイルの文字コードがSJIS以外だと文字化けする
	std::string str;
	while (getline(ifs, str)) {
		// 空行なら次のトピックへ
		if (str == "") {
			topic->shuffleWords();
			topics.push_back(topic);
			topic = new Topic();
			continue;
		}

		// 英文を追加する
		topic->addSentences(std::regex_replace(str, std::regex(pattern), "[----]"));

		// 単語を抜き出す
		std::smatch results;
		auto start = str.cbegin();

		while (std::regex_search(start, str.cend(), results, std::regex(pattern))) {
			auto result = results.str();
			topic->addWords(result.substr(1, result.size() - 2));
			start = results[0].second;
		}

		getline(ifs, str);
		topic->addJapaneses(str);
	}

	delete topic;
	return topics;
}