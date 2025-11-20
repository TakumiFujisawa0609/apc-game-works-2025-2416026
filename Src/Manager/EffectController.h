#pragma once
#include <DxLib.h>
#include <vector>
#include <map>
#include "../Utility/AsoUtility.h"


class EffectController
{
public:

	//エフェクトの種別
	enum class EFFECT_TYPE
	{
		NONE = -1,
		SPAWN_SKELETON, // 魔法陣
		SWORD_SLASH,
		SWORD_HIT,
	};

	/// @brief デフォルトコンストラクタ
	EffectController(void);

	/// @brief 通常デストラクタ
	~EffectController(void) = default;


	/// @brief 初期化処理
	void Init(void);

	/// @brief 更新処理
	void Update(void);

	/// @brief エフェクト停止処理
	void StopEffect(EFFECT_TYPE type);

	
	/// @brief エフェクト種別変更関数
	/// @param _type エフェクトの種類
	/// @param _pos 位置
	/// @param _angle 回転
	/// @param _scale スケール
	void SetEffect(EFFECT_TYPE _type, const VECTOR& _pos,
				   const VECTOR& _angle = AsoUtility::VECTOR_ZERO, const VECTOR& _scale = AsoUtility::VECTOR_ONE);


	//void CreateEffect(EFFECT_TYPE _type);

private:

	struct Effect
	{
		VECTOR pos;
		VECTOR scale;
		VECTOR angle;

		int handle;
		int playId;
		EFFECT_TYPE type;
		float aliveTime; // 有効時間
	};
	std::map<EFFECT_TYPE, Effect*> effects_;
	
	//EffectBase情報格納
	//EffectBase* effect_;


	bool GetIsEffectAlive(EFFECT_TYPE type);

	void UpdateEffect(EFFECT_TYPE _type, Effect& _effect);
};
