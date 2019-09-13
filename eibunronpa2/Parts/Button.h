#pragma once

#include "Parts.h"
#include <string>
#include <functional>
#include <tuple>

class Button :
	public Parts
{
private:
	float x1, x2, y1, y2;
	int mColor;
	bool mFillFlag;
	float mLineTickness;
	std::string mText;
	int mFontHandle;
	int mImageHandle;

	std::function<void(Button*)> mUpdateFunc;
	std::function<void(Button*)> mDrawFunc;
	std::function<void(Button*)> mClickFunc;

public:
	Button(float x1, float y1, float x2, float y2);
	virtual ~Button();

	void update() override;
	void draw() override;

	void setColor(int color);
	void setFillFlag(bool fillFlag);
	void setLineTickness(float lineTickness);
	void setText(std::string text, int fontHandle);
	void setText(std::string text);
	void setUpdateFunc(std::function<void(Button*)> updateFunc);
	void setDrawFunc(std::function<void(Button*)> drawFunc);
	void setClickFunc(std::function<void(Button*)> clickFunc);
	void setRect(float x1, float y1, float x2, float y2);
	void setRect(std::tuple<float, float, float, float>);
	void setPosition(int x, int y);
	void setImage(int imageHandle);

	// 表示領域のタプルを取得（x1,y1,x2,y2）
	std::tuple<float, float, float, float> getRect();

	bool containPoint(int x, int y);
};

