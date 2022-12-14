#pragma once
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include <cassert>
#include <memory>
#include <list>
#include "EnemyBullet.h"
#include "Induction.h"
#include "Turning.h"
#include "Player/Player.h"
#include "MathUtility.h"
using namespace MathUtility;

//自機クラスの前方宣言
class Player;

//GameSceneの前方宣言
class GameScene;

class Enemy
{

	enum class Phase
	{
		None,
		Approach,//接近
		Leave,   //離脱
	};

public:
	/// 生成
	void Initialize(Model* model, uint32_t textureHandle);

	/// 更新
	void Update(bool isFlag,int movie);

	/// 描画
	void Draw(ViewProjection viewProjection);

	//ループ内の初期化
	void	State();

	//弾種類
	//直線(自機狙い)
	void SelfAiming(int32_t speed);
	//螺旋
	void TurningFire(int32_t speed);

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//半径を返す関数
	float GetRadius();

	bool IsDead() const { return isDead_; }

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	//弾リストを取得
	std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; };
	//弾
	std::list<std::unique_ptr<Turning>> tBullets_;
	//弾リストを取得
	std::list<std::unique_ptr<Turning>>& GetTurning() { return tBullets_; };

	//敵弾デリート
	void	BulletDelete();

	int32_t GetHP()const { return HP_; }
private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	DebugText* debugText_ = nullptr;

	//速度
	Vector3 velocity_ = { 0.0f,0.0f,0.1f };

	//接近速度
	Vector3 approach_ = {0.0f,0.0f,-0.3f};

	//離脱速度
	Vector3 leave_ = {0.0f,0.0f,0.3f};

	// Affin関数の構造体
	affin::AffinMat affinMat;

	//打ち出すまでの時間
	float delayTimer = 20.0f;
	//誘導弾の発射時間
	float inductionTimer = 25.0f;
	//追従弾の発射時間
	float turningTimer = 15.0f;

	//弾を切り替えるタイマー
	float switTimer = 30.0f;


	//デスフラグ
	bool isDead_ = false;

	//自キャラ
	Player* player_ = nullptr;

	//体力
	int32_t HP_ = 20;
	//半径
	float radius = 15.0f;
};

