#pragma once
#include <DxLib.h>
#include <vector>
#include <map>


class EffectManager
{
public:

	//エフェクトの種別
	enum class EFFECT_TYPE
	{
		NONE = -1,
		COTTON_DAMAGE,
		COTTON_KNOCK,
		STAR,
	};


	/// <summary>
	/// インスタンスを明示的に生成
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns>入力マネージャ</returns>
	static EffectManager& GetInstance(void);

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
	/// 解放処理
	/// </summary>
	void Destroy(void);

	/// <summary>
	/// ストップ処理
	/// </summary>
	void StopEffect(EFFECT_TYPE type);

	/// <summary>
	///エフェクト種別変更関数
	/// </summary>
	void ChangeEffect(EFFECT_TYPE type, VECTOR pos);


private:

	struct EFFECT
	{
		VECTOR pos;
		VECTOR size;
		VECTOR angle;

		int handle;
		int playId;
		float aliveTime; // 有効時間
	};
	std::map<EFFECT_TYPE, EFFECT> effects_;
	

	// シングルトンインスタンス
	static EffectManager* instance_;

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	EffectManager(void);

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	EffectManager(const EffectManager& other) = default;

	/// <summary>
	/// 通常デストラクタ
	/// </summary>
	~EffectManager(void) = default;

	//EffectBase情報格納
	//EffectBase* effect_;



	bool GetIsEffectAlive(EFFECT_TYPE type);
};
