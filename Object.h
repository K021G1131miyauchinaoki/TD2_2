#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "affin/affin.h"

class Object
{
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection viewProjection_, uint32_t textureHandle);
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;

	// Affin�֐��̍\����
	affin::AffinMat affinMat;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
};

