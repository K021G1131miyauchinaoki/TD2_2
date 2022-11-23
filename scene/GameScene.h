#pragma once

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
#include "Object.h"
#include "Enemy.h"
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

  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t enemyHandle_ = 0;
	uint32_t clearHandle_ = 0;
	uint32_t overHandle_ = 0;
	uint32_t titleHandle_ = 0;

	//オブジェクト
	Object* object_ = nullptr;

	//3Dモデル
	Model* model_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;

	//敵キャラ
	Enemy* enemy_ = nullptr;

	//敵の打ち出すまでの時間
	float enemyDalayTimer = 0.0f;

	bool isWait_ = false;

	int waitTimer = 300;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	GameScene* gameScene_ = nullptr;

	//天球
	//skydome* skydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	const float M_PI = 3.141592f;

	//シーン管理
	enum Scene
	{
		title,
		play,
		clear,
		over
	};
	int scene;

	//フェーズ管理,ボス登場,形態変化
	enum Movie
	{
		nonMovie,	 //ムービーなし
		appearance,  //ボス登場
		change1,	 //形態変化1
		change2,	 //形態変化2
	};
	int movie;
	bool isMovie;

	//フェーズをタイム管理（仮）
	const	int phaseTime = 300;
	int phaseTimer;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
