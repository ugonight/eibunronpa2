#include "FuncDraw.h"

FuncDraw::FuncDraw(std::function<void()> updateFunc, std::function<void()> drawFunc)
{
	mUpdateFunc = updateFunc;
	mDrawFunc = drawFunc;
}


FuncDraw::~FuncDraw()
{
	// delete &mUpdateFunc;
	// delete &mDrawFunc;
}

void FuncDraw::update() {
	mUpdateFunc();
}

void FuncDraw::draw() {
	mDrawFunc();
}