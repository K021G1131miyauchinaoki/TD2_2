﻿#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "WinApp.h"
#include "math.h"
#include "affin/affin.h"
#include "MyFunc.h"
#include "Player/Player.h"
#include "Enemy.h"
#include "skydome.h"
#include "RailCamera.h"
#include "EnemyBullet.h"
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public:

  public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//すべての子分の当たり判定をチェックする関数
	void CheckAllCollisions();

	//敵弾を追加する
	void AddEnemyBullet(std::unique_ptr<EnemyBullet>&enemyBullet);

	//弾リスト
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	//敵弾の更新
	void EnemyBulletUpdate();

	/// 敵発生
	void EnemyOcurrence(const Vector3& v);

	//敵発生データの読み込み
	void LoadEnemyPopData();

	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t enemyHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;

	//敵弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//敵
	std::list<std::unique_ptr<Enemy>> enemys_;

	//敵の打ち出すまでの時間
	float enemyDalayTimer = 0.0f;

	bool isWait_ = false;

	int waitTimer = 300;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	GameScene* gameScene_ = nullptr;

	//天球
	skydome* skydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	const float M_PI = 3.141592f;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};