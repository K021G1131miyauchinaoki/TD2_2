#include "MyFunc.h"

float MyFunc::Deg2Rad(float x)
{
	const float M_PI = 3.141592f;
	return x * M_PI / 180;
}

float MyFunc::Rad2Deg(float x)
{
	const float M_PI = 3.141592f;
	return x / (M_PI / 180);
}

void MyFunc::UpdateWorldTransform(WorldTransform& worldTransform)
{
	//�A�t�B���ϊ��p�̍s��
	affin::AffinMat affinMat;

	//Translate�̏�������
	affin::setScaleMat(affinMat.scale, worldTransform);
	affin::setRotateMat(affinMat, worldTransform);
	affin::setTranslateMat(affinMat.translate, worldTransform);

	//matWorld�ɒP�ʍs�������
	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	//�s��̌v�Z
	affin::setTransformationWolrdMat(affinMat, worldTransform);

	//�s��̓]��
	worldTransform.TransferMatrix();
}