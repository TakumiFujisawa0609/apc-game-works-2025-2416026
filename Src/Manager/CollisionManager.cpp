#include "CollisionManager.h"
#include <DxLib.h>
#include <map>

#include "../Common/Vector2.h"
#include "../Utility/AsoUtility.h"
#include "../Utility/UtilityCollision.h"
#include "../Object/Actor/Object.h"
#include "../Object/Player/Player.h"
#include "../Object/Enemy/EnemyController.h"
#include "../Object/Enemy/Enemy.h"
#include "../Object/Enemy/EnemyBoss.h"
#include "../Object/Stage/Stage.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/EffectController.h"

CollisionManager::CollisionManager(Player& _player, EnemyController& _enemys, Stage& _stage):
	player_(&_player), enemys_(&_enemys), stage_(_stage),
	stageColHandle_(-1)
{
	// 当たり判定用ハンドル読み込み
	stageColHandle_ = stage_.GetTransform().modelId;

	// ステージ当たり判定割り当て
	MV1SetupCollInfo(stageColHandle_, -1);

	//ステージサイズ設定
	MV1SetScale(stageColHandle_, stage_.GetTransform().scl);

	//ステージ座標設定
	MV1SetPosition(stageColHandle_, stage_.GetTransform().pos);
}


void CollisionManager::Init(int& stageHandle, const VECTOR& stagePos, const VECTOR& stageScale,
							int& damageHandle, const VECTOR& damagePos, const VECTOR& damageScale)
{
	//colChara_.clear();


	// 当たり判定用ハンドル読み込み
	stageColHandle_ = stage_.GetTransform().modelId;

	// ステージ当たり判定割り当て
	MV1SetupCollInfo(stageColHandle_, -1);

	//ステージサイズ設定
	MV1SetScale(stageColHandle_, stageScale);

	//ステージ座標設定
	MV1SetPosition(stageColHandle_, stagePos);
}
/*
void CollisionManager::SetCharaCollision(CollisionManager::COL_TARGET target, CharaBase& chara)
{
	int num = static_cast<int>(target);

	// 当たり判定をリストに格納
	colChara_.emplace(num, &chara);
}*/


void CollisionManager::Update(void)
{
	// プレイヤーと敵の当たり判定
	CollisionPlayerToEnemy();

	// プレイヤーとボスの当たり判定
	CollisionPlayerToBoss();

	//CollisionGround();

	// キャラクター同士の当たり判定
	//CollisionChara();

	// 地面当たり判定
	//CollisionsGround();

	// 壁当たり判定
	CollisionsWall();
}

void CollisionManager::DrawDebug(void)
{
#ifdef _DEBUG

	if (stageColHandle_ != -1)
	{
		MV1DrawModel(stageColHandle_);
	}

#endif // _DEBUG
}

void CollisionManager::Release(void)
{
}


void CollisionManager::CollisionChara(void)
{
	/*
	SceneManager& scene = SceneManager::GetInstance();
	SoundManager& sound = SoundManager::GetInstance();
	EffectController& effect = EffectController::GetInstance();

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
					effect.ChangeEffect(EffectController::EFFECT_TYPE::COTTON_DAMAGE, targetChara.GetPos());
					effect.Update();
				}
		
			}

			targetNum++;
		}

		checkNum++;
		targetNum = 0;
	}*/
}

void CollisionManager::CollisionPlayerToEnemy(void)
{
	/*　プレイヤーと敵の当たり判定　*/

	VECTOR pBody, eBody, pPos, ePos = AsoUtility::VECTOR_ZERO;
	float pRad, eRad = 0.0f;
	int power = 0;
	pPos = player_->GetPos();

	// 敵未割当時、処理終了
	if (enemys_->GetEnemyLists().empty()) { return; }

	for (auto& enemyList : enemys_->GetEnemyLists())
	{
		for (auto& enemy : enemyList)
		{
			// 無効状態・HP0の時、スキップ
			if (!enemy->GetIsCollisionActive()) { continue; }

			pBody = player_->GetFramePos(COLLISION_TYPE::BODY);
			pRad = player_->GetRadius(COLLISION_TYPE::BODY);
			eBody = enemy->GetFramePos(COLLISION_TYPE::BODY);
			eRad = enemy->GetRadius(COLLISION_TYPE::BODY);
			ePos = enemy->GetPos();

			if (UtilityCollision::IsHitSphereToSphere(pBody, pRad, eBody, eRad))
			{
				// プレイヤーの位置と反発
				//enemy->SetPos(UtilityCollision::CollisionReflectXZ(ePos.y, eBody, eRad, pBody, pRad));
			}

			// プレイヤーの攻撃時の敵の被ダメージ処理
			if (player_->CheckActiveAttack())
			{
				enemy->UpdateModelFrames();

				pBody = player_->GetPosForward();
				pRad = player_->GetRadiusAttack(player_->GetMotionType());
				power = player_->GetPower();
				eBody = enemy->GetFramePos(Object::COLLISION_TYPE::BODY);
				eRad = enemy->GetRadius(COLLISION_TYPE::BODY);

				if (UtilityCollision::IsHitSphereToSphere(pBody, pRad, eBody, eRad))
				{
					enemy->SetDamage(power);

					if (!enemy->GetIsElite())
					{
						const float KNOCK_XZ = 0.05f;
						const float KNOCK_Y = 1.25f;
						enemy->KnockBack(player_->GetDir(), KNOCK_Y, KNOCK_XZ);
					}

					const float SCALE = 35.0f;

					SceneManager::GetInstance().GetEffects().
						SetEffect(EffectController::EFFECT_TYPE::SWORD_HIT,
							eBody, 1.0f, AsoUtility::VECTOR_ZERO, {SCALE, SCALE, SCALE});
				}
			}

			// 敵の攻撃時のプレイヤーの被ダメージ処理
			if (enemy->CheckActiveAttack())
			{
				eBody = enemy->GetPosForward();
				eRad = enemy->GetRadiusAttack();
				power = enemy->GetPower();
				pBody = player_->GetFramePos(Object::COLLISION_TYPE::BODY);
				pRad = player_->GetRadius(COLLISION_TYPE::BODY);
				if (UtilityCollision::IsHitSphereToSphere(eBody, eRad, pBody, pRad))
				{
					player_->SetDamage(power);
				}
			}
		}
	}
}

void CollisionManager::CollisionPlayerToBoss(void)
{
	/*　プレイヤーと敵の当たり判定　*/

	VECTOR pBody, eBody, pPos, ePos = AsoUtility::VECTOR_ZERO;
	float pRad, eRad = 0.0f;
	int power = 0;
	pPos = player_->GetPos();

	for (auto& boss : enemys_->GetEnemyBossList())
	{
		pBody = player_->GetFramePos(COLLISION_TYPE::BODY);
		pRad = player_->GetRadius(COLLISION_TYPE::BODY);

		// 召喚魔法陣当たり判定
		if (boss->GetIsSpawnCircle())
		{
			eRad = boss->GetSpawnCircleRadius();
			ePos = boss->GetSpawnCirclePos();
		}

		// ボス当たり判定
		else
		{
			eBody = boss->GetFramePos(COLLISION_TYPE::BODY);
			ePos = boss->GetPos();
			eRad = boss->GetRadius(COLLISION_TYPE::BODY);
		}

		if (UtilityCollision::IsHitSphereToSphere(pBody, pRad, eBody, eRad))
		{
			// プレイヤーの位置と反発
			//enemy->SetPos(UtilityCollision::CollisionReflectXZ(ePos.y, eBody, eRad, pBody, pRad));
		}

		if (boss->GetIsSpawnCircle())
		{
			if (UtilityCollision::IsHitSphereToSphere(pPos, pRad, ePos, eRad))
			{
				// 魔法陣無効化
				boss->SetIsSpawnCircle(false);
			}
			continue;
		}

		// 敵未割当時、処理終了
		// 無効状態・HP0の時、スキップ
		if (!boss->GetIsCollisionActive()) { continue; }

		// プレイヤーの攻撃時の敵の被ダメージ処理
		if (player_->CheckActiveAttack())
		{
			boss->UpdateModelFrames();

			pBody = player_->GetPosForward();
			pRad = player_->GetRadiusAttack(player_->GetMotionType());
			power = player_->GetPower();
			eBody = boss->GetFramePos(Object::COLLISION_TYPE::BODY);
			eRad = boss->GetRadius(COLLISION_TYPE::BODY);

			if (UtilityCollision::IsHitSphereToSphere(pBody, pRad, eBody, eRad))
			{
				boss->SetDamage(power);

				const float SCALE = 100.0f;
				SceneManager::GetInstance().GetEffects().
					SetEffect(EffectController::EFFECT_TYPE::SWORD_HIT,
							  eBody, 1.0f, AsoUtility::VECTOR_ZERO, { SCALE, SCALE, SCALE });
			}
		}

		// 敵の攻撃時のプレイヤーの被ダメージ処理
		if (boss->CheckActiveAttack())
		{
			eBody = boss->GetPosForward();
			eRad = boss->GetRadiusAttack();
			power = boss->GetPower();
			pBody = player_->GetFramePos(Object::COLLISION_TYPE::BODY);
			pRad = player_->GetRadius(COLLISION_TYPE::BODY);

			if (UtilityCollision::IsHitSphereToSphere(eBody, eRad, pBody, pRad))
			{
				player_->SetDamage(power);
			}
		}
	}
}

void CollisionManager::CollisionEnemyToEnemy(void)
{
	int listSize = static_cast<int>(enemys_->GetEnemyLists().size());

	VECTOR pos1 = AsoUtility::VECTOR_ZERO, pos2 = AsoUtility::VECTOR_ZERO;
	float eRad1 = 0.0f, eRad2 = 0.0f;

	
	for (int list = 0; list < listSize; list++)
	{
		int size = static_cast<int>(enemys_->GetEnemys(list).size());

		for (int y = 0; y < size; y++)
		{
			for (int x = y; x < size; x++)
			{
				Enemy& enemy1 = enemys_->GetEnemy(list, y);
				Enemy& enemy2 = enemys_->GetEnemy(list, x);
				pos1 = enemy1.GetPos();
				pos2 = enemy2.GetPos();
				eRad1 = enemy1.GetRadius(Object::COLLISION_TYPE::BODY);
				eRad2 = enemy2.GetRadius(Object::COLLISION_TYPE::BODY);

				if (UtilityCollision::IsHitSphereToSphere(pos1, eRad1, pos2, eRad2))
				{
					// 前フレーム位置に戻す
					//enemy1->SetPos(UtilityCollision::CollisionReflectXZ(pPos, pRad, ePos, eRad));

				}
			}
		}
	}
}


void CollisionManager::EnemyDamageProc(Enemy& _enemy, int _damage)
{
	
}

void CollisionManager::CollisionGround(void)
{
	/*　地面の当たり判定　*/

	// オフセット
	float offset = 0.0f;

	// 判定位置
	VECTOR topPos, bottomPos = AsoUtility::VECTOR_ZERO;

	// 当たり判定用ポリゴン
	MV1_COLL_RESULT_POLY result;

	// 衝突フラグ
	bool checkGround; 

	// 当たり判定位置
	topPos = player_->GetFramePos(Object::COLLISION_TYPE::HEAD);
	bottomPos = player_->GetPos();

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
			player_->RevertPosY(hitPos.y);

			{
				VECTOR charaVelo = player_->GetVelocity();

				if (charaVelo.y < 0.0f)
				{
					//charaVelo.y = 0.0f;
				}

				player_->SetVelocity(charaVelo);
			}
		}
	}

	/*
	for (auto& enemyList : enemys_->GetEnemyLists())
	{
		for (auto& enemy : enemyList)
		{
			// 当たり判定位置
			topPos = enemy->GetFramePos(Object::COLLISION_TYPE::HEAD);
			bottomPos = enemy->GetPos();

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
					enemy->RevertPosY(hitPos.y);

					{
						VECTOR charaVelo = enemy->GetVelocity();

						if (charaVelo.y < 0.0f)
						{
							//charaVelo.y = 0.0f;
						}

						enemy->SetVelocity(charaVelo);
					}
				}
			}
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
	
	// オフセット
	const float offset = 0.1f;

	// 当たり判定位置
	VECTOR topPos, bottomPos = AsoUtility::VECTOR_ZERO;

	// 当たり判定の位置
	topPos = player_->GetFramePos(Object::COLLISION_TYPE::HEAD);
	bottomPos = player_->GetFramePos(Object::COLLISION_TYPE::BOTTOM);

	// 当たり判定用ポリゴン
	MV1_COLL_RESULT_POLY_DIM result;


	// 敵と壁の当たり判定
	if (CheckWallCollision(COL_TYPE::CAPCEL, topPos, bottomPos, &result, offset))
	{
		// 衝突した位置の法線ベクトル
		VECTOR normVec = AsoUtility::VECTOR_ZERO;

		// 衝突したポリゴンの数
		int num = result.HitNum;

		const int WALL_FRAME = 4;

		for (int i = 0; i < num; i++)
		{
			auto d = result.Dim[i];

			if (d.Normal.y < THEESHOLD_WALL &&
				d.FrameIndex != WALL_FRAME)
			{
				// 衝突した法線ベクトルを取得
				normVec = d.Normal;

				// 現在地点を前フレームに戻す
				player_->RevertPosXZ(normVec, BOUNCE_WALL);
			}
		}
	}

	return;
	for (auto& enemyList : enemys_->GetEnemyLists())
	{
		for (auto& enemy : enemyList)
		{
			// 当たり判定の位置
			topPos = enemy->GetFramePos(Object::COLLISION_TYPE::HEAD);
			bottomPos = enemy->GetFramePos(Object::COLLISION_TYPE::BOTTOM);

			// 敵と壁の当たり判定
			if (CheckWallCollision(COL_TYPE::CAPCEL, topPos, bottomPos, &result, offset))
			{
				// 衝突した位置の法線ベクトル
				VECTOR normVec = AsoUtility::VECTOR_ZERO;

				// 衝突したポリゴンの数
				int num = result.HitNum;

				for (int i = 0; i < num; i++)
				{
					auto dim = result.Dim[i];

					if (dim.Normal.y < THEESHOLD_WALL)
					{
						// 衝突した法線ベクトルを取得
						normVec = dim.Normal;

						// 現在地点を前フレームに戻す
						enemy->RevertPosXZ(normVec, BOUNCE_WALL);
					}
				}
			}
		}
	}
}

bool CollisionManager::CheckWallCollision(const VECTOR& startPos, const VECTOR& endPos,
									  MV1_COLL_RESULT_POLY* result, float offset)
{
	bool ret = false;

	/* レイと壁の当たり判定 */
	// Y軸を調整
	VECTOR end = endPos;
	end.y += offset;

	MV1_COLL_RESULT_POLY res = MV1CollCheck_Line(stageColHandle_, -1,startPos, end);
	
	const int WALL_FRAME = 4;

	if (res.HitFlag &&
		res.FrameIndex != WALL_FRAME)
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