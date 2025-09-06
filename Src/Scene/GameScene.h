#pragma once
#include "SceneBase.h"

class GameScene : public SceneBase
{
public:

	// デフォルトコンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) = default;

	// 初回ロード
	void Load(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;


private:

};