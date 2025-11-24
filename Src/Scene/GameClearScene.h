#pragma once
#include "./SceneBase.h"
#include <DxLib.h>

class GameClearScene : public SceneBase
{
public:

	enum class STATE_CLEAR
	{
		TITLE,
		RESTART,
		GAME_END,
		MAX
	};


	/// @brief デフォルトコンストラクタ 
	GameClearScene(void);

	/// @brief デフォルトデストラクタ
	~GameClearScene(void) = default;

	/// @brief 初期読み込み
	void Load(void)override;

	/// @brief 初期化処理
	void Init(void)override;

	/// @brief 更新処理
	void Update(void)override;

	/// @brief 描画処理
	void Draw(void)override;

	/// @brief メモリ解放処理
	void Release(void)override;


private:

	STATE_CLEAR state_;

	int clearHandle_;
};