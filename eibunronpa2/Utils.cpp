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

	//�g���q�̐ݒ�
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
	char String[2048];	// �ϊ���̕�������i�[����z��
	int Width;

	// va_list �̏���( �������͈����w...�x�̒��O�̈������L�q���܂� )
	va_start(VaList, FormatString);

	// ���������� + �ϒ��������X�g���珑��������ɏ]���ĕ����z��ɕϊ�
	vsprintf_s(String, sizeof(String), FormatString, VaList);

	// va_list �̌�n��
	va_end(VaList);

	// ������`�掞�̕����擾
	Width = GetDrawStringWidthToHandle(String, strlen(String), FontHandle);

	// �w�肳�ꂽx���W�𕶎���̒��Sx���W�Ƃ��ĕ`��
	// return DrawString(x - Width / 2, y, String, Color);
	return DrawStringToHandle(x - Width / 2, y, String, Color, FontHandle);
}
