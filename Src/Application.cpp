#include "Application.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <cassert>
#include <memory>

#include "./Common/GameExit.h"
#include "./Common/FrameRate.h"
#include "./Common/Font.h"
#include "./Object/Status/StatusData.h"
#include "./Manager/InputManager.h"
#include "./Manager/SceneManager.h"
#include "./Manager/ResourceManager.h"
#include "./Manager/SoundManager.h"


// シングルトンインスタンス
Application* Application::instance_ = nullptr;


void Application::CreateManager(void)
{
	/* インスタンス生成処理 */

	// インスタンス未生成時 生成
	if (instance_ == nullptr) instance_ = new Application();

	instance_->Init(); // 初期化処理
}

Application::Application(void)
{
	// デフォルトコンストラクタ(private)
}

void Application::Init(void)
{
	/* 初期化処理 */

	// システム処理

	// ゲーム名
	SetWindowText(GAME_NAME);

	//画面サイズ設定
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);

	// ウィンドウの状態の設定
	ChangeWindowMode(true);

	// DX_LIB_11の使用
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//垂直同期を切る
	SetWaitVSyncFlag(false);

	// DX_LIB初期化処理
	if (DxLib_Init() == -1) assert("\nDxLibが初期化されていません");


	// エフェクト初期化処理
	InitEffecseer();

	// 入力制御初期化
	SetUseDirectInputFlag(true);

	// マネージャー生成
	CreateManagers();

	// 終了メニュー
	exit_ = std::make_unique<GameExit>();
	exit_->Init();

	// ゲーム可能処理
	isGame_ = true;


	Font& font = Font::GetInstance();

	// フォント割り当て
	font.AddFont(Font::FONT_GAME, Font::GAME_FONT_NAME, Font::FONT_GAME_PATH,
				 Font::GAME_FONT_SIZE, Font::GAME_FONT_WIDTH, Font::FONT_TYPE_EDGE);

	// デフォルトフォント割り当て
	font.SetDefaultFont(Font::FONT_GAME);
}
void Application::CreateManagers(void)
{
	// 入力マネージャ
	InputManager::CreateInstance();

	// ステータスデータ
	StatusData::CreateInstance();

	// リソースマネージャ生成
	ResourceManager::CreateInstance();

	// フレームレートマネージャ生成
	FrameRate::CreateInstance();

	// 音声マネージャー生成
	SoundManager::CreateInstance();

	// シーンマネージャー生成
	SceneManager::CreateInstance();

	//フォントマネージャー生成
	Font::CreateInstance();
}


void Application::Run(void)
{
	/*　実行処理　*/
	
	// fps管理マネージャ
	FrameRate& fps = FrameRate::GetInstance();

	// シーン管理マネージャ
	SceneManager& scene = SceneManager::GetInstance();

	// 入力マネージャ
	InputManager& input = InputManager::GetInstance();


	while (ProcessMessage() == 0 && isGame_)
	{
		// フレームレート更新処理
		fps.Update();

		//フレームレート制限
		if (fps.GetLimitFrameRate() == false)
		{
			// フレームレート割り当て
			fps.SetFrameRate();

			// 入力マネージャ更新
			input.Update();

			if (!exit_->GetIsActiveMenu())
			{
				// シーン更新
				scene.Update();
			}

			if (isActiveExitMenu_)
			{
				exit_->Update();
			}
			
			// 描画処理
			Draw();
		}
	}
}

void Application::Draw(void)
{
	/* 描画処理 */
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen(); // 描画した画像を解放

	// シーン描画
	SceneManager::GetInstance().Draw();

	// 終了メニュー
	exit_->Draw();

#ifdef _DEBUG
	FrameRate::GetInstance().Draw(); // フレームレート描画
#endif

	ScreenFlip(); // 裏画面を表にコピー
}

void Application::Destroy(void)
{
	/*　インスタンス削除処理　*/

	// フォントファイルの解放
	Font::GetInstance().Destroy();

	// シーンマネージャ
	SceneManager::GetInstance().Destroy();

	// フレームレートマネージャ
	FrameRate::GetInstance().Destroy();

	// 入力マネージャ
	InputManager::GetInstance().Destroy();

	// 音声マネージャ
	SoundManager::GetInstance().Destroy();

	// リソースマネージャー
	ResourceManager::GetInstance().Destroy();

	// ステータスデータ
	StatusData::GetInstance()->Destroy();


	Effkseer_End();

	DxLib_End();

	// アプリケーション削除
	delete instance_;
}

void Application::InitEffecseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}