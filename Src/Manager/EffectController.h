#pragma once
#include <DxLib.h>
#include <vector>
#include <map>


class EffectController
{
public:

	//エフェクトの種別
	enum class EFFECT_TYPE
	{
		NONE = -1,
		SWORD_SLASH,
	};

	/// @brief デフォルトコンストラクタ
	EffectController(void);

	/// @brief 通常デストラクタ
	~EffectController(void) = default;


	/// @brief 初期化処理
	void Init(void);

	/// @brief 更新処理
	void Update(void);

	/// @brief 描画処理
	void Draw(void);

	/// @brief エフェクト停止処理
	void StopEffect(EFFECT_TYPE type);

	/// @brief エフェクト種別変更関数
	void ChangeEffect(EFFECT_TYPE type, VECTOR pos);


	//void CreateEffect(EFFECT_TYPE _type);

private:

	struct Effect
	{
		VECTOR pos;
		VECTOR size;
		VECTOR angle;

		int handle;
		int playId;
		float aliveTime; // 有効時間
	};
	std::map<EFFECT_TYPE, Effect*> effects_;
	
	//EffectBase情報格納
	//EffectBase* effect_;


	bool GetIsEffectAlive(EFFECT_TYPE type);
};
