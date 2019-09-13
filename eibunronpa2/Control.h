#pragma once

#include <vector>
#include<memory>

class Scene;

// �V�[���𐧌䂷��N���X�B�V���O���g���B
class Control
{
	std::vector<std::shared_ptr<Scene>> *mScene;

public:
	Control();
	~Control();

	// ����������
	void init();
	// �X�V����
	void update();
	// �`�揈��
	void draw();

	void pushScene(Scene* scene);
	void popScene();
};