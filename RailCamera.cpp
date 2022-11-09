#include "RailCamera.h"

using namespace MathUtility;

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	input_ = Input::GetInstance();
	//�V���O���C���X�^���X���擾����
	debugText_ = DebugText::GetInstance();

	//���[���h�g�����X�t�H�[���̏����ݒ�(���W�Ɖ�]�p)
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	worldTransform_.Initialize();
	//�r���[�v���W�F�N�V�����̏�����
	//viewProjection_.farZ = 300.0f;
	viewProjection_.Initialize();
}

void RailCamera::Update()
{
	Vector3 move = { 0, 0, 0 };

	const float kCharaSpeed = 0.2f;

	Matrix4 matTranslate;

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

#pragma region �ړ�����
	//if (input_->PushKey(DIK_A)) {
	//	move.x -= kCharaSpeed;
	//}
	//if (input_->PushKey(DIK_D)) {
	//	move.x += kCharaSpeed;
	//}
	//if (input_->PushKey(DIK_W)) {
	//	move.y += kCharaSpeed;
	//}
	//if (input_->PushKey(DIK_S)) {
	//	move.y -= kCharaSpeed;
	//}

	////�ړ����E���W
	//const float kMoveLimitX = 36.0f;
	//const float kMoveLimitY = 20.0f;

	//worldTransform_.translation_.x += move.x;
	//worldTransform_.translation_.y += move.y;
	//
	////�͈͂𒴂��Ȃ�����
	//worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	//worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	//worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	//worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	matTranslate = affin::generateTransMat(worldTransform_);
#pragma endregion

#pragma region ��]����
	Vector3 Rotation = { 0, 0, 0 };
	const float kCharaRotX = MyFunc::Deg2Rad(0.5f);

	Matrix4 matRotX;

	//��]����
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.x += kCharaRotX;
	}
	if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.x -= kCharaRotX;
	}
	//(-)2�΂𒴂������]�p��0�ɖ߂�����
	if (worldTransform_.rotation_.x >= (M_PI * 2) || worldTransform_.rotation_.x <= -(M_PI * 2)) {
		worldTransform_.rotation_.x = 0;
	}

	matRotX = affin::generateRotateXMat(worldTransform_);
#pragma endregion

	worldTransform_.matWorld_.operator*=(matRotX);

	worldTransform_.matWorld_.operator*=(matTranslate);

	//�s��̓]��
	worldTransform_.TransferMatrix();



	viewProjection_.eye = worldTransform_.translation_;
	//�O���x�N�g��
	Vector3 forward(0, -1, 0);

	//���[���J�����̉�]�𔽉f
	forward = affin::matVector(forward, worldTransform_.matWorld_);

	//���_����O���ɓK���ɐi�񂾈ʒu�������_
	viewProjection_.target = viewProjection_.eye + forward;

	//����x�N�g��
	Vector3 up(0, 1, 1);
	//
	viewProjection_.up = affin::matVector(up, worldTransform_.matWorld_);

	//�r���[�v���W�F�N�V�����̍X�V�ƍs��o�b�t�@�ւ̓]��
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	//eye�̕\��
	debugText_->SetPos(50, 110);
	debugText_->Printf(
		"%f,%f,%f", viewProjection_.eye.x, viewProjection_.eye.y,
		viewProjection_.eye.z);

}

WorldTransform* RailCamera::GetWorldPosition()
{
	return &worldTransform_;
}