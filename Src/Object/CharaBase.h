#pragma once
#include "DxLib.h"

class CharaBase
{
public:

	/// <summary>
	/// 行動状態
	/// </summary>
	enum class STATE
	{
		NONE = 0,
	};

	

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	CharaBase(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CharaBase(void) = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init(void);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	virtual void Release(void);

	/// <summary>
	/// パラメータ割り当て
	/// </summary>
	virtual void SetParam(void);

protected:
	
	/// <summary>
	/// プレイヤーのパラメータ
	/// </summary>
	struct Parameter
	{
		STATE state;	 // 行動状態
		int hp;			 // HP
		float moveSpeed; // 移動速度
		VECTOR pos;		 // 位置
		VECTOR dir;		 // 移動方向
	};

	// 状態
	Parameter param_; 
};