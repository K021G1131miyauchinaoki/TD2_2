#include "Object.h"

void Object::Initialize(Model* model)
{
	assert(model);
	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.scale_ = { 20,10,15 };

	worldTransform_.rotation_ = {};

	worldTransform_.translation_ = Vector3{ 0,0,0 };

}

void Object::Update()
{
	//�P�ʍs���ݒ�
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//�s��̌v�Z
	affinMat.scale = affin::generateScaleMat(worldTransform_);
	affinMat.translate = affin::generateTransMat(worldTransform_);

	//�s��̍���
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//�s��̓]��
	worldTransform_.TransferMatrix();
}

void Object::Draw(ViewProjection viewProjection_, uint32_t textureHandle)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle);
}
