#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include <cassert>
#include <memory>
#include <list>
#include "EnemyBullet.h"
#include "Induction.h"
#include "Turning.h"
#include "Player/Player.h"
#include "MathUtility.h"
using namespace MathUtility;

//���@�N���X�̑O���錾
class Player;

//GameScene�̑O���錾
class GameScene;

class Enemy
{

	enum class Phase
	{
		None,
		Approach,//�ڋ�
		Leave,   //���E
	};

public:
	/// ����
	void Initialize(Model* model, uint32_t textureHandle);

	/// �X�V
	void Update(int num);

	/// �`��
	void Draw(ViewProjection viewProjection);

	//�ڋ�
	void Approach();

	//���E
	void Leave();

	//�e���
	//����(���@�_��)
	void SelfAiming(int32_t speed);
	//�U��
	void InductionFire();
	//����
	void TurningFire(int32_t speed);

	void SetPlayer(Player* player) { player_ = player; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	//���a��Ԃ��֐�
	float GetRadius();

	//�G���X�g
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() const { return isDead_; }

private:
	//���[���h�g�����X�t�H�[��
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	
	//�Q�[���V�[��
	GameScene* gameScene_ = nullptr;

	DebugText* debugText_ = nullptr;

	//���x
	Vector3 velocity_ = { 0.0f,0.0f,0.1f };

	//�ڋߑ��x
	Vector3 approach_ = {0.0f,0.0f,-0.3f};

	//���E���x
	Vector3 leave_ = {0.0f,0.0f,0.3f};

	// Affin�֐��̍\����
	affin::AffinMat affinMat;

	//�ł��o���܂ł̎���
	float delayTimer = 20.0f;
	//�U���e�̔��ˎ���
	float inductionTimer = 25.0f;
	//�Ǐ]�e�̔��ˎ���
	float turningTimer = 15.0f;

	//�e��؂�ւ���^�C�}�[
	float switTimer = 30.0f;

	//�G�̍s���p�^�[��
	Phase phase_ = Phase::None;

	//�f�X�t���O
	bool isDead_ = false;

	//�G���X�g
	std::list<std::unique_ptr<Enemy>> enemys_;

	//���L����
	Player* player_ = nullptr;

	//�̗�
	int32_t HP_ = 10;
	//���a
	float radius = 15.0f;
	//�t�F�[�Y�t���O
	int isPhase;

};

