#pragma once

#include "Scene.h"

#include <vector>
#include <memory>

class Parts;

class Title :
	public Scene
{
private:
	std::vector<std::string> mFileList;
	int mFHDescription, mFHFileName;

#pragma region ×‚©‚È•Ï”
	int mTime;
	int mSelectIndex;
	float mOpacity;
	bool mToplay;
#pragma endregion

public:
	Title();
	virtual ~Title();

	void init() override;
	void update() override;
	void draw() override;
};

