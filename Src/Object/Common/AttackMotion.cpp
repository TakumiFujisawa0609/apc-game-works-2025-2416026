#include "AttackMotion.h"
#include <DxLib.h>
#include <unordered_map>
#include "../../Manager/SceneManager.h"


AttackMotion::AttackMotion(void)
{
	attackState_ = ATTACK_STATE::NONE;
	curTimeAction_ = timeAttack_ = 0.0f;
	for (float time : timeActions_) { time = 0.0f; }
	isConti_ = false;
}

void AttackMotion::Init(void)
{
	attackState_ = ATTACK_STATE::NONE;
	curTimeAction_ = timeAttack_ = 0.0f;
	for (float time : timeActions_) { time = 0.0f; }
}

void AttackMotion::Update(void)
{
	if (attackState_ != ATTACK_STATE::NONE)
	{
		ChangeAction();
	}

	// 攻撃開始
	if (attackState_ == ATTACK_STATE::ACTIVE)
	{
		UpdateActive();
	}

	// 入力
	else if (attackState_ == ATTACK_STATE::INPUT)
	{
		UpdateInput();
	}
	else if (attackState_ == ATTACK_STATE::END)
	{
		UpdateEnd();
	}
}

void AttackMotion::Draw(void)
{
}


bool AttackMotion::GetIsActionAttack(void) const
{
	/*　攻撃したか判定　*/
	bool ret = false;

	if (attackState_ == ATTACK_STATE::ACTIVE &&
		timeAttack_ < DAMAGE_ACTIVE_TIME)
	{
		ret = true;
	}
	return ret;
}


void AttackMotion::_SetMotionTime(float _activeTime, float _endTime, float _atkTime, bool _isConti, float _inputTime)
{
	int state = -1;

	// モーション有効時間
	state = static_cast<int>(ATTACK_STATE::ACTIVE);
	timeActions_[state] = _activeTime;

	// 入力猶予時間
	state = static_cast<int>(ATTACK_STATE::INPUT);
	timeActions_[state] = _inputTime;

	// モーション終了時間
	state = static_cast<int>(ATTACK_STATE::END);
	timeActions_[state] = _endTime;

	// 攻撃間隔
	timeAtkMax_ = _atkTime;

	// 連続フラグ割り当て
	isConti_ = _isConti;

	attackState_ = ATTACK_STATE::ACTIVE;
	curTimeAction_ = _activeTime;
}


void AttackMotion::UpdateActive(void)
{
	// 攻撃間隔時間を減少する
	float delta = SceneManager::GetInstance().GetDeltaTime();
	timeAttack_ -= ((timeAttack_ >= 0.0f) ? delta : 0.0f);

	// 攻撃間隔が０の時、最大時間に戻す
	timeAttack_ = ((timeAttack_ < 0.0f) ? timeAtkMax_ : timeAttack_);
}

void AttackMotion::UpdateInput(void)
{
}

void AttackMotion::UpdateEnd(void)
{
}

void AttackMotion::ChangeAction(void)
{
	/*　モーション状態遷移　*/
	float delta = SceneManager::GetInstance().GetDeltaTime();

	curTimeAction_ -= ((attackState_ != ATTACK_STATE::NONE) ? delta : 0.0f);

	if (curTimeAction_ < 0.0f)
	{
		// 状態を進め、終了時に無効状態化
		int state = static_cast<int>(attackState_) + 1;
		state = ((state == static_cast<int>(ATTACK_STATE::MAX)) ? -1 : state);

		// 状態割り当て
		attackState_ = static_cast<ATTACK_STATE>(state);
		curTimeAction_ = ((state != -1) ? timeActions_[state] : curTimeAction_);
	}
}
