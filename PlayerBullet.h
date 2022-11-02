#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"
#include "affin/affin.h"

class PlayerBullet
{
public:
	//�R���X�g���N�^
	PlayerBullet();

	//�f�X�g���N�^
	~PlayerBullet();

	//�����o�֐�
		//������
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//�X�V����
	void Update();

	//�`�揈��
	void Draw(const ViewProjection& viewprojection);

	//isDead��getter
	bool IsDead() const { return isDead_; }

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//���a��Ԃ��֐�
	float GetRadius();

private:
	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLiteTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLiteTime;

	//�f�X�t���O
	bool isDead_ = false;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���a
	float radius = 1.0f;

};