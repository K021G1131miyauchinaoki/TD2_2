#pragma once
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include "Player/Player.h"
#include "MathUtility.h"
using namespace MathUtility;

//�O���錾Player
class Player;

class Induction
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

	void SetPlayer(Player* player) { player_ = player; }
	Vector3 sLerp(const Vector3& v1, const Vector3& v2, float time);
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;

	//����<frm>
	static const int32_t kLiteTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLiteTime;

	//�f�X�t���O
	bool isDead_ = false;

	//�A�t�B��
	affin::AffinMat affinMat;

	//���a
	float radius = 1.0f;
	Player* player_ = nullptr;
};

