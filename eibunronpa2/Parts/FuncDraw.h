#pragma once
#include "Parts.h"

#include <functional>

class FuncDraw :
	public Parts
{
	std::function<void()> mUpdateFunc;
	std::function<void()> mDrawFunc;

public:
	FuncDraw(std::function<void()> updateFunc, std::function<void()> drawFunc);
	virtual ~FuncDraw();

	void update() override;
	void draw() override;

};

