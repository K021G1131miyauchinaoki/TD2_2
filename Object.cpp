#include "Object.h"

void Object::Initialize(Model* model)
{
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.scale_ = { 20,10,15 };

	worldTransform_.rotation_ = {};

	worldTransform_.translation_ = Vector3{ 0,0,0 };

}

void Object::Update()
{
	//単位行列を設定
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//行列の計算
	affinMat.scale = affin::generateScaleMat(worldTransform_);
	affinMat.translate = affin::generateTransMat(worldTransform_);

	//行列の合成
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
}

void Object::Draw(ViewProjection viewProjection_, uint32_t textureHandle)
{
	model_->Draw(worldTransform_, viewProjection_, textureHandle);
}
