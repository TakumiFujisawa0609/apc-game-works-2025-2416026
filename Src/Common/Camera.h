#pragma once
#include "../Utility/Quaternion.h"
#include <DxLib.h>
#include <vector>

class Player;
class StageTutorial;

/// <summary>
/// カメラ
/// </summary>
class Camera
{
public:

	/// <summary>
	/// カメラ状態
	/// </summary>
	enum class MODE
	{
		NONE,
		FIXEX_POINT,  // 定点カメラモード
		FLLOW,        // 追従モード
	};

	// カメラ移動速度
	static constexpr float SPEED =100.0f;

	// カメラの初期角度
	static constexpr VECTOR DEFAULT_ANGLES =
	{ 30.0f * (DX_PI_F / 180.0f), 0.0f, 0.0f };

	// カメラ最小描画領域
	static constexpr float CAMERA_NEAR = 20.0f;

	// カメラ最大描画領域
	static constexpr float CAMERA_FAR = 4000.0f;


	// 最大ズームインY
	static constexpr float ZOOM_IN_MAX_Y = 500.0f;
	// 最大ズームインZ
	static constexpr float ZOOM_IN_MAX_Z = 650.0f;
	// 最大ズームアウトY
	static constexpr float ZOOM_OUT_MAX_Y = 800.0f;
	// 最大ズームアウトZ
	static constexpr float ZOOM_OUT_MAX_Z = 2500.0f;

	// ズーム速度
	static constexpr float ZOOM_SPEED = 2.0f;


	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	Camera(void);

	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	~Camera(void) = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(void);
	
	/// <summary>
	/// 描画処理
	/// </summary>
	void DrawDebug(void);

	/// <summary>
	/// メモリ解放処理
	/// </summary>
	void Release(void);


	/// <summary>
	/// 追従対象の初期化
	/// </summary>
	void ResetTrackingTarget(void);

	/// <summary>
	/// カメラ位置取得
	/// </summary>
	/// <returns>現在カメラ位置</returns>
	VECTOR GetPos(void) const;

	/// <summary>
	/// ズームイン
	/// </summary>
	void ZoomIn(void);

	/// <summary>
	/// ズームアウト
	/// </summary>
	void ZoomOut(void);

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
	/// 追従対象の設定
	/// </summary>
	/// <param name="target">追尾対象の座標</param>
	void SetTrackingTarget(VECTOR* target);
	
	//対象同士の中間座標
	void SetCameraTarget(void);
	
	/// <summary>
	///対象同士の中間座標リストに割り当て
	/// </summary>
	void SetCameraTargetList(VECTOR* pos);

	/// <summary>
	/// カメラ移動制限割り当て
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	void SetPosLimit(VECTOR min, VECTOR max);


private:

	// カメラ状態
	MODE mode_;


	std::vector<VECTOR*> targetsPos_;

	VECTOR pos_;	// カメラ位置
	float zoomPos_; // ズーム位置
	VECTOR targetPos_; // カメラ注視点
	Quaternion rot_; // 回転度

	// 追尾しない時間
	float freeTime_;

	VECTOR zoomPos;
	VECTOR zoomPow;

	VECTOR posLimitMin_;
	VECTOR posLimitMax_;

	VECTOR* trackingTarget_;

	/// <summary>
	/// ズーム倍率割り当て
	/// </summary>
	/// <param name="pos">２つの座標間のベクトル</param>
	void _SetZoomDiff(const VECTOR& vecDiff);

	/// <summary>
	/// カメラ位置制限
	/// </summary>
	void PosLimit(void);
};