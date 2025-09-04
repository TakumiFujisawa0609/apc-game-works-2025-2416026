#pragma once

class SceneBase
{
public:

	// デフォルトコンストラクタ
	SceneBase(void);

	// デフォルトデストラクタ
	virtual ~SceneBase(void) = 0;

	// リソース読み込み処理
	virtual void Load(void);

	// 初期化処理
	virtual void Init(void) = 0;
	
	// 更新処理
	virtual void Update(void) = 0;

	// 描画処理
	virtual void Draw(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;


	/// <summary>
	/// ロード出来たか否か
	/// </summary>
	bool IsLoad(void);

};