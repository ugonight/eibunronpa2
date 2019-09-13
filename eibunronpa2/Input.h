#pragma once

#include <utility>

class Input
{
private:
	Input() = default;
	~Input() = default;

	static Input* instance;

	static int mMouseX, mMouseY;
	static bool mMouseClickL, mMouseClickR;
	static bool mMouseInputL, mMouseInputR;
	static bool mMouseLeaveL, mMouseLeaveR;
	static char* mKeyStateBuf; 
	static bool* mKeyPushBuf;

public:
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(Input&&) = delete;

	static Input& get_instance();
	static void create();
	static void destroy();

	void update();

	static std::pair<int, int> getMousePos();
	static bool getMouseClickL();
	static bool getMouseClickR();
	static bool getMouseInputL();
	static bool getMouseInputR();
	static bool getMouseLeaveL();	// ƒ}ƒEƒX‚ª—£‚³‚ê‚½‚Æ‚«
	static bool getMouseLeaveR();
	static int getKeyState(int key);
	static bool getKeyPush(int key);
};

