#pragma once
#include "DebugText.h"
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include "Player/Player.h"
#include "MathUtility.h"
using namespace MathUtility;

class Turning
{
public:
	//生成
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//isDeadのgetter
	bool IsDead() const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//半径を返す関数
	float GetRadius();
	bool GetPhaseNum();

	Vector3 sLerp(const Vector3& start, const Vector3& end, float time);

	void SetPlayer(Player* player) { player_ = player; }
private:
	//テキスト
	DebugText* debugText_ = nullptr;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;
	Vector3 hardVelocity_;
	
	//寿命<frm>
	static const int32_t kLiteTime = 60 * 10;
	static const int32_t followTime = 60 * 1;

	//デスタイマー
	int32_t deathTimer_ = kLiteTime;
	int32_t followingTimer_ = followTime;

	//デスフラグ
	bool isDead_ = false;
	//行動制御
	bool phaseNum_ = false;

	//アフィン
	affin::AffinMat affinMat;

	//半径
	float radius = 1.0f;

	Player* player_ = nullptr;
};

