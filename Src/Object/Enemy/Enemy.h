#pragma once
#include "../Object.h"
#include "../Status/StatusEnemy.h"
#include <string>
class StatusEnemy;
class Player;

class Enemy : public Object
{
public:

	enum class ACTION_STATE
	{
		NONE = -1,
		INACTIVE, // 非表示状態
		SPAWN,    // 生成状態
		IDLE,	  // 待機状態
		SEARCH,   // プレイヤー追従
		MOVE,     // 移動状態
		ATTACK_START, 
		ATTACK_ACTIVE,
		ATTACK_END,
		MAX,
	};

	static constexpr float LOCAL_ANGLE_Y = (180.0f * (DX_PI_F / 180.0f));
	
	
	Enemy(StatusEnemy::TYPE type, Player& players);

	/// @brief デフォルトデストラクタ
	virtual ~Enemy(void) = default;


	/// @brief 初回読み込み処理
	/// @param  
	void Load(void)override;

	void Init(const VECTOR& pos, float angleY = 0.0f, ACTION_STATE state = ACTION_STATE::IDLE);

	void Draw(void)override;

	void Update(void)override;

	void Release(void)override;


	void SetParam(void) override;

	/// @brief 行動状態取得
	ACTION_STATE GetActionState(void)const { return paramEnemy_.actionState; };

	bool GetIsActive(void)const { return (paramEnemy_.actionState != ACTION_STATE::INACTIVE);  };

	/// <summary>
	/// 敵表示処理
	/// </summary>
	/// <param name="_isView">表示するか否か</param>
	void SetIsView(bool _isView);

	

protected:

	struct EnemyParam
	{
		std::string name;

		ACTION_STATE actionState;

		StatusEnemy::TYPE type;

		float curAtkInterval;

		float atkRange;

		float searchRange;

		// 索敵有効フラグ
		bool isHearing;
	};

	EnemyParam paramEnemy_;

	

	StatusEnemy& status_;

	Player& player_;


	void LoadResource(void);

	/// @brief プレイヤーの方向を追尾
	void LookPlayerPos(void);


	virtual void InitModelFrame(void)override = 0;

	virtual void InitAnim(void) = 0;

	void UpdateAnim(void)override;

	void SearchField(void);
	void DrawSearchRange(void);

	void UpdateState(void);
	virtual void UpdateStateIdle(void);
	virtual void UpdateStateSpawn(void);
	virtual void UpdateStateSearch(void);
	virtual void UpdateStateMove(void);
	virtual void UpdateStateAtk(void);

	void ChangeActionState(ACTION_STATE state);
};