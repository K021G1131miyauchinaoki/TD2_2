#pragma once
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include "Player/Player.h"

class Induction
{
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity,Vector3& angle);
	void Update();
	void Draw(const ViewProjection& viewProjection);


private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;
	//�p�x
	float angle_;
	//���a
	float radius = 1.0f;
};

