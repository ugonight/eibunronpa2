#include "Scene.h"
#include "../Control.h"
#include "../Parts/Parts.h"

#include<string>

Scene::Scene() : mRemoveAllParts(false), mRemovePartsName(""), mDoPopScene(false) {}
Scene::~Scene() {

}

//template<class TScene>
//void Scene::pushScene(TScene* scene) {
//	mControl->pushScene(scene);
//}

void Scene::setControl(Control *control) {
	mControl = control;
}

void Scene::initAll() {
	init();
}

void Scene::updateAll() {
	update();
	//for (auto parts : mPartsList) {
	//	parts->update();
	//}
	for (int i = 0;i < mPartsList.size();i++) {
		mPartsList[i]->update();
	}

	if (mRemoveAllParts) {
		mPartsList.clear();
		mRemoveAllParts = false;
	}

	if (mRemovePartsName != "") {
		int i = 0;
		for (auto parts : mPartsList) {
			std::string name = parts->getName();
			if (name.find(mRemovePartsName) != std::string::npos) {
				mPartsList.erase(mPartsList.begin() + i);
				break;
			}
			i++;
		}
	}

	if (mDoPopScene) {
		mControl->popScene();
	}
}

void Scene::drawAll() {
	draw();
	for (auto parts : mPartsList) {
		parts->draw();
	}
}

void Scene::addParts(Parts* parts) {
	mPartsList.push_back(std::shared_ptr<Parts>(parts));
	// mPartsList.push_back(std::make_shared<Parts>(parts));
}

void Scene::addParts(std::shared_ptr<Parts> parts) {
	mPartsList.push_back(parts);
}

Parts* Scene::getPartsByName(std::string name) {
	for (auto parts : mPartsList)
	{
		if (parts->getName() == name) {
			return parts.get();
		}
	}
	return nullptr;
}

void Scene::pushScene(Scene* scene) {
	mControl->pushScene(scene);
}

void Scene::popScene() {
	// mControl->popScene();
	mDoPopScene = true;
}

void Scene::removeAllParts() {
	// mPartsList.clear();
	mRemoveAllParts = true;
}

void Scene::removePartsByName(std::string name)
{
	mRemovePartsName = name;
}