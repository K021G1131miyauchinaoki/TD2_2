#include "Enemy.h"
#include "GameScene.h"
void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.scale_ = { 15,15,15 };
	worldTransform_.translation_ = { 0,0,28 };
	worldTransform_.Initialize();

}

void	Enemy::State() {
	BulletDelete();
	//打ち出すまでの時間
	delayTimer = 20.0f;
	//誘導弾の発射時間
	inductionTimer = 25.0f;
	//追従弾の発射時間
	turningTimer = 15.0f;
	//弾を切り替えるタイマー
	switTimer = 30.0f;
	//デスフラグ
	isDead_ = false;
	//体力
	HP_ = 20;
}

void Enemy::Update( bool isFlag,int movie)
{
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
	tBullets_.remove_if([](std::unique_ptr<Turning>& bullet) { return bullet->IsDead(); });

	//単位行列を設定
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	//行列の計算
	affinMat.scale = affin::generateScaleMat(worldTransform_);
	affinMat.translate = affin::generateTransMat(worldTransform_);
	
	//行列の合成
	affin::setTransformationWolrdMat(affinMat, worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
	
	//弾を発射
	if (movie == 0)
	{
		switTimer -= 0.1f;
		if (HP_ > 10)
		{
			if (switTimer >= 15.0f)
			{
				SelfAiming(1);
			}
			else if (switTimer < 15.0f)
			{
				TurningFire(1);
			}
		}
		else if (HP_ <= 10)
		{
			SelfAiming(2);
			TurningFire(2);
		}
		if (switTimer <= 0.0f)
		{
			switTimer = 30.0f;
		}

		for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
			bullet->Update();
		}
		for (std::unique_ptr<Turning>& bullet : tBullets_) {
			bullet->Update();
		}
	}
	/*InductionFire();*/
	

	//デバックテキスト
	/*debugText_->SetPos(50, 60);
	debugText_->Printf("%f,%f,%f", 
		worldTransform_.translation_.x, 
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
	debugText_->SetPos(50, 20);
	debugText_->Printf("HP_:%d", HP_);*/
}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
	for (std::unique_ptr<Turning>& bullet : tBullets_) {
		bullet->Draw(viewProjection);
	}

}

void Enemy::SelfAiming(int32_t speed)
{

	//球の速度
	const float kBulletSpeed = 0.5f * speed;
	//ディレイタイム
	delayTimer -= 0.5f;

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
		bullets_.push_back(std::move(newBullet));
		//gameScene_->AddEnemyBullet(newBullet);

		delayTimer = 20.0f;
	}
}

void Enemy::TurningFire(int32_t speed)
{
	turningTimer -= 0.2f * speed;

	if (turningTimer <= 0.0f)
	{
		//弾を生成
		std::unique_ptr<Turning> newBullet = std::make_unique<Turning>();
		//弾の初期化
		newBullet->Initialize(model_, worldTransform_.translation_, Vector3(0.0f, 0.0f, 0.1f));
		newBullet->SetPlayer(player_);
		//弾の登録
		tBullets_.push_back(std::move(newBullet));

		//gameScene_->AddTurning(newBullet);

		turningTimer = 15.0f;
	}
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
	if (HP_ <= 0)
	{
		isDead_ = true;
	}
	else
	{
		HP_ -= 1;
	}
}

//半径を返す関数
float Enemy::GetRadius()
{
	return radius;
}

void	Enemy::BulletDelete(){
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->OnCollision();
	}
	for (std::unique_ptr<Turning>& bullet : tBullets_) {
		bullet->OnCollision();
	}
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) { return bullet->IsDead(); });
	tBullets_.remove_if([](std::unique_ptr<Turning>& bullet) { return bullet->IsDead(); });
}