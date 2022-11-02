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

namespace MyFunc
{
	//���W�A���ϊ�
	float Deg2Rad(float x);

	//�x�ϊ�
	float Rad2Deg(float x);

	void UpdateWorldTransform(WorldTransform& worldTransform);
};

