#pragma once
#include <cassert>
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "affin/affin.h"
#include "Input.h"
#include "DebugText.h"
#include "MyFunc.h"

class RailCamera
{
public:
	//初期化
	void Initialize(const Vector3& position, const Vector3& rotation);

	//更新
	void Update(int num);

	const ViewProjection& GetViewProjection() 
	{ 
		return viewProjection_;
	}

	//ワールド座標の取得
	WorldTransform* GetWorldPosition();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	Input* input_ = nullptr;

	DebugText* debugText_ = nullptr;

	const float M_PI = 3.141592f;
};