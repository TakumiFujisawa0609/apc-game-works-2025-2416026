#pragma once
#include "../Common/Quaternion.h"
#include <DxLib.h>
#include <vector>
#include <memory>

class Player;
class StageTutorial;


class Camera
{
public:

	/// @brief カメラ状態
	enum class MODE
	{
		NONE = -1,
		FIXEX_POINT,      // 定点カメラモード
		FOLLOW,           // 追従モード
		FOLLOW_AUTO_ZOOM, // 追従モード[自動調整型]
	};

	// カメラの初期角度
	//static constexpr VECTOR DEFAULT_ANGLES = { (0.0f * (DX_PI_F / 180.0f)), 0.0f, 0.0f };

	// カメラのローカル座標
	static constexpr VECTOR LOCAL_CAMERA_POS = { 0.0, 400.0f, -1000.0f };

	// 追尾位置のローカル座標
	static constexpr VECTOR TARGET_LOCAL_POS = { 0.0f, 300.0f, 0.0f };


	// カメラ最小描画領域
	static constexpr float CAMERA_NEAR = 20.0f;

	// カメラ最大描画領域
	static constexpr float CAMERA_FAR = 10000.0f;


	// 最大ズームインローカル座標
	static constexpr VECTOR ZOOM_IN_MAX = { 0.0f, 100.0f, 650.0f };

	// 最大ズームアウトローカル座標
	static constexpr VECTOR ZOOM_OUT_MAX = { 0.0f, 2000.0f, 2500.0f };

	// ズーム速度
	static constexpr float ZOOM_SPEED = 2.0f;

	// カメラ移動倍率(0～1.0)
	static constexpr float CAMERA_FOLLOW_SPEED = 0.0f;

	// 注視点への視点移動速度(0～1.0)
	static constexpr float CAMERA_LOOK_SPEED = 0.0f;


	/// @brief デフォルトコンストラクタ
	Camera(void);

	/// @brief デフォルトデストラクタ
	~Camera(void) = default;


	/// @brief 初回読み込み処理
	void Load(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <para name="mode">カメラ状態</param>
	/// <param name="pos">位置</param>
	/// <param name="angleY">角度</param>
	void Init(MODE mode, const VECTOR& pos = {}, float angleY = 0.0f, Player* player = nullptr);

	/// <summary>
	/// 描画処理
	/// </summary>
	void SetBeforeDraw(void);
	
	/// @brief 描画処理
	void DrawDebug(void);

	/// @brief メモリ解放処理
	void Release(void);


	void UpdatePlayerTransform(VECTOR* pos, Quaternion* rot);

	/// <summary>
	/// 追従対象の初期化
	/// </summary>
	void ResetTrackingTarget(void);

	/// <summary>
	/// カメラ位置取得
	/// </summary>
	/// <returns>現在カメラ位置</returns>
	const VECTOR& GetPos(void) const { return pos_.camera;  };

	/// <summary>
	/// カメラ位置割り当て
	/// </summary>
	/// <param name="pos">移動する位置</param>
	/// <param name="angle">回転する向き</param>
	/// <param name="cameraMode">カメラ状態指定</param>
	/// <param name="freeTime">追尾しない時間</param>
	/// <param name="targetPos">追尾位置</param>
	void SetCameraPos(const VECTOR& pos, const VECTOR& angle, MODE cameraMode,
					  float freeTime = 0.0f, const VECTOR& targetPos = {});

	/// <summary>
	/// カメラモード変更
	/// </summary>
	/// <param name="mode"></param>
	/// <param name="pos"></param>
	void SetCameraMode(MODE mode, const VECTOR& pos = {});

	/// <summary>
	/// 追従対象の設定
	/// </summary>
	/// <param name="target">追尾対象の座標</param>
	void SetTrackingTarget(VECTOR* target);
	
	//対象同士の中間座標
	void SetCameraTarget(void);

	/// @brief カメラ移動制限割り当て
	void SetPosLimit(const VECTOR& _min, const VECTOR& _max);

	const VECTOR& GetCameraPos(void) const { return pos_.camera; }

	const VECTOR& GetCameraAngles(void)const { return rot_.camera.ToEuler(); }
	const Quaternion& GetCameraRot(void)const { return rot_.camera; }

	bool GetIsCameraClip(const VECTOR& _target);

private:

	// カメラ状態
	MODE mode_;

	Player* follow_;

	std::vector<VECTOR*> targetsPos_;
	//std::shared_ptr<VECTOR> target_;

	struct Zoom
	{
		float maltiple; // ズーム倍率
		VECTOR pos;
		VECTOR distance; // 注視点からカメラ位置までの距離
	};
	Zoom zoom_;

	struct Pos
	{
		VECTOR camera;	// カメラ位置

		VECTOR target; // カメラ注視点
		VECTOR playerPos; // プレイヤー位置

		VECTOR limitMin;
		VECTOR limitMax;

		VECTOR moveMaltiple; // 移動倍率
	};
	Pos pos_;
	
	struct Rotation
	{
		Quaternion camera; // 回転量

		Quaternion target; // 注視点の回転度
		Quaternion* player; // プレイヤーの回転度
	};
	Rotation rot_;


	// 追尾しない時間
	float freeTime_;



	/// <summary>
	/// ズーム倍率割り当て
	/// </summary>
	/// <param name="pos">２つの座標間のベクトル</param>
	void _SetZoomDiff(const VECTOR& vecDiff);


	// カメラ移動処理
	void DebugMove(void);

	/// <summary>
	/// 定点カメラ処理
	/// </summary>
	void SetBeforeDraw_FixexPoint(void);

	void SetBeforeDraw_Follow(void);

	/// @brief 滑らかに追尾回転する処理
	void SmoothRotation(void);

	/// @brief カメラ回転処理
	void _UpdateCameraRot(void);

	void SetBeforeDraw_FollowZoom(void);
};