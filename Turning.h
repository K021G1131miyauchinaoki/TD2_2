#pragma once
#include "DebugText.h"
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include "Player/Player.h"
#include "MathUtility.h"
using namespace MathUtility;

class Turning
{
public:
	//����
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//�X�V
	void Update();

	//�`��
	void Draw(const ViewProjection& viewProjection);

	//isDead��getter
	bool IsDead() const { return isDead_; }

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//���a��Ԃ��֐�
	float GetRadius();
	bool GetPhaseNum();

	Vector3 sLerp(const Vector3& start, const Vector3& end, float time);

	void SetPlayer(Player* player) { player_ = player; }
private:
	//�e�L�X�g
	DebugText* debugText_ = nullptr;

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;
	Vector3 hardVelocity_;
	
	//����<frm>
	static const int32_t kLiteTime = 60 * 10;
	static const int32_t followTime = 60 * 1;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLiteTime;
	int32_t followingTimer_ = followTime;

	//�f�X�t���O
	bool isDead_ = false;
	//�s������
	bool phaseNum_ = false;

	//�A�t�B��
	affin::AffinMat affinMat;

	//���a
	float radius = 1.0f;

	Player* player_ = nullptr;
};

