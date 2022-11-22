#pragma once
#include <cassert>
#include<vector>
#include<cstdlib>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin/affin.h"
#include "Input.h"
#include "DebugText.h"
#include "MyFunc.h"
#include"Enemy.h"

class RailCamera
{
public:
	//������
	void Initialize(const Vector3& position, const Vector3& rotation);

	//�X�V
	void Update(int num);

	//���[�v���̏�����
	void	State();

	const Vector3 CatmullRomSpline(Vector3 P0, Vector3 P1, Vector3 P2, Vector3 P3, float t);
	Vector3	splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t);

	const ViewProjection& GetViewProjection(){return viewProjection_;}

	const	int& GetSwitch();

	//���[���h���W�̎擾
	WorldTransform* GetWorldPosition();



private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	const float M_PI = 3.141592f;

	bool isMovei;

	Enemy* enemy = nullptr;
	Vector3 rotMove = { 0, 0, 0 };
	float	radius;
	const	float len = 5;
	float	add_x;
	float	add_z;
	Vector3	pos;
	float	angle;
	//���`���
	//�擪�ƍŌ�ɐ���_��ǉ����Ă���
	std::vector<Vector3>points;
	//p1����X�^�[�g����
	size_t startIndex;
	Vector3	start = { 30,-14,26 };//�X�^�[�g�n�_
	Vector3	p2 = { 20, -7 , 10 };
	Vector3	p3 = { 14,-4,-6 };
	Vector3	end = { 0,3,-35 };	 //�G���h�n�_
	float timerTrans;
	const	float timerTransMax=100;
	float	timeRate;
	
	int	switchTimer;

	Vector3	startTrans;
	Vector3	endTrans;
	float startFovAngle;
	float endFovAngle;
	float timerFovAngle;
	const float timeAngleMax=100;
};