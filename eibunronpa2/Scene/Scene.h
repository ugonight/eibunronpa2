#pragma once

#include<vector>
#include<memory>
#include<string>
#include "../Control.h"

// class Control;
class Parts;

class Scene
{
private:
	Control *mControl;
	std::vector<std::shared_ptr<Parts>> mPartsList;
	bool mRemoveAllParts;
	std::string mRemovePartsName;
	bool mDoPopScene;

protected:
	void pushScene(Scene* scene);
	void popScene();
	void addParts(Parts* parts);
	void addParts(std::shared_ptr<Parts> parts);
	Parts* getPartsByName(std::string name);
	void removeAllParts();
	void removePartsByName(std::string name);

public:
	Scene();
	virtual ~Scene();

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;

	void initAll();
	void updateAll();
	void drawAll();

	void setControl(Control *control);
};

