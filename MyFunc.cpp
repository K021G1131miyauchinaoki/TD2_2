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
	//アフィン変換用の行列
	affin::AffinMat affinMat;

	//Translateの情報を入れる
	affin::setScaleMat(affinMat.scale, worldTransform);
	affin::setRotateMat(affinMat, worldTransform);
	affin::setTranslateMat(affinMat.translate, worldTransform);

	//matWorldに単位行列を入れる
	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	//行列の計算
	affin::setTransformationWolrdMat(affinMat, worldTransform);

	//行列の転送
	worldTransform.TransferMatrix();
}