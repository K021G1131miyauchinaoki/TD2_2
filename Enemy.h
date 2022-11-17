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
	void Update(int num);

	/// 描画
	void Draw(ViewProjection viewProjection);

	//接近
	void Approach();

	//離脱
	void Leave();

	//弾種類
	//直線(自機狙い)
	void SelfAiming(int32_t speed);
	//誘導
	void InductionFire();
	//螺旋
	void TurningFire(int32_t speed);

	void SetPlayer(Player* player) { player_ = player; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//半径を返す関数
	float GetRadius();

	//敵リスト
	const std::list<std::unique_ptr<Enemy>>& GetEnemys() { return enemys_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() const { return isDead_; }

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	//ゲームシーン
	GameScene* gameScene_ = nullptr;

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

	//敵の行動パターン
	Phase phase_ = Phase::None;

	//デスフラグ
	bool isDead_ = false;

	//敵リスト
	std::list<std::unique_ptr<Enemy>> enemys_;

	//自キャラ
	Player* player_ = nullptr;

	//体力
	int32_t HP_ = 10;
	//半径
	float radius = 15.0f;
	//フェーズフラグ
	int isPhase;

};

