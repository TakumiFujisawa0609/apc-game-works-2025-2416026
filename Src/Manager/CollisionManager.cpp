#include "CollisionManager.h"
#include <DxLib.h>
#include <map>

#include "../Common/Vector2.h"
#include "../Utility/AsoUtility.h"
#include "../Utility/UtilityCollision.h"
#include "../Object/Object.h"
#include "../Object/Player.h"
#include "../Object/Enemy/EnemyController.h"
#include "../Object/Enemy/Enemy.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/EffectManager.h"

// シングルトンクラス
CollisionManager* CollisionManager::instance_ = nullptr;

void CollisionManager::CreateInstance(Player& _player)
{
	if (instance_ == nullptr)
	{
		instance_ = new CollisionManager(_player);
	}

	instance_->player_ = &_player;
	//instance_->Init();
}

CollisionManager::CollisionManager(Player& _player)
{
	player_ = &_player;
	stageColHandle_ = -1;
	stageDamageHandle_ = -1;

	// リストクリア
	//colChara_.clear();
	//collisionStage_.clear();
}




void CollisionManager::Init(int& stageHandle, const VECTOR& stagePos, const VECTOR& stageScale,
							int& damageHandle, const VECTOR& damagePos, const VECTOR& damageScale)
{
	//colChara_.clear();


	// 当たり判定割り当て
	SetStageCollision(stageHandle, stagePos, stageScale,
					  damageHandle, damagePos, damageScale);
}
/*
void CollisionManager::SetCharaCollision(CollisionManager::COL_TARGET target, CharaBase& chara)
{
	int num = static_cast<int>(target);

	// 当たり判定をリストに格納
	colChara_.emplace(num, &chara);
}*/
void CollisionManager::SetStageCollision(int& stageHandle, const VECTOR& stagePos, const VECTOR& stageScale,
										 int& damageHandle, const VECTOR& damagePos, const VECTOR& damageScale)
{
	// 当たり判定用ハンドル読み込み
	stageColHandle_ = stageHandle;

	// ステージ当たり判定割り当て
	MV1SetupCollInfo(stageColHandle_, -1);

	//ステージサイズ設定
	MV1SetScale(stageColHandle_, stageScale);

	//ステージ座標設定
	MV1SetPosition(stageColHandle_, stagePos);


	// ダメージ領域の当たり判定ハンドル読み込み
	if (damageHandle != -1)
	{
		stageDamageHandle_ = damageHandle;

		// ダメージ領域の当たり判定割り当て
		MV1SetupCollInfo(stageDamageHandle_);

		// ダメージ領域のサイズ設定
		MV1SetScale(stageDamageHandle_, damageScale);

		// ダメージ領域の座標設定
		MV1SetPosition(stageDamageHandle_, damagePos);
	}
}


void CollisionManager::Update(void)
{
	// プレイヤーと敵の当たり判定
	CollisionEnemys();

	// キャラクター同士の当たり判定
	CollisionChara();

	// 地面当たり判定
	CollisionsGround();

	// 壁当たり判定
	CollisionsWall();

	// ダメージ領域当たり判定
	CollisionsStageDamage();
}

void CollisionManager::DrawDebug(void)
{
#ifdef _DEBUG
	/*
	for (auto& chara : colChara_)
	{
		chara.second->GetCollision().DrawDebug();
	}*/

	if (stageColHandle_ != -1)
	{
		//MV1DrawModel(stageColHandle_);
	}
	
	if (stageDamageHandle_ != -1)
	{
		//MV1DrawModel(stageDamageHandle_);
	}

#endif // _DEBUG
}

void CollisionManager::Destroy(void)
{
	// リストクリア
	//colChara_.clear();
	if (instance_ != nullptr)
	{
		delete instance_;

		instance_ = nullptr;
	}
}


void CollisionManager::CollisionChara(void)
{
	/*
	SceneManager& scene = SceneManager::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();
	EffectManager& effect = EffectManager::GetInstance();

	int max = static_cast<int>(COL_NUM::MAX);
	int checkNum  = 0; // 判定対象
	int targetNum = 0; // 検索対象
	Vector2 weightKnock = {}; // ふっとばし値


	for (auto& check : colChara_)
	{
		for (auto& target : colChara_)
		{

			// 判定対象が捕まれている時、スキップ
			if (checkChara.GetActionState() == ACTION_STATE::CAUGHT)
			{
				targetNum++;
				continue;
			}


			// キャラ同士の当たり判定
			if (checkChara.GetIsHit() && targetChara.GetIsHit())
			{
				VECTOR checkTop = checkCol.GetColPos(COL_NUM::BODY_TOP);
				VECTOR checkBot = checkCol.GetColPos(COL_NUM::BODY_BOTTOM);
				float checkRad  = checkCol.GetColScale(COL_NUM::BODY).x;

				VECTOR targetTop = targetCol.GetColPos(COL_NUM::BODY_TOP);
				VECTOR targetBot = targetCol.GetColPos(COL_NUM::BODY_BOTTOM);
				float targetRad  = targetCol.GetColScale(COL_NUM::BODY_TOP).x;

				if (IsHitCapsuleToCapsule(checkTop, checkBot, checkRad,
										  targetTop, targetBot, targetRad))
				{
					int offsetX = (checkChara.GetPos().x - targetChara.GetPos().x);
					offsetX *= ((offsetX < 0.0f) ? -1 : 1);
					
					int offsetZ = (checkChara.GetPos().z - targetChara.GetPos().z);
					offsetZ *= ((offsetZ < 0.0f) ? -1 : 1);


					if (offsetX > 1.0f || offsetZ > 1.0f)
					{
						// キャラ同士の横反発
						checkChara.RevertPosXZ(targetChara.GetPos(), BOUNCE_PLAYER_XZ);
						targetChara.RevertPosXZ(checkChara.GetPos(), BOUNCE_PLAYER_XZ);

						if (checkBot.y > (targetTop.y + targetRad))
						{
							// キャラが他キャラに乗るとき、縦に反発
							checkChara.RevertPosY((targetTop.y + targetRad) + BOUNCE_PLAYER_Y, true);
						}
					}
					else
					{
						checkChara.SetIsGround(true);
					}
				}
			}

			// 各行動の有効処理(判定対象の正面に検索対象がいる時)
			if (IsHitSphareToSphere(checkForward, targetBody,
									checkScale.x, targetScale.x))
			{
				if (!targetChara.GetIsHit() ||
					targetChara.GetActionState() == ACTION_STATE::THROWN)
				{
					targetNum++;
					continue;
				}

				// "つかみ"攻撃の有効処理
				if (checkChara.GetActionState() == ACTION_STATE::CATCH)
				{
					// 対象を"つかみ成功状態"化
					checkChara.SetActionState(ACTION_STATE::CATCH_HOLD);

					// 相手を"捕まれ状態"化
					targetChara.SetActionState(ACTION_STATE::CAUGHT);

					// つかむSE再生
					sound.Play(SoundManager::SRC::SE_CATCH,
							   Sound::TIMES::ONCE, true);
				}


				//　"殴る"攻撃
				else if (checkChara.GetActionState() == ACTION_STATE::ATTACK)
				{
					// 吹っ飛ばし量
					weightKnock.x = (CharaBase::WeightCalc(targetChara.GetWeight(), CharaBase::WEIGHT_POW_PUNCH_XZ));
					weightKnock.y = (CharaBase::WeightCalc(targetChara.GetWeight(), CharaBase::WEIGHT_POW_PUNCH_Y));

					// 攻撃終了
					checkChara.SetActionState(ACTION_STATE::ATTACK_END);
					
					// 吹っ飛ばし処理
					targetChara.KnockBack(checkChara.GetPos(),
										  CharaBase::TIME_INV_PUNCH,
										  CharaBase::KNOCK_PUNCH_Y + weightKnock.y,
										  CharaBase::KNOCK_PUNCH_XZ + weightKnock.x,
										  CharaBase::KNOCK_PUNCH_XZ_MIN + weightKnock.x);

					// 対象被ダメ
					targetChara.SetDamage();

					// 攻撃SE再生
					sound.Play(SoundManager::SRC::SE_ATTACK,
							   Sound::TIMES::ONCE, true);

					// ヒットストップ
					scene.SetPerform(SceneManager::PERFORM_TYPE::HIT_STOP, CharaBase::HIT_STOP_PUNCH);

					//エフェクト再生
					effect.ChangeEffect(EffectManager::EFFECT_TYPE::COTTON_DAMAGE, targetChara.GetPos());
					effect.Update();
				}
		
			}

			targetNum++;
		}

		checkNum++;
		targetNum = 0;
	}*/
}

void CollisionManager::CollisionEnemys(void)
{
	auto& enemys = EnemyController::GetInstance().GetEnemys();
	VECTOR pForward = player_->GetFramePos(Object::COLLISION_TYPE::HAND_L);
	float pRad = player_->GetRadius();

	// 攻撃していないとき、終了
	if (!player_->GetIsAttack()) return;

	for (auto& enemy : enemys)
	{
		if (!enemy->GetIsActive())continue;

		enemy->UpdateModelFrames();
		VECTOR ePos = enemy->GetFramePos(Object::COLLISION_TYPE::BODY);
		float eRad = enemy->GetRadius();

		if (UtilityCollision::IsHitSphereToSphere(pForward, pRad, ePos, eRad))
		{
			bool temp = false;
		}
	}
	
}
void CollisionManager::EnemyDamageProc(Enemy& _enemy, int _damage)
{
	// 被ダメージ処理
	_enemy.SetDamage();

	// 

	// 
}

void CollisionManager::CollisionsGround(void)
{
	/*　地面の当たり判定　*/
	/*
	// 当たり判定
	CollisionBase* col = nullptr;

	// オフセット
	float offset = 0.0f;

	// 判定位置
	VECTOR topPos, bottomPos = AsoUtility::VECTOR_ZERO;

	// 当たり判定用ポリゴン
	MV1_COLL_RESULT_POLY result;

	// 衝突フラグ
	bool checkGround; 

	for (auto& charaCol : colChara_)
	{
		CharaBase& chara   = *charaCol.second;
		CollisionBase& col = chara.GetCollision();

		// 当たり判定位置
		topPos = col.GetColPos(COL_NUM::BODY_TOP);
		bottomPos = chara.GetPos();

		offset = (col.GetColOffset(COL_NUM::BODY_BOTTOM) / 2.0f);

		// 地面衝突フラグ
		checkGround = chara.GetIsGround();

		// 投げ状態時、当たり判定を無効化
		if (!chara.GetIsHit()) continue;


		// 壁の当たり判定
		if (CheckGroundCollision(topPos, bottomPos, &result, offset))
		{
			// 衝突距離の差分
			VECTOR hitLength = VSub(bottomPos, result.HitPosition);

			// 衝突距離の長さ
			float distance = VSize(hitLength);


			// 衝突距離がしきい値より小さい
			if (distance <= THEESHOLD_GROUND)
			{
				// 衝突した位置
				VECTOR hitPos = result.HitPosition;

				// 現在地点を衝突した位置にする
				chara.RevertPosY(hitPos.y, !chara.GetIsGround());

				if (!checkGround)
				{
					VECTOR charaVelo = chara.GetVelocity();

					if (charaVelo.y < 0.0f)
					{
						charaVelo.y = 0.0f;
					}

					chara.SetVelocity(charaVelo);

					// 着地処理
					chara.SetIsGround(true);
				}
			}
		}
		else if (checkGround)
		{
			// 地面から離れる
			chara.SetIsGround(false);
		}
	}*/
}

bool CollisionManager::CheckGroundCollision(VECTOR& top, VECTOR& bottom,
	MV1_COLL_RESULT_POLY_DIM* result, float radius)
{
	/*　地面の高さ取得処理　*/

	bool ret = false;

	MV1_COLL_RESULT_POLY_DIM res;

	// カプセル当たり判定処理
	res = MV1CollCheck_Capsule(stageColHandle_, -1,
		top, bottom,
		radius);

	// どれか１つに衝突時true
	if (res.HitNum >= 1)
	{
		*result = res;

		// いずれかのポリゴンに衝突したらtrue
		ret = true;
	}

	// ポリゴンの後始末処理
	MV1CollResultPolyDimTerminate(res);


	return ret;
}
bool CollisionManager::CheckGroundCollision(const VECTOR& top, const VECTOR& bottom,
											MV1_COLL_RESULT_POLY* result, float offset)
{
	/* 地面の高さ取得処理 */

	bool ret = false;

	// 当たり判定位置
	VECTOR startPos	 = top;
	VECTOR endPos = bottom;

	// 当たり判定位置の調整値
	startPos.y	+= offset;

	// 地面の下まで判定を延長
	endPos.y	-= THEESHOLD_GROUND;


	MV1_COLL_RESULT_POLY res;

	res = MV1CollCheck_Line(stageColHandle_, -1, startPos, endPos);

	if (res.HitFlag)
	{
		*result = res;

		ret = true;
	}


	return ret;
}


void CollisionManager::CollisionsWall(void)
{
	/*　壁の当たり判定処理　*/
	/*
	// オフセット
	float offset;

	// 当たり判定位置
	VECTOR topPos, bottomPos = AsoUtility::VECTOR_ZERO;

	
	for (auto& colChara : colChara_)
	{
		//　壁の当たり判定
		
		CharaBase& chara = *colChara.second;
		CollisionBase& col = chara.GetCollision();

		// 当たり判定の位置
		topPos    = col.GetColPos(COL_NUM::BODY_TOP);
		bottomPos = col.GetColPos(COL_NUM::BODY_BOTTOM);
		offset = col.GetColOffset(COL_NUM::BODY_BOTTOM);

		// 当たり判定用ポリゴン
		MV1_COLL_RESULT_POLY_DIM result;


		// プレイヤーと壁の当たり判定
		if (CheckWallCollision(COL_TYPE::CAPCEL, topPos, bottomPos, &result, offset))
		{
			// 衝突した位置の法線ベクトル
			VECTOR normVec = AsoUtility::VECTOR_ZERO;

			// 衝突したポリゴンの数
			int num = result.HitNum;

			for (int i = 0; i < num; i++)
			{
				auto d = result.Dim[i];

				if (d.Normal.y < THEESHOLD_WALL)
				{
					// 衝突した法線ベクトルを取得
					normVec = d.Normal;

					// 現在地点を前フレームに戻す
					chara.RevertPosXZ(normVec, BOUNCE_WALL);
				}
			}
		}
	}*/
}

bool CollisionManager::CheckWallCollision(const VECTOR& startPos, const VECTOR& endPos,
									  MV1_COLL_RESULT_POLY* result, float offset)
{
	bool ret = false;

	/* レイと壁の当たり判定 */
	// Y軸を調整
	VECTOR end = endPos;
	end.y += offset;

	MV1_COLL_RESULT_POLY res = MV1CollCheck_Line(stageColHandle_, -1,
												 startPos, end);

	if (res.HitFlag)
	{
		*result = res;

		// 壁に衝突したら true
		ret = true;
	}

	return ret;
}
bool CollisionManager::CheckWallCollision(COL_TYPE type, const VECTOR& topPos, const VECTOR& bottomPos,
									  MV1_COLL_RESULT_POLY_DIM* result, float radius)
{
	bool ret = false;

	/*　カプセルと壁の当たり判定　*/
	MV1_COLL_RESULT_POLY_DIM res;

	VECTOR top = topPos;
	VECTOR bottom = bottomPos;

	bottom.y += radius;

	res = MV1CollCheck_Capsule(stageColHandle_, -1,
							   top, bottom,
							   radius);

	if (res.HitNum >= 1)
	{
		*result = res;

		// いずれかのポリゴンに衝突したらtrue
		ret = true;
	}

	// ポリゴンの後始末処理
	MV1CollResultPolyDimTerminate(res);


	return ret;
}


void CollisionManager::CollisionsStageDamage(void)
{
	/*　ダメージ領域の当たり判定処理　*/
	/*
	EffectManager& effect = EffectManager::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();

	// ダメージ領域が未割当時、処理を終了
	if (stageDamageHandle_ == -1) return;

	// オフセット
	float offset;

	// 当たり判定位置
	VECTOR topPos, bottomPos = AsoUtility::VECTOR_ZERO;

	// 当たり判定用ポリゴン
	MV1_COLL_RESULT_POLY_DIM result;


	for (auto& colChara : colChara_)
	{
		//　ダメージ領域の当たり判定

		CharaBase& chara   = *colChara.second;
		CollisionBase& col = chara.GetCollision();

		// 当たり判定の位置
		topPos    = col.GetColPos(COL_NUM::BODY_TOP);
		bottomPos = col.GetColPos(COL_NUM::BODY_BOTTOM);
		offset    = col.GetColOffset(COL_NUM::BODY_BOTTOM);

		// 当たり判定が無いとき・ゲームオーバー時、スキップ
		if (!chara.GetIsHit() || chara.GetActionState() == ACTION_STATE::GAME_OVER) continue;


		// プレイヤーとダメージ領域の当たり判定
		if (CheckStageDamageCollision(&chara, topPos, bottomPos, &result, offset))
		{
			// 衝突した位置の法線ベクトル
			VECTOR normVec = AsoUtility::VECTOR_ZERO;

			// 衝突したポリゴンの数
			int num = result.HitNum;

			for (int i = 0; i < num; i++)
			{
				auto d = result.Dim[i];

				// 衝突した法線ベクトルを取得
				normVec = d.Normal;

				// 思いっきり上に吹っ飛ばす
				chara.KnockBack(normVec, CharaBase::TIME_INV_GAMEOVER,
								CharaBase::KNOCK_GAMEOVER_Y, CharaBase::KNOCK_GAMEOVER_XZ);

				// ゲームオーバー化
				chara.SetActionState(ACTION_STATE::GAME_OVER);

				// 撃破SE再生
				sound.Play(SoundManager::SRC::SE_KNOCK, Sound::TIMES::ONCE);

				//エフェクト再生
				effect.ChangeEffect(EffectManager::EFFECT_TYPE::COTTON_KNOCK, chara.GetPrePos());
				effect.Update();
				
			}
		}
	}*/
}

/*
bool CollisionManager::CheckStageDamageCollision(CharaBase* chara, const VECTOR& topPos, const VECTOR& bottomPos,
												 MV1_COLL_RESULT_POLY_DIM* result, float offset)
{
	bool ret = false;

	// モデルの当たり判定情報を取得
	MV1_COLL_RESULT_POLY_DIM res;
	res = MV1CollCheck_Capsule(stageDamageHandle_, -1,
									 topPos, bottomPos, offset);

	// 衝突したポリゴンがあるかチェック
	if (res.HitNum > 0)
	{
		// 衝突☆
		ret = true;

		// 衝突位置を割り当て
		*result = res;
	}
	
	// あと始末☆
	MV1CollResultPolyDimTerminate(res);

	return ret;
}*/



bool CollisionManager::IsHitLine(const VECTOR& pos1, const VECTOR& pos2, MV1_COLL_RESULT_POLY* result)
{

	bool ret = false;/*
	CollisionBase& col = colChara_[COL_TARGET::PLAYER_1];

	int head = static_cast<int>(PlayerCollision::COL_NUM::BODY_TOP);
	int leg = static_cast<int>(PlayerCollision::COL_NUM::BODY_BOTTOM);

	VECTOR top = col->GetColPos(head);
	VECTOR bottom = col->GetColPos(leg);

	top.y += (col->GetColOffset(head));
	bottom.y -= col->GetColOffset(leg);

	MV1_COLL_RESULT_POLY res =
		MV1CollCheck_Line(stageColHandle_, -1, top, bottom);

	// 衝突判定
	if (res.HitFlag)
	{
		*result = res;
		return true;
	}
	*/
	return ret;
}