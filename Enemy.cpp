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

void Enemy::Update(int num, bool isFlag)
{
	this->isPhase = num;
	//単位行列を設定
	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

	switch(phase_)
	{
	case Phase::Approach: //待機フェーズ
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
	switTimer -= 0.1f;
	if (HP_ > 5)
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
	else if (HP_ <= 5)
	{
		SelfAiming(2);
		TurningFire(2);
	}
	if (switTimer <= 0.0f)
	{
		switTimer = 30.0f;
	}
	
	/*InductionFire();*/
	

	//デバックテキスト
	/*debugText_->SetPos(50, 60);
	debugText_->Printf("%f,%f,%f", 
		worldTransform_.translation_.x, 
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);*/
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

void Enemy::SelfAiming(int32_t speed)
{
	if (isPhase == 1)
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
			/*bullets_.push_back(std::move(newBullet));*/
			gameScene_->AddEnemyBullet(newBullet);

			delayTimer = 20.0f;
		}
	}
}

void Enemy::InductionFire()
{
	if (isPhase == false)
	{
		inductionTimer -= 0.1f;
		//球の速度
		const float kBulletSpeed = 0.3f;

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
		if (inductionTimer <= 0) {
			//球の生成
			std::unique_ptr<Induction> newBullet = std::make_unique<Induction>();
			//球の初期化
			newBullet->Initialize(model_, worldTransform_.translation_, velocity);
			newBullet->SetPlayer(player_);

			//球の登録
			/*bullets_.push_back(std::move(newBullet));*/
			gameScene_->Addinduction(newBullet);

			inductionTimer = 20.0f;
		}
		/*debugText_->SetPos(50, 20);
		debugText_->Printf("speed : %f.%f,%f", velocity.x, velocity.y, velocity.z);*/
	}
}

void Enemy::TurningFire(int32_t speed)
{
	if (isPhase == 1)
	{
		turningTimer -= 0.3f * speed;
		
		if (turningTimer <= 0.0f)
		{
			//弾を生成
			std::unique_ptr<Turning> newBullet = std::make_unique<Turning>();
			//弾の初期化
			newBullet->Initialize(model_, worldTransform_.translation_, Vector3(0.0f, 0.0f, 0.1f));
			newBullet->SetPlayer(player_);
			//弾の登録
			gameScene_->AddTurning(newBullet);

			turningTimer = 15.0f;
		}
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

