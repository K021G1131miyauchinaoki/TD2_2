#pragma once
#include <cassert>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin/affin.h"
#include "Input.h"
#include "DebugText.h"
#include "MyFunc.h"

class RailCamera
{
public:
	//������
	void Initialize(const Vector3& position, const Vector3& rotation);

	//�X�V
	void Update(int num);

	const ViewProjection& GetViewProjection() 
	{ 
		return viewProjection_;
	}

	//���[���h���W�̎擾
	WorldTransform* GetWorldPosition();

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	const float M_PI = 3.141592f;
};