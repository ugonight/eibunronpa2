#pragma once

#include<vector>
#include<string>

class Utils
{
public:
	Utils();
	~Utils();

	static std::vector<std::string> get_file_path_in_dir(const std::string& dir_name, const std::string& extension);
	static int DrawFormatStringCenteringToHandle(int x, int y, unsigned int Color, int FontHandle, const char *FormatString, ...);
};

