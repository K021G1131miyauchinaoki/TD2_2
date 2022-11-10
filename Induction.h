#pragma once
#include "model.h"
#include "math.h"
#include <cassert>
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin/affin.h"
#include "Player/Player.h"

class Induction
{
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity,Vector3& angle);
	void Update();
	void Draw(const ViewProjection& viewProjection);


private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;
	//角度
	float angle_;
	//半径
	float radius = 1.0f;
};

