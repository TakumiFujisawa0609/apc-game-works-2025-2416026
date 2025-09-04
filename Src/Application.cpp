/* ---------------------
　アプリケーションクラス
 ----------------------- */

#include "Common/FrameRate.h"
//#include "Common/Font.h"
#include "Common/SaveFile.h"

#include "Manager/SceneManager.h"
#include "Manager/ResourceManager.h"

#include "Application.h"

#include <DxLib.h>


Application* Application::manager_ = nullptr; // シングルトンインスタンス


/// <summary>
/// インスタンス生成処理
/// </summary>
void Application::CreateManager(void)
{
	// インスタンス未生成時 生成
	if (manager_ == nullptr) manager_ = new Application();

	manager_->Init(); // 初期化処理
}

/// <summary>
/// デフォルトコンストラクタ(private)
/// </summary>
Application::Application(void)
{
	
}

/// <summary>
/// インスタンス取得処理
/// </summary>
/// <returns>アプリケーションマネージャ</returns>
Application& Application::GetInstance(void)
{
	return *manager_;
}


/// <summary>
/// 初期化処理
/// </summary>
void Application::Init(void)
{
	/*  システム処理  */

	SetWindowText("BaseProject～"); //タイトル
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32); //画面サイズ設定
	ChangeWindowMode(true);							//(false：フルスクリーン)

	// DX_LIB初期化処理
	if (DxLib_Init() == -1) OutputDebugString("DxLibが初期化されていません");


	SetWaitVSyncFlag(false); //垂直同期を切る

	FrameRate::CreateManager();		  // フレームレートマネージャ生成
	SceneManager::CreateInstance();	  // シーンマネージャー生成
	ResourceManager::CreateInstance(); // リソースマネージャ生成

	// セーブファイル読み込み処理
//	if (SaveFile::GetInstance()->LoadCSV() == false) _isError = true;
}

/// <summary>
/// 実行処理
/// </summary>
void Application::Run(void)
{
	FrameRate& frameRate	   = FrameRate::GetInstance();	  // fps管理マネージャ
	SceneManager& sceneManager = SceneManager::GetInstance(); // シーン管理マネージャ


	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		frameRate.Update(); // フレームレート更新処理

		//フレームレート制限
		if (frameRate.GetLimitFrameRate() == false)
		{
			frameRate.SetFrameRate();

			sceneManager.Update();
			/*
			SaveFile* save = SaveFile::GetInstance();

			float score = save->GetScore(0, SaveFile::SAVE_TYPE::SCORE_NUM);

			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				score += 1.0f;
				OutputDebugString("スコア増加");

				save->SetScore(0, SaveFile::SAVE_TYPE::SCORE_NUM, score);
				OutputDebugString("スコア割り当て");
			}
			if (CheckHitKey(KEY_INPUT_RETURN))
			{
				save->SaveCSV(); // セーブ

				if (save->LoadCSV() == false)
				{
					_isError = true;
				}
				OutputDebugString("\nスコア保存・読み込み");

			}

			save->SetScore(0, SaveFile::SAVE_TYPE::SCORE_NUM, score);
			*/
			Draw(); // 描画処理
		}
	}
}

void Application::InitEffecseer(void)
{
}

/// <summary>
/// 描画処理
/// </summary>
void Application::Draw(void)
{
	ClearDrawScreen(); // 描画した画像を解放
#ifdef _DEBUG
	FrameRate::GetInstance().Draw(); // フレームレート描画
#endif

	//SaveFile::GetInstance()->Draw();

	ScreenFlip();	   //裏画面を表にコピー
}

/// <summary>
/// インスタンス削除処理
/// </summary>
/// <param name=""></param>
void Application::Destroy(void)
{
	ResourceManager::GetInstance().Release(); // リソースマネージャ削除
	SceneManager::GetInstance().Release();	  // シーンマネージャ削除処理
	FrameRate::GetInstance().Release();		  // フレームレートマネージャ削除処理

	//SaveFile::GetInstance()->Release(); //セーブファイル解放

	// フォントファイル解放
	//Font::GetInstance()->Release();

	DxLib_End(); //DXライブラリ終了

	delete manager_; // アプリケーションマネージャ削除
}