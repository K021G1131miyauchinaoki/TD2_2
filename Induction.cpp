#include "Induction.h"

void Induction::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULLポインタ
	assert(model);

	//
	model_ = model;
	//テクスチャハンドルの読み込み
	textureHandle_ = TextureManager::Load("mera.jpg");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数で受けとった初期座標をセット
	worldTransform_.translation_ = { position.x, position.y, position.z };
	/*affinMat.translate = affin::generateTransMat(worldTransform_);*/

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	debugText_ = DebugText::GetInstance();
}

void Induction::Update()
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	float bulletSpeed = 0.3f;
	//差分ベクトル計算 (playerから自機の座標までのベクトル)(終点〜始点)
	Vector3 toPlayer = player_->GetWorldPosition() - worldTransform_.translation_;
	//正規化をする
	Vector3Normalize(toPlayer);
	Vector3Normalize(velocity_);
	//新たな速度に変更する
	velocity_ = sLerp(velocity_, toPlayer, 0.5f) * bulletSpeed;

	//座標を移動させる
	worldTransform_.translation_ -= velocity_;
	affinMat.translate = affin::generateTransMat(worldTransform_);
	affin::setTransformationWolrdMat(affinMat, worldTransform_);
	//行列の合成
	worldTransform_.TransferMatrix();

	//時間経過で弾が消える
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void Induction::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	
}

//ワールド座標を取得
Vector3 Induction::GetWorldPosition()
{
	//ワールド座標を入れるための変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Induction::OnCollision()
{
	isDead_ = true;
}

//半径を返す関数
float Induction::GetRadius()
{
	return radius;
}

Vector3 Induction::sLerp(const Vector3& verocity, const Vector3& Length, float time)
{
	//↓Lerp関数sLerpに変更する必要がある
	
	return verocity;
}
