#include "Utils.h"

#include <Windows.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <DxLib.h>

Utils::Utils()
{
}


Utils::~Utils()
{
}

std::vector<std::string> Utils::get_file_path_in_dir(const std::string& dir_name, const std::string& extension) noexcept(false)
{
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;//defined at Windwos.h
	std::vector<std::string> file_names;

	//拡張子の設定
	std::string search_name = dir_name + "\\*." + extension;

	hFind = FindFirstFile(search_name.c_str(), &win32fd);

	if (hFind == INVALID_HANDLE_VALUE) {
		// throw std::runtime_error("file not found");
		return file_names;
	}

	do {
		if (win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		}
		else {
			file_names.push_back(win32fd.cFileName);
			// printf("%s\n", file_names.back().c_str());
		}
	} while (FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return file_names;
}

int Utils::DrawFormatStringCenteringToHandle(int x, int y, unsigned int Color, int FontHandle, const char *FormatString, ...)
{
	va_list VaList;
	char String[2048];	// 変換後の文字列を格納する配列
	int Width;

	// va_list の準備( 第二引数は引数『...』の直前の引数を記述します )
	va_start(VaList, FormatString);

	// 書式文字列 + 可変長引数リストから書式文字列に従って文字配列に変換
	vsprintf_s(String, sizeof(String), FormatString, VaList);

	// va_list の後始末
	va_end(VaList);

	// 文字列描画時の幅を取得
	Width = GetDrawStringWidthToHandle(String, strlen(String), FontHandle);

	// 指定されたx座標を文字列の中心x座標として描画
	// return DrawString(x - Width / 2, y, String, Color);
	return DrawStringToHandle(x - Width / 2, y, String, Color, FontHandle);
}
