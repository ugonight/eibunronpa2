#include "Parts.h"


Parts::Parts() : mName(""), mState(0) {

}

Parts::~Parts() {

}

void Parts::setName(std::string name) {
	mName = name;
}

void Parts::setState(int state) {
	mState = state;
}

std::string Parts::getName() {
	return mName;
}

int Parts::getState() {
	return mState;
}