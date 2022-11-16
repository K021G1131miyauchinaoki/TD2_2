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

	debugText_ = DebugText::GetInstance();
}

void Induction::Update()
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//���W���ړ�������
	//�x�N�g���v�Z (player���玩�@�̍��W�܂ł̃x�N�g��)
	Vector3 toPlayer = player_->GetWorldPosition() - worldTransform_.translation_;
	//���K��
	Vector3Normalize(toPlayer);
	Vector3Normalize(velocity_);
	//�V���ȑ��x�ɕύX����
	velocity_ = sLerp(velocity_, toPlayer, 0.01f) * 0.3f;

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
	
	debugText_->SetPos(50, 10);
	debugText_->Printf("�e���W : %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
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

Vector3 Induction::sLerp(const Vector3& v1, const Vector3& v2, float time)
{
	//��Lerp�֐�sLerp�ɕύX����K�v������
	return v1 * (1.0f - time) + v2 * time;
}

