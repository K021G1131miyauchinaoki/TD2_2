#include "Player.h"

Player::Player(){}

//初期化
void Player::Initialize(Model* model, uint32_t textureHandle)
{
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
    textureHandle_ = textureHandle;
	model_ = model;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.rotation_ = {};

	worldTransform_.translation_ = Vector3{ 0,0,-15 };

	/*追加*/
	timer = 0;
}

//更新
void Player::Update(int num)
{
	//移動
	Move();
	//回転
	Rotate();

	//ペアレント先更新
	//worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	MyFunc::UpdateWorldTransform(worldTransform_);

	//攻撃
	if (num==0)
	{
		Attack();
	}
	//弾更新
	for(std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr < PlayerBullet>& bullet)
		{
			return bullet->IsDead();
		});

	/*debugText_->SetPos(50, 50);
	debugText_->Printf("%f,%f,%f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);

	debugText_->SetPos(50, 70);
	debugText_->Printf("%f,%f,%f",
		worldTransform_.rotation_.x,
		worldTransform_.rotation_.y,
		worldTransform_.rotation_.z);*/
}

//移動
void Player::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	const float kCharacterSpeed = 0.2f;

	//押した方向で移動量を変化
	if (input_->PushKey(DIK_RIGHT))
	{
		move.x = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_LEFT))
	{
		move.x = -kCharacterSpeed;
	}

	if (input_->PushKey(DIK_UP))
	{
		move.z = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_DOWN))
	{
		move.z = -kCharacterSpeed;
	}

	//移動限界座標
	const float kMoveLimitX = 36.0f;
	const float kMoveLimitY = 20.0f;

	//transLationにmoveを加算する
	worldTransform_.translation_ += move;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
	
}

void Player::Rotate()
{
	//キャラクターの回転ベクトル
	Vector3 rotate = { 0,0,0 };

	const float kCharacterSpeed = 0.02f;

	//押した方向で移動量を変化
	if (input_->PushKey(DIK_I))
	{
		rotate.y = kCharacterSpeed;
	}
	else if (input_->PushKey(DIK_U))
	{
		rotate.y = -kCharacterSpeed;
	}

	//移動限界座標
	const float kRotateLimitX = 0.5f;

	//rotationにrotateを加算する
	worldTransform_.rotation_ += rotate;

	//範囲を超えない処理
	worldTransform_.rotation_.y = max(worldTransform_.rotation_.y, -kRotateLimitX);
	worldTransform_.rotation_.y = min(worldTransform_.rotation_.y, +kRotateLimitX);

}

//描画
void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

//弾の発射
void Player::Attack() 
{

	if (input_->PushKey(DIK_SPACE)) {

		timer -= 0.1f;

		//自キャラの座標をコピー
		Vector3 position = GetWorldPosition();

		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = bVelocity(velocity, worldTransform_);

		//クールタイムが０になったとき
		if (timer <= 0)
		{
			//球の生成
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			//球の初期化
			newBullet->Initialize(model_, position, velocity);

			//球の登録
			bullets_.push_back(std::move(newBullet));

			timer = time;
		}
	}
}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform) 
{

	Vector3 result = { 0,0,0 };

	//内積
	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];

	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	return result;
}

//ワールド座標を取得
Vector3 Player::GetWorldPosition()
{
	//ワールド座標を入れるための変数
	Vector3 worldPos;

	//ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision()
{

}

//半径を返す関数
float Player::GetRadius()
{
	return radius;
}

void Player::SetParent(WorldTransform* worldTransform)
{
	worldTransform_.parent_ = worldTransform;
}