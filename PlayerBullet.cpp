#include "PlayerBullet.h"
#include <cassert>
#include "MyFunc.h"

//コンストラクタ
PlayerBullet::PlayerBullet() {

}

//デストラクタ
PlayerBullet::~PlayerBullet() {

}

//メンバ関数
//初期化
void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity) {

	//nullポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = TextureManager::Load("black.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();

		/*追加*/
	worldTransform_.scale_ = { 0.2f,0.2f,0.2f };

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

}

//更新処理
void PlayerBullet::Update() {

	MyFunc::UpdateWorldTransform(worldTransform_);

	//座標を移動させる
	worldTransform_.translation_ += velocity_;

	//時間経過で弾が消える
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}

}

//描画処理
void PlayerBullet::Draw(const ViewProjection& viewprojection) {

	model_->Draw(worldTransform_, viewprojection, textureHandle_);

}

//ワールド座標を取得
Vector3 PlayerBullet::GetWorldPosition()
{
	//ワールド座標を入れるための変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

//半径を返す関数
float PlayerBullet::GetRadius()
{
	return radius;
}
