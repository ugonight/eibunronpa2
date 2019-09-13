#include "Text.h"

#include<DxLib.h>
#include "../Input.h"

Text::Text(int x, int y, std::string text, int fontHandle) : x1(x), y1(y), mText(text), mFontHandle(fontHandle), mUpdateFunc(nullptr), mDrawFunc(nullptr), mClickFunc(nullptr)
{
	w = GetDrawStringWidthToHandle(text.c_str(), text.length(), mFontHandle);
	h = GetFontSizeToHandle(mFontHandle);
	x2 = x1 + w;
	y2 = y1 + h;

	mColor = GetColor(255, 255, 255);
}


Text::~Text()
{
}

void Text::update() {
	if (mUpdateFunc != nullptr)
		mUpdateFunc(this);

	if (Input::getMouseClickL() && containPoint(Input::getMousePos().first, Input::getMousePos().second) && mClickFunc != nullptr)
		mClickFunc(this);
}

void Text::draw() {
	DrawFormatStringToHandle(x1, y1, mColor, mFontHandle, mText.c_str());

	if (mDrawFunc != nullptr)
		mDrawFunc(this);
}

void Text::setColor(int color) {
	mColor = color;
}

void Text::setText(std::string text, int fontHandle) {
	mText = text;
	mFontHandle = fontHandle;
	w = GetDrawStringWidthToHandle(mText.c_str(), mText.length(), mFontHandle);
}

void Text::setText(std::string text) {
	mText = text;
	w = GetDrawStringWidthToHandle(mText.c_str(), mText.length(), mFontHandle);
}

void Text::setUpdateFunc(std::function<void(Text*)> updateFunc) {
	mUpdateFunc = updateFunc;
}

void Text::setDrawFunc(std::function<void(Text*)> drawFunc) {
	mDrawFunc = drawFunc;
}

void Text::setClickFunc(std::function<void(Text*)> clickFunc) {
	mClickFunc = clickFunc;
}

std::tuple<float, float, float, float> Text::getRect() {
	return std::make_tuple(x1, y1, x2, y2);
}

std::string Text::getText() {
	return mText;
}

bool Text::containPoint(int x, int y) {
	return x >= x1 && x <= x2 && y >= y1 && y <= y2;
}

bool  Text::containSubPoint(int x, int y, int start, int end) {
	// ‰üs‚ª‚¢‚­‚Â‚ ‚é‚©’²‚×‚é
	auto sub0 = mText.substr(0, start);
	int nc = 0;
	for (int i = 0;i < sub0.c_str()[i] != '\0';i++) {
		if (sub0.c_str()[i] != '\n') {
			nc++;
		}
	}

	auto sub1 = mText.substr(start, end);
	int w1 = GetDrawStringWidthToHandle(sub0.c_str(), sub0.length(), mFontHandle);
	int w2 = GetDrawStringWidthToHandle(sub1.c_str(), sub1.length(), mFontHandle);
	int dx1 = x1 + w1, dy1 = y1 + h * nc, dx2 = x1 + w1 + w2, dy2 = y1 + h * (nc + 1);

	return x >= dx1 && x <= dx2 && y >= dy1 && y <= dy2;
}

void Text::setPosition(float x, float y) {
	x1 = x;
	y1 = y;
	// w = GetDrawStringWidthToHandle(mText.c_str(), mText.length(), mFontHandle);
	// h = GetFontSizeToHandle(mFontHandle);
	x2 = x1 + w;
	y2 = y1 + h;
}

void Text::setCenterPosition(float x, float y) {
	x1 = x - w / 2.0f;
	y1 = y - h / 2.0f;
	x2 = x1 + w;
	y2 = y1 + h;
}

float Text::getWidth() {
	return w;
}

float Text::getHeight() {
	return h;
}