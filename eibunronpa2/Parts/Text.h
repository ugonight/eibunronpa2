#pragma once
#include "Parts.h"

#include<functional>

class Text :
	public Parts
{
private:
	float x1, x2, y1, y2;
	float w, h;
	int mColor;
	std::string mText;
	int mFontHandle;

	std::function<void(Text*)> mUpdateFunc;
	std::function<void(Text*)> mDrawFunc;
	std::function<void(Text*)> mClickFunc;

public:
	Text(int x, int y, std::string text, int fontHandle);
	virtual ~Text();

	void update() override;
	void draw() override;

	void setColor(int color);
	void setText(std::string text, int fontHandle);
	void setText(std::string text);
	void setUpdateFunc(std::function<void(Text*)> updateFunc);
	void setDrawFunc(std::function<void(Text*)> drawFunc);
	void setClickFunc(std::function<void(Text*)> clickFunc);
	void setPosition(float x, float y);
	void setCenterPosition(float x, float y);

	std::tuple<float, float, float, float> getRect();
	float getWidth();
	float getHeight();
	std::string getText();

	bool containPoint(int x, int y);
	bool containSubPoint(int x, int y,int start,int end);

};

