#include "Turning.h"
void Turning::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
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
	worldTransform_.translation_ = { position.x, position.y, position.z - 20 };
	/*affinMat.translate = affin::generateTransMat(worldTransform_);*/

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
	
	debugText_ = DebugText::GetInstance();
}

void Turning::Update()
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	if (phaseNum_ == true)
	{
		worldTransform_.rotation_.y += 0.3f;
	}
	affinMat.rotateY = affin::generateRotateYMat(worldTransform_);
	affin::setRotateMat(affinMat, worldTransform_);
	//���W���ړ�������
	if (worldTransform_.translation_.z >= 0.0f && phaseNum_ == false)
	{
		worldTransform_.translation_ -= velocity_;
		if (worldTransform_.translation_.z <= 0.0f && phaseNum_ == false)
		{
			phaseNum_ = true;
		}
	}
	if (phaseNum_ == true)
	{
		
		//���̑��x
		const float kBulletSpeed = 0.5f;

		//���@�_���e
		assert(player_);
		
		//�v���C���[�̃��[���h���W�̎擾
		Vector3 playerPosition;
		playerPosition = player_->GetWorldPosition();
		//���@�̃��[���h���W�̎擾
		Vector3 myPosition;
		myPosition = GetWorldPosition();

		Vector3 velocity(0, 0, 0);

		//�����x�N�g�������߂�
		velocity = myPosition - playerPosition;

		//���������߂�
		Vector3Length(velocity);
		//���K��
		Vector3Normalize(velocity);
		//�x�N�g���̒�����,�����ɍ��킹��
		if (--followingTimer_ <= 0.0f)
		{
			velocity *= kBulletSpeed;//���ꂪ���x�ɂȂ�
			followingTimer_ = followTime;
		}
		worldTransform_.translation_ -= velocity / 5;

		//���Ԍo�߂Œe��������
		if (--deathTimer_ <= 0)
		{
			isDead_ = true;
		}
	}
	
	affinMat.translate = affin::generateTransMat(worldTransform_);
	affin::setTransformationWolrdMat(affinMat, worldTransform_);
	//�s��̍���
	worldTransform_.TransferMatrix();

	

	debugText_->SetPos(50, 80);
	debugText_->Printf("worldTranslation : %f", worldTransform_.translation_.z);
	debugText_->SetPos(50, 100);
	debugText_->Printf("phaseNum : %f", phaseNum_);
	debugText_->SetPos(50, 120);
	debugText_->Printf("follwoTime : %d", followingTimer_);
}

void Turning::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//���[���h���W���擾
Vector3 Turning::GetWorldPosition()
{
	//���[���h���W�����邽�߂̕ϐ�
	Vector3 worldPos;

	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Turning::OnCollision()
{
	isDead_ = true;
}

//���a��Ԃ��֐�
float Turning::GetRadius()
{
	return radius;
}

bool Turning::GetPhaseNum()
{
	return phaseNum_;
}

Vector3 Turning::sLerp(const Vector3& start, const Vector3& end, float time)
{
	//��Lerp�֐�sLerp�ɕύX����K�v������

	return start * (1 - time) + end * time;
}

