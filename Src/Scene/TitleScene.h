#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase
{
public:

	TitleScene(void);

	~TitleScene(void) = default;


	// リソース読み込み
	void Load(void) override;

	// 初期化処理
	void Init(void) override;

	// 更新処理
	void Update(void) override;

	// 描画処理
	void Draw(void) override;

	// 解放処理
	void Release(void) override;


private:
};