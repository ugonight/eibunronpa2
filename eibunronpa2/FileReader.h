#pragma once

#include <vector>
#include <string>

class Topic;

class FileReader
{
public:
	const static std::string pattern;
	
	FileReader();
	~FileReader();

	static std::vector<Topic*> LoadTopic(std::string file);
	
};

