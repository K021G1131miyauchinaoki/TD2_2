#include "Enemy.h"
#include "GameScene.h"
void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.scale_ = { 15,15,15 };
	worldTransform_.translation_ = { 0,0,28 };

}

void Enemy::Update()
{
	//単位行列を設定
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	switch(phase_)
	{
	case Phase::Approach: //接近フェーズ
	default:
		//移動(ベクトルを加算)
		Approach();
		break;
	case Phase::Leave:   //離脱フェーズ
		Leave();
		break;
	
	}
	//行列の計算
	affinMat.scale = affin::generateScaleMat(worldTransform_);
	affinMat.translate = affin::generateTransMat(worldTransform_);
	
	//行列の合成
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
	
	//弾を発射
	SelfAiming();

	//デバックテキスト
	debugText_->SetPos(50, 60);
	debugText_->Printf("%f,%f,%f", 
		worldTransform_.translation_.x, 
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

void Enemy::Approach()
{
	//worldTransform_.translation_ += approach_;

	////既定の位置に着いたら離脱へ
	//if (worldTransform_.translation_.z <= 10.0f)
	//{
	//	phase_ = Phase::Leave;
	//}
}

void Enemy::Leave()
{
	//worldTransform_.translation_ += leave_;

	////既定の位置に着いたら接近へ
	//if (worldTransform_.translation_.z >= 80.0f)
	//{
	//	phase_ = Phase::Approach;
	//}
}

void Enemy::SelfAiming()
{
	delayTimer -= 0.1f;

	//球の速度
	const float kBulletSpeed = 0.5f;

	//自機狙い弾
	assert(player_);

	//プレイヤーのワールド座標の取得
	Vector3 playerPosition;
	playerPosition = player_->GetWorldPosition();
	//敵のワールド座標の取得
	Vector3 enemyPosition;
	enemyPosition = GetWorldPosition();

	Vector3 velocity(0, 0, 0);

	//差分ベクトルを求める
	velocity = enemyPosition - playerPosition;

	//長さを求める
	Vector3Length(velocity);
	//正規化
	Vector3Normalize(velocity);
	//ベクトルの長さを,速さに合わせる
	velocity *= kBulletSpeed;//これが速度になる

	//クールタイムが０になったとき
	if (delayTimer <= 0) {
		//球の生成
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//球の初期化
		newBullet->Initialize(model_, worldTransform_.translation_, velocity);

		//球の登録
		/*bullets_.push_back(std::move(newBullet));*/
		gameScene_->AddEnemyBullet(newBullet);

		delayTimer = 20.0f;
	}
}

void Enemy::InductionFire()
{
	//発射するまでの時間
	delayTimer -= 0.1f;

}

//ワールド座標を取得
Vector3 Enemy::GetWorldPosition() {
	//ワールド座標を入れるための変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision()
{
	isDead_ = true;
}

//半径を返す関数
float Enemy::GetRadius()
{
	return radius;
}
