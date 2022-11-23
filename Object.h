#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <cassert>
#include "affin/affin.h"

class Object
{
public:
	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection viewProjection_, uint32_t textureHandle);
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

	// Affin関数の構造体
	affin::AffinMat affinMat;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
};

