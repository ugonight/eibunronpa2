#pragma once

#include <vector>
#include<memory>

class Scene;

// シーンを制御するクラス。シングルトン。
class Control
{
	std::vector<std::shared_ptr<Scene>> *mScene;

public:
	Control();
	~Control();

	// 初期化処理
	void init();
	// 更新処理
	void update();
	// 描画処理
	void draw();

	void pushScene(Scene* scene);
	void popScene();
};