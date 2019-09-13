#include "Button.h"
#include "../Utils.h"
#include "DxLib.h"
#include "../Input.h"

Button::Button(float x1, float y1, float x2, float y2)
	: x1(x1), x2(x2), y1(y1), y2(y2), mUpdateFunc(nullptr), mDrawFunc(nullptr), mClickFunc(nullptr)
{
	mColor = GetColor(255, 255, 255);
	mFillFlag = false;
	mLineTickness = 1.0f;
	mText = "";
	mFontHandle = -1;
	mImageHandle = -1;
}


Button::~Button()
{

}

void Button::update() {
	if (mUpdateFunc != nullptr)
		mUpdateFunc(this);

	if (Input::getMouseClickL() && containPoint(Input::getMousePos().first, Input::getMousePos().second) && mClickFunc != nullptr)
		mClickFunc(this);
}

void Button::draw() {
	if (mLineTickness > 0.0f)
		DrawBoxAA(x1, y1, x2, y2, mColor, mFillFlag, mLineTickness);

	if (mText != "" && mFontHandle > 0) {
		Utils::DrawFormatStringCenteringToHandle((x1 + x2) / 2.0f, (y1 + y2) / 2.0f - GetFontSizeToHandle(mFontHandle) / 2.0f, mColor, mFontHandle, mText.c_str());
	}

	if (mDrawFunc != nullptr)
		mDrawFunc(this);

	if (mImageHandle > 0) {
		int SizeX, SizeY;
		GetGraphSize(mImageHandle, &SizeX, &SizeY);
		DrawGraph((x1 + x2) / 2.0 - SizeX / 2.0, (y1 + y2) / 2.0 - SizeY / 2.0, mImageHandle, TRUE);
	}
}

void Button::setColor(int color) {
	mColor = color;
}

void Button::setFillFlag(bool fillFlag) {
	mFillFlag = fillFlag;
}

void Button::setLineTickness(float lineTickness) {
	mLineTickness = lineTickness;
}

void Button::setText(std::string text, int fontHandle) {
	mText = text;
	mFontHandle = fontHandle;
}

void Button::setText(std::string text) {
	mText = text;
}

void Button::setUpdateFunc(std::function<void(Button*)> updateFunc) {
	mUpdateFunc = updateFunc;
}

void Button::setDrawFunc(std::function<void(Button*)> drawFunc) {
	mDrawFunc = drawFunc;
}

void Button::setClickFunc(std::function<void(Button*)> clickFunc) {
	mClickFunc = clickFunc;
}

void Button::setRect(float x1, float y1, float x2, float y2) {
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
}

void Button::setRect(std::tuple<float, float, float, float> rect) {
	this->x1 = std::get<0>(rect);
	this->y1 = std::get<1>(rect);
	this->x2 = std::get<2>(rect);
	this->y2 = std::get<3>(rect);
}

void Button::setPosition(int x, int y) {
	int dx = x - x1, dy = y - y1;
	this->x1 += dx;
	this->y1 += dy;
	this->x2 += dx;
	this->y2 += dy;
}

std::tuple<float, float, float, float> Button::getRect() {
	return std::make_tuple(x1, y1, x2, y2);
}

bool Button::containPoint(int x, int y) {
	return x >= x1 && x <= x2 && y >= y1 && y <= y2;
}

void Button::setImage(int imageHandle) {
	mImageHandle = imageHandle;
}
