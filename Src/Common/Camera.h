#pragma once
#include "../Common/Quaternion.h"
#include <DxLib.h>
#include <vector>
#include <memory>

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
		NONE = -1,
		FIXEX_POINT, // 定点カメラモード
		FLLOW,       // 追従モード
		FOLLOW_AUTO_ZOOM,  // 追従モード[自動調整型]
	};

	// カメラの初期角度
	static constexpr VECTOR DEFAULT_ANGLES =
	{ (30.0f * (DX_PI_F / 180.0f)), 0.0f, 0.0f };

	// カメラ最小描画領域
	static constexpr float CAMERA_NEAR = 20.0f;

	// カメラ最大描画領域
	static constexpr float CAMERA_FAR = 4000.0f;


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

	static constexpr VECTOR CAMERA_DISTANCE = { 0.0, 300.0f, -850.0f };


	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static Camera& GetInstance(void) { return *instance_; };


	/// <summary>
	/// 初回読み込み処理
	/// </summary>
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
	
	/// <summary>
	/// 描画処理
	/// </summary>
	void DrawDebug(void);

	/// <summary>
	/// メモリ解放処理
	/// </summary>
	void Release(void);

	/// <summary>
	/// インスタンス削除
	/// </summary>
	void Destroy(void);


	void UpdatePlayerTransform(VECTOR* pos, Quaternion* rot);

	/// <summary>
	/// 追従対象の初期化
	/// </summary>
	void ResetTrackingTarget(void);

	/// <summary>
	/// カメラ位置取得
	/// </summary>
	/// <returns>現在カメラ位置</returns>
	const VECTOR& GetPos(void) const { return pos_.cameraPos;  };

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

	/// <summary>
	/// カメラ移動制限割り当て
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	void SetPosLimit(VECTOR min, VECTOR max);

	const VECTOR& GetCameraPos(void) const { return pos_.cameraPos; }

private:

	static Camera* instance_;

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
		VECTOR cameraPos;	// カメラ位置

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
	


	// デフォルトコンストラクタ
	Camera(void);

	// デフォルトデストラクタ
	~Camera(void) = default;

	// デフォルトコピーコンストラクタ
	Camera(const Camera& other) = default;


	/// <summary>
	/// ズーム倍率割り当て
	/// </summary>
	/// <param name="pos">２つの座標間のベクトル</param>
	void _SetZoomDiff(const VECTOR& vecDiff);

	/// <summary>
	/// カメラ位置制限
	/// </summary>
	void PosLimit(void);

	// カメラ移動処理
	void DebugMove(void);

	/// <summary>
	/// 定点カメラ処理
	/// </summary>
	void SetBeforeDraw_FixexPoint(void);

	void SetBeforeDraw_Follow(void);
	void BeforeDrawProc(void);
	void _UpdateCameraRot(void);

	void SetBeforeDraw_FollowZoom(void);
};