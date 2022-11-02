#pragma once
#include <cassert>
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin/affin.h"

//�V��
class skydome
{
public:
	//������
	void Initialize(Model* model);

	//�X�V
	void Update();

	//�`��
	void Draw(ViewProjection viewProjection_);
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;


	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	//���f��
	Model* model_ = nullptr;

	// Affin�֐��̍\����
	affin::AffinMat affinMat;
};

