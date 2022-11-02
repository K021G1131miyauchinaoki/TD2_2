#pragma once
#include <cassert>
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin/affin.h"

//天球
class skydome
{
public:
	//初期化
	void Initialize(Model* model);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection viewProjection_);
private:
	//ワールド変換データ
	WorldTransform worldTransform_;


	//ビュープロジェクション
	ViewProjection viewProjection_;

	//モデル
	Model* model_ = nullptr;

	// Affin関数の構造体
	affin::AffinMat affinMat;
};

