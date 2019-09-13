#pragma once

#include<string>

class Parts
{
protected:
	std::string mName;
	int mState;

public:	
	Parts();
	virtual ~Parts();

    virtual void update() = 0;
	virtual void draw() = 0;

	void setName(std::string name);
	void setState(int state);

	std::string getName();
	int getState();

};

