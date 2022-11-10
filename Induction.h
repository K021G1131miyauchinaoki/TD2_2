#pragma once
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include "Player/Player.h"
#include "MathUtility.h"
using namespace MathUtility;

//前方宣言Player
class Player;

class Induction
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

	void SetPlayer(Player* player) { player_ = player; }
	Vector3 sLerp(const Vector3& v1, const Vector3& v2, float time);
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;

	//寿命<frm>
	static const int32_t kLiteTime = 60 * 5;

	//デスタイマー
	int32_t deathTimer_ = kLiteTime;

	//デスフラグ
	bool isDead_ = false;

	//アフィン
	affin::AffinMat affinMat;

	//半径
	float radius = 1.0f;
	Player* player_ = nullptr;
};

