#include "Control.h"
#include "Scene/Scene.h"
#include "Scene/Title.h"
#include "Input.h"

#include <type_traits>

Control::Control()
{
	mScene = new std::vector<std::shared_ptr<Scene>>();
	Input::create();

	// 最初はタイトルを表示
	auto title = new Title();
	title->setControl(this);
	mScene->push_back(std::shared_ptr<Title>(title));
}

Control::~Control()
{
	//mScene->clear();
	//delete mScene;
	Input::destroy();
}

void Control::init() {

	mScene->back()->initAll();
}

void Control::update() {
	Input::get_instance().update();
	mScene->back()->updateAll();
}

void Control::draw() {
	mScene->back()->drawAll();
}

void Control::pushScene(Scene* scene)
{
	scene->setControl(this);
	scene->initAll();
	mScene->push_back(std::shared_ptr<Scene>(scene));
}

void Control::popScene() {
	mScene->pop_back();
}