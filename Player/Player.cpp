#include "Player.h"

Player::Player(){}

//������
void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
    textureHandle_ = textureHandle;
	model_ = model;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.rotation_ = {};

	worldTransform_.translation_ = Vector3{ 0,0,-15 };

	/*�ǉ�*/
	timer = 0;
}

//�X�V
void Player::Update(int num)
{
	//�ړ�
	Move();
	//��]
	Rotate();

	//�y�A�����g��X�V
	//worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	MyFunc::UpdateWorldTransform(worldTransform_);

	//�U��
	if (num==0)
	{
		Attack();
	}
	//�e�X�V
	for(std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr < PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	/*debugText_->SetPos(50, 50);
	debugText_->Printf("%f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("%f,%f,%f",
		worldTransform_.rotation_.x,
		worldTransform_.rotation_.y,
		worldTransform_.rotation_.z);*/
}

//�ړ�
void Player::Move()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	const float kCharacterSpeed = 0.2f;

	//�����������ňړ��ʂ�ω�
	if (input_->PushKey(DIK_RIGHT))
	{
		move.x = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_LEFT))
	{
		move.x = -kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP))
	{
		move.z = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		move.z = -kCharacterSpeed;
	}

	//�ړ����E���W
	const float kMoveLimitX = 36.0f;
	const float kMoveLimitY = 20.0f;

	//transLation��move�����Z����
	worldTransform_.translation_ += move;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	
}

void Player::Rotate()
{
	//�L�����N�^�[�̉�]�x�N�g��
	Vector3 rotate = { 0,0,0 };

	const float kCharacterSpeed = 0.02f;

	//�����������ňړ��ʂ�ω�
	if (input_->PushKey(DIK_I))
	{
		rotate.y = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_U))
	{
		rotate.y = -kCharacterSpeed;
	}

	//�ړ����E���W
	const float kRotateLimitX = 0.5f;

	//rotation��rotate�����Z����
	worldTransform_.rotation_ += rotate;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.rotation_.y = max(worldTransform_.rotation_.y, -kRotateLimitX);
	worldTransform_.rotation_.y = min(worldTransform_.rotation_.y, +kRotateLimitX);

}

//�`��
void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

//�e�̔���
void Player::Attack() 
{

	if (input_->PushKey(DIK_SPACE)) {

		timer -= 0.1f;

		//���L�����̍��W���R�s�[
		Vector3 position = GetWorldPosition();

		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = bVelocity(velocity, worldTransform_);

		//�N�[���^�C�����O�ɂȂ����Ƃ�
		if (timer <= 0)
		{
			//���̐���
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//���̏�����
			newBullet->Initialize(model_, position, velocity);

			//���̓o�^
			bullets_.push_back(std::move(newBullet));

			timer = time;
		}
	}
}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform) 
{

	Vector3 result = { 0,0,0 };

	//����
	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];

	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	return result;
}

//���[���h���W���擾
Vector3 Player::GetWorldPosition()
{
	//���[���h���W�����邽�߂̕ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision()
{

}

//���a��Ԃ��֐�
float Player::GetRadius()
{
	return radius;
}

void Player::SetParent(WorldTransform* worldTransform)
{
	worldTransform_.parent_ = worldTransform;
}