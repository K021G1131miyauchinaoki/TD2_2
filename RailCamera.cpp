#include "RailCamera.h"

using namespace MathUtility;

const	Vector3	RailCamera::lerp(const	Vector3& start, const	Vector3& end, const	float	t)
{
	//float	y=t;
	//return	start*(1.0f-y)+end*y;
	return	start * (1.0f - t) + end * t;
}

const Vector3 RailCamera::CatmullRomSpline(Vector3 P0, Vector3 P1, Vector3 P2, Vector3 P3, float t)
{
	Vector3 a = (2 * P1) + ((-P0 + P2) * t);

	Vector3 b = ((2 * P0) - (5 * P1) + (4 * P2) - P3) * t * t;

	Vector3 c = (-P0 + (3 * P1) - (3 * P2) + P3) * t * t * t;

	return 0.5f * (a + b + c);
}

//制御点の集合(vectorコンテナ),補間する区間の添字、時間経過率
Vector3	RailCamera::splinePosition(const std::vector<Vector3>& points, size_t startIndex, float t) {
	//補間すべき点の数
	size_t n = points.size() - 2;

	if (startIndex > n)return points[n];//pnの値を返す
	if (startIndex < 1)return points[1];//p1の値を返す

	//p0～p3の制御点を取得する※p1～p2を補間する
	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	//Catmull-Romの式による補間
	Vector3 position = CatmullRomSpline(p0, p1, p2, p3, t);

	return position;

}

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	input_ = Input::GetInstance();
	//シングルインスタンスを取得する
	debugText_ = DebugText::GetInstance();

	//ワールドトランスフォームの初期設定(座標と回転角)
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	worldTransform_.Initialize();
	//ビュープロジェクションの初期化
	//viewProjection_.farZ = 300.0f;
	viewProjection_.Initialize();
	//先頭と最後に制御点を追加している
	//							p0    p1    p2 p3 p4  p5
	std::vector<Vector3>points{ start,start,p2,p3,end,end };
	//p1からスタートする
	size_t startIndex = 1;
}

void RailCamera::Update(int num)
{
	if (isMovei) {
		if (num == 0)
		{
			worldTransform_.translation_ = Vector3(0, 50, 0);
			isMovei = false;
		}
	}
	else
	{
		isMovei = true;
		if (num == 1)//登場
		{
			worldTransform_.translation_ = Vector3(0, 0, -20);
			//worldTransform_.rotation_ = Vector3(0, 0, 5);
		}
		else if (num == 2)//形態変化1
		{
			worldTransform_.translation_ = Vector3(0, 0, -20);
		}
		else if (num == 3)//形態変化2
		{
			worldTransform_.translation_ = Vector3(0, 50, 30);
		}
	}
	
	Vector3 move = { 0, 0, 0 };
	

	const float kCharaSpeed = 0.2f;

	Matrix4 matTranslate;

	worldTransform_.matWorld_ = MathUtility::Matrix4Identity();

#pragma region 移動処理
	if (num==1)
	{
		timerTrans++;
		timeRate = timerTrans / timerTransMax;
		if (timeRate>=1.0f)
		{
			if (startIndex < points.size() - 3)
			{
				startIndex++;
				timerTrans -= timerTransMax;
				timeRate -= 1.0f;
			}
			else
			{
				timerTrans = timerTransMax;
				timeRate = 1.0f;
			}
		}
		worldTransform_.translation_ = splinePosition(points, startIndex, timeRate);
	}

	if (input_->PushKey(DIK_M)) {
		angle+=0.01f;
		radius = angle * 3.14f / 180.0f;
		add_x = cos(radius)*len;
		add_z = sin(radius)*len;
		rotMove.x = pos.x + add_x;
		rotMove.z = pos.z + add_z;
		rotMove.y = pos.y;
		worldTransform_.translation_ = rotMove;	
	}
	if (input_->PushKey(DIK_A)) {
		move.x -= kCharaSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += kCharaSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharaSpeed;
	}
	if (input_->PushKey(DIK_S)) {
		move.y -= kCharaSpeed;
	}
	if (input_->PushKey(DIK_Z)) {
		move.z -= kCharaSpeed;
	}
	if (input_->PushKey(DIK_X)) {
		move.z += kCharaSpeed;
	}
	////移動限界座標
	//const float kMoveLimitX = 36.0f;
	//const float kMoveLimitY = 20.0f;

	worldTransform_.translation_ += move;
	//
	////範囲を超えない処理
	//worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	//worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	//worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	//worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	matTranslate = affin::generateTransMat(worldTransform_);
#pragma endregion

#pragma region 回転処理
	Vector3 Rotation = { 0, 0, 0 };
	const float kCharaRotX = MyFunc::Deg2Rad(0.5f);

	Matrix4 matRotX;
	Matrix4 matRotY;

	//回転処理
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.x += kCharaRotX;
	}
	if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.x -= kCharaRotX;
	}
	if (input_->PushKey(DIK_R)) {
		worldTransform_.rotation_.y += kCharaRotX;
	}
	if (input_->PushKey(DIK_T)) {
		worldTransform_.rotation_.y -= kCharaRotX;
	}
	//(-)2πを超えたら回転角を0に戻す処理
	if (worldTransform_.rotation_.x >= (M_PI * 2) || worldTransform_.rotation_.x <= -(M_PI * 2)) {
		worldTransform_.rotation_.x = 0;
	}
	if (worldTransform_.rotation_.y >= (M_PI * 2) || worldTransform_.rotation_.y <= -(M_PI * 2)) {
		worldTransform_.rotation_.y = 0;
	}
	matRotX = affin::generateRotateXMat(worldTransform_);
	matRotY = affin::generateRotateYMat(worldTransform_);
#pragma endregion

	worldTransform_.matWorld_.operator*=(matRotX);
	worldTransform_.matWorld_.operator*=(matRotY);
	worldTransform_.matWorld_.operator*=(matTranslate);

	//行列の転送
	worldTransform_.TransferMatrix();

	viewProjection_.eye = worldTransform_.translation_;
	//前方ベクトル
	Vector3 forward;
	if (num == 0)
	{
		forward = viewProjection_.eye + Vector3{ 0, -1, 0 };
	}
	else if (num == 1)//登場
	{
		forward =  Vector3{ 0, 0, 28 };
		//viewProjection_.fovAngleY=20.0f * MathUtility::PI / 180.0f;;
	}
	else if (num == 2)//形態変化1
	{
		forward = { 0, -1, -1 };
	}
	else if (num == 3)//形態変化2
	{
		forward = { 0, -1, 2 };
	}
	//レールカメラの回転を反映
	forward = affin::matVector(forward, worldTransform_.matWorld_);

	//視点から前方に適当に進んだ位置が注視点
	viewProjection_.target =  forward;

	//上方ベクトル
	Vector3 up(0, 1, 0);
	//
	viewProjection_.up = affin::matVector(up, worldTransform_.matWorld_);

	//ビュープロジェクションの更新と行列バッファへの転送
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();

	//eyeの表示
	/*debugText_->SetPos(50, 110);
	debugText_->Printf(
		"%f,%f,%f", viewProjection_.eye.x, viewProjection_.eye.y,
		viewProjection_.eye.z);
	debugText_->SetPos(50, 130);
	debugText_->Printf(
		"%f,%f,%f", worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z);
	debugText_->SetPos(50, 150);
	debugText_->Printf(
		"%f,%f,%f", add_x,add_z,angle);
	debugText_->SetPos(50, 170);
	debugText_->Printf(
		"%f,%f,%f", rotMove.x, rotMove.z, angle); 
		debugText_->SetPos(50, 190);
	debugText_->Printf("%f", timeRate);
}

WorldTransform* RailCamera::GetWorldPosition()
{
	return &worldTransform_;
}