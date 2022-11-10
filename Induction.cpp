#include "Induction.h"

void Induction::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULL�|�C���^
	assert(model);

	//
	model_ = model;
	//�e�N�X�`���n���h���̓ǂݍ���
	textureHandle_ = TextureManager::Load("mera.jpg");

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();
	//�����Ŏ󂯂Ƃ����������W���Z�b�g
	worldTransform_.translation_ = { position.x, position.y, position.z };
	/*affinMat.translate = affin::generateTransMat(worldTransform_);*/

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

void Induction::Update()
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	Vector3 toPlayer;
	toPlayer.x= player_->GetWorldPosition().x - worldTransform_.translation_.x;
	toPlayer.z = player_->GetWorldPosition().z - worldTransform_.translation_.z;
	//���K��
	Vector3Normalize(toPlayer);
	Vector3Normalize(velocity_);
	//���ʐ��`��Ԃō��̑��x�Ǝ��L�����ւ̃x�N�g������}��,�V���ȑ��x�Ƃ���
	velocity_=/*�X���[�v�֐�(velocity_,toPlayer,1�t���[���ł̕�Ԋ���t)*�G�e�̑��x*/
	//�i�s�����Ɍ����ڂ̉�]�����킹��

	//���W���ړ�������
	worldTransform_.translation_ -= velocity_;
	affinMat.translate = affin::generateTransMat(worldTransform_);
	affin::setTransformationWolrdMat(affinMat, worldTransform_);
	//�s��̍���
	worldTransform_.TransferMatrix();

	//���Ԍo�߂Œe��������
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void Induction::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//���[���h���W���擾
Vector3 Induction::GetWorldPosition()
{
	//���[���h���W�����邽�߂̕ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Induction::OnCollision()
{
	isDead_ = true;
}

//���a��Ԃ��֐�
float Induction::GetRadius()
{
	return radius;
}