#include "PlayerBullet.h"
#include <cassert>
#include "MyFunc.h"

//�R���X�g���N�^
PlayerBullet::PlayerBullet() {

}

//�f�X�g���N�^
PlayerBullet::~PlayerBullet() {

}

//�����o�֐�
//������
void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {

	//null�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

		/*�ǉ�*/
	worldTransform_.scale_ = { 0.2f,0.2f,0.2f };

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;

}

//�X�V����
void PlayerBullet::Update() {

	MyFunc::UpdateWorldTransform(worldTransform_);

	//���W���ړ�������
	worldTransform_.translation_ += velocity_;

	//���Ԍo�߂Œe��������
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

//�`�揈��
void PlayerBullet::Draw(const ViewProjection& viewprojection) {

	model_->Draw(worldTransform_, viewprojection, textureHandle_);

}

//���[���h���W���擾
Vector3 PlayerBullet::GetWorldPosition()
{
	//���[���h���W�����邽�߂̕ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

//���a��Ԃ��֐�
float PlayerBullet::GetRadius()
{
	return radius;
}
