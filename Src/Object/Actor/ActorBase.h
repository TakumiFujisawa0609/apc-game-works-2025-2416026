#pragma once
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/Transform.h"

class ActorBase
{
public:

	ActorBase(void);

	virtual ~ActorBase(void) = default;


	virtual void Load(void);

	virtual void Init(const VECTOR& _pos, float _angleY);

	virtual void Update(void);

	virtual void Draw(void);

	virtual void Release(void);

	const Transform& GetTransform(void) { return *transform_; };


	/// @brief 位置割り当て
	void SetPos(const VECTOR& pos) { transform_->pos = pos; };

	/// @brief 現在位置取得
	VECTOR& GetPos(void) { return transform_->pos; };

	/// @brief ローカル座標取得
	const VECTOR& GetPosLocal(void) const { return transform_->localPos; };

	/// @brief オイラー角の向き取得
	const VECTOR& GetRotationEuler(void) const { return transform_->rot; };

	/// @brief クォータニオン角の向き取得
	Quaternion& GetRotation(void) { return transform_->quaRot; };

	/// @brief ローカル座標のクォータニオン角の向き取得
	const Quaternion& GetRotationLocal(void) const { return transform_->quaRotLocal; };

	/// @brief スケール取得
	const VECTOR& GetScale(void) const { return transform_->scl; };


protected:

	// シーンマネージャ
	SceneManager& sceneMng_;

	// リソースマネージャ
	ResourceManager& resMng_;


	Transform* transform_;


	/// @brief 各オブジェクトの読み込み処理
	virtual void LoadPost(void) = 0;

	/// @brief その他処理初期化 
	virtual void InitPost(void) = 0;
	
	// @brief サブクラス独自更新処理
	virtual void UpdatePost(void) = 0;

	/// @brief 各オブジェクト独自の描画
	virtual void DrawPost(void) {};

	virtual void ReleasePost(void) {};
};