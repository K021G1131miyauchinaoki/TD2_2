#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <fstream>



GameScene::GameScene() {}

GameScene::~GameScene()
{
	delete model_;
	delete debugCamera_;
	delete player_;
	delete enemy_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読みこむ
	textureHandle_ = TextureManager::Load("player.png");
	enemyHandle_ = TextureManager::Load("enemy.png");
	clearHandle_ = TextureManager::Load("Clear.png");
	overHandle_ = TextureManager::Load("Over.png");
	titleHandle_ = TextureManager::Load("title.png");

	//3Dモデルの生成
	model_ = Model::Create();

	//レールカメラの生成
	railCamera_ = new	RailCamera();
	railCamera_->Initialize(Vector3(0, 50, 0), Vector3(0, 0, 0));

	//オブジェクト
	object_ = new Object();
	//初期化
	object_->Initialize(model_);
	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵キャラの初期化
	enemy_ = new	Enemy();
	enemy_->Initialize(model_,enemyHandle_);
	enemy_->SetPlayer(player_);

	//親子構造
	player_->SetParent(railCamera_->GetWorldPosition());

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	////軸方向の表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);

	////軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

	////ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	scene = Scene::title;
	
	isMovie = false;
	phaseTimer = phaseTime;
}

void GameScene::Update()
{
	switch (scene)
	{
	case Scene::title:		/*タイトル*/
		object_->Update();
		debugText_->SetPos(10, 10);
		debugText_->Printf("title");
		//キーを押したらプレイへ
		if (input_->TriggerKey(DIK_F))
		{
			scene = Scene::play;
			movie = Movie::appearance;
			//ループ内の初期化
			player_->State();
			enemy_->State();
			railCamera_->State();
		}
		break;
	case Scene::play:		/*プレイ*/
		if (input_->TriggerKey(DIK_0))
		{
			movie++;
			if (movie==4)
			{
				movie = 0;
			}
		}
		////ムービーのフラグ処理
		//ムービーなし以外はフラグがtrue
		if (movie!=Movie::nonMovie)
		{
			isMovie = true;
		}
		//ムービーなしでフラグがfalse
		else
		{
			isMovie = false;
		}
		////ムービーの切り替え処理
		if (movie!=0&&railCamera_->GetSwitch()==200)
		{
			movie = Movie::nonMovie;
		}
		else //ムービー中フェーズタイマーを初期化
		{
			phaseTimer = phaseTime;
		}
		//当たり判定
		CheckAllCollisions();
		//フェーズの切り替え
		/*if (phaseTimer--<0&&phase<=1)
		{
			phase ^= 1;
			phaseTimer = phaseTime;
		}*/
		debugText_->SetPos(10, 30);
		debugText_->Printf("%d",phaseTimer);
		debugText_->SetPos(10, 50);
		debugText_->Printf("%d", movie);
		//自キャラの更新
		/*phaseがtrueならプレイヤーの攻撃*/
		player_->Update(movie);

		//敵キャラの更新
		enemy_->Update(isMovie,movie);

		//レールカメラの更新
		railCamera_->Update(movie);

		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
		
		//クリア
		/*if (enemy_->GetHP() <= 0)
		{
			scene = Scene::clear;
		}*/
		if (input_->TriggerKey(DIK_B))
		{
			scene = Scene::clear;
		}
		//オーバー
		if (player_->GetHP() <= 0)
		{
			scene = Scene::over;
		}
		/*if (input_->TriggerKey(DIK_V))
		{
			scene = Scene::over;
		}*/
		break;

	case Scene::clear:		/*ゲームクリア*/
		object_->Update();
		debugText_->SetPos(10, 30);
		debugText_->Printf("clear");
		//スペースでタイトル
		if (input_->TriggerKey(DIK_SPACE))
		{
			scene = Scene::title;
		}
		break;

	case Scene::over:		/*ゲームオーバー*/
		object_->Update();
		debugText_->SetPos(10, 10);
		debugText_->Printf("over");
		//スペースでタイトル
		if (input_->TriggerKey(DIK_SPACE))
		{
			scene = Scene::title;
		}
		break;
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	switch (scene)
	{
	case Scene::title:
		object_->Draw(railCamera_->GetViewProjection(), titleHandle_);
		break;
	case Scene::play:
		//自キャラの描画
		if (movie==Movie::nonMovie)
		{
		}
			player_->Draw(railCamera_->GetViewProjection());

		//敵キャラの描画
		enemy_->Draw(railCamera_->GetViewProjection());
		break;
	case Scene::clear:
		object_->Draw(railCamera_->GetViewProjection(), clearHandle_);
		break;
	case Scene::over:
		object_->Draw(railCamera_->GetViewProjection(), overHandle_);
		break;
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();

	//ライン描画が参照するビュープロジェクションを参照する(アドレス渡し)
	/*PrimitiveDrawer::GetInstance()->DrawLine3d()*/

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//それぞれの半径
	float pRadius = player_->GetRadius();
	float eRadius = NULL;
	//float eTRadius = NULL;
	eRadius = enemy_->GetRadius();

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets_ = enemy_->GetBullets();
	const std::list<std::unique_ptr<Turning>>& turning_ = enemy_->GetTurning();

#pragma region 自キャラと敵弾の当たり判定

	//自キャラの座標
	posA = player_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets_)
	{
		posB = bullet->GetWorldPosition();
		float ebRadius = bullet->GetRadius();
		//座標Aと座標Bの距離を求める
		//弾と弾の交差判定
		if (
			((posB.x - posA.x) * (posB.x - posA.x)) +
			((posB.y - posA.y) * (posB.y - posA.y)) +
			((posB.z - posA.z) * (posB.z - posA.z)) <=
			((pRadius + ebRadius) * (pRadius + ebRadius)))
		{
			//コールバックを呼び出す
			player_->OnCollision();
			bullet->OnCollision();
		}
	}


	//自キャラの座標
	posA = player_->GetWorldPosition();
	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<Turning>& bullet : turning_)
	{
		posB = bullet->GetWorldPosition();
		float ebRadius = bullet->GetRadius();
		//座標Aと座標Bの距離を求める
		//弾と弾の交差判定
		if (
			((posB.x - posA.x) * (posB.x - posA.x)) +
			((posB.y - posA.y) * (posB.y - posA.y)) +
			((posB.z - posA.z) * (posB.z - posA.z)) <=
			((pRadius + ebRadius) * (pRadius + ebRadius)))
		{
			//コールバックを呼び出す
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

#pragma region 敵キャラと自機の弾の当たり判定
	//敵キャラの座標
	posA = enemy_->GetWorldPosition();
	//敵キャラと自弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		posB = bullet->GetWorldPosition();
		float pbRadius = bullet->GetRadius();
		//座標Aと座標Bの距離を求める
		//球と球
		if (
			((posB.x - posA.x) * (posB.x - posA.x)) + ((posB.y - posA.y) * (posB.y - posA.y)) +
			((posB.z - posA.z) * (posB.z - posA.z)) <=
			((eRadius + pbRadius) * (eRadius + pbRadius))) {
			//コールバックを呼び出す
			enemy_->OnCollision(); // 一緒に体力も減らしてる
			bullet->OnCollision();
		}
	}
	
	
#pragma endregion

//#pragma region 自弾と敵弾の当たり判定
//
//	//自弾の座標
//	for (const std::unique_ptr<PlayerBullet>& pBullet : playerBullets) {
//		posA = pBullet->GetWorldPosition();
//		float pbRadius = pBullet->GetRadius();
//		//敵弾の座標
//		for (const std::unique_ptr<EnemyBullet>& eBullet : enemyBullets_) {
//			posB = eBullet->GetWorldPosition();
//			float ebRadius = eBullet->GetRadius();
//			//球と球
//			if (
//				((posB.x - posA.x) * (posB.x - posA.x)) +
//				((posB.y - posA.y) * (posB.y - posA.y)) +
//				((posB.z - posA.z) * (posB.z - posA.z)) <=
//				((ebRadius + pbRadius) * (ebRadius + pbRadius)))
//			{
//				//コールバックを呼び出す
//				pBullet->OnCollision();
//				eBullet->OnCollision();
//			}
//		}
//	}
//
//#pragma endregion
}