#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "DebugText.h"
#include "Input.h"
#include "affin/affin.h"
#include "ViewProjection.h"
#include "MyFunc.h"
#include "PlayerBullet.h"
#include<memory>
#include<list>

//自キャラ
class Player
{
public:
	Player();
	//初期化
	void Initialize(Model* model,uint32_t textureHandle);

	//ループ内の初期化
	void	State();

	//更新
	void Update();

	//移動
	void Move();

	//回転
	void Rotate();

	//攻撃
	void Attack();

	//描画
	void Draw(ViewProjection viewProjection_);

	//キャラの向きに応じた方向に球を出す
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//弾リスト
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//半径を返す関数
	float GetRadius();

	//親子構造のセッター
	void SetParent(WorldTransform* worldTransform);

	bool IsDead() const { return isDead_; }
	int32_t GetHP()const { return HP_; }
private:

	//デスフラグ
	bool isDead_ = false;

	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//打ち出すまでの時間
	float timer = 0.0f;
	const float	time=1.5f;
	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//体力
	int32_t HP_ = 3;
	bool chargFlag = false;
	int32_t bulletCount = 0;
	//半径
	float radius = 1.0f;
};