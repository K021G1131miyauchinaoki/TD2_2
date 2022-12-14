#include "Turning.h"
void Turning::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	// NULLポインタ
	assert(model);

	//
	model_ = model;
	//テクスチャハンドルの読み込み
	textureHandle_ = TextureManager::Load("enemyB.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	//引数で受けとった初期座標をセット
	worldTransform_.translation_ = { position.x, position.y, position.z - 20 };
	/*affinMat.translate = affin::generateTransMat(worldTransform_);*/

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
	
	debugText_ = DebugText::GetInstance();
}

void Turning::Update()
{
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();
	if (phaseNum_ == true)//追従するのか判定(trueで追従中)
	{
		//追従中横に回転
		worldTransform_.rotation_.y += 0.3f;
	}
	affinMat.rotateY = affin::generateRotateYMat(worldTransform_);
	affin::setRotateMat(affinMat, worldTransform_);
	//座標を移動させる
	if (worldTransform_.translation_.z >= 0.0f && phaseNum_ == false)//弾が出始め所定の場所へ進める
	{
		worldTransform_.translation_ -= velocity_;
		if (worldTransform_.translation_.z <= 0.0f && phaseNum_ == false)//所定地についたら追従モード
		{
			phaseNum_ = true;
		}
	}
	if (phaseNum_ == true)//追従モード中一定時間でプレイヤーの動きをトレース進行方向が曲がるなど
	{
		
		//球の速度
		const float kBulletSpeed = 0.5f;

		//自機狙い弾
		assert(player_);
		
		//プレイヤーのワールド座標の取得
		Vector3 playerPosition;
		playerPosition = player_->GetWorldPosition();
		//自機のワールド座標の取得
		Vector3 myPosition;
		myPosition = GetWorldPosition();

		Vector3 velocity(0, 0, 0);

		//差分ベクトルを求める
		velocity = myPosition - playerPosition;

		//長さを求める
		Vector3Length(velocity);
		//正規化
		Vector3Normalize(velocity);
		//ベクトルの長さを,速さに合わせる
		if (--followingTimer_ <= 0.0f)//一定時間で再度計算されている値を掛ける
		{
			velocity *= kBulletSpeed;//これが速度になる
			followingTimer_ = followTime;//タイマーを戻して再度トレース出来るようにする
		}
		worldTransform_.translation_ -= velocity / 5;

		//時間経過で弾が消える
		if (--deathTimer_ <= 0)
		{
			isDead_ = true;
		}
	}
	
	affinMat.translate = affin::generateTransMat(worldTransform_);
	affin::setTransformationWolrdMat(affinMat, worldTransform_);
	//行列の合成
	worldTransform_.TransferMatrix();
}

void Turning::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

//ワールド座標を取得
Vector3 Turning::GetWorldPosition()
{
	//ワールド座標を入れるための変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Turning::OnCollision()
{
	isDead_ = true;
}

//半径を返す関数
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
	//↓Lerp関数sLerpに変更する必要がある

	return start * (1 - time) + end * time;
}

