/* -----------------
 ゲーム制作：基本ソース
 　 氏名 　：2416026　中川原 諒
-------------------- */

#include "Application.h"
#include <DxLib.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	// アプリケーションマネージャ 生成
	Application::CreateInstance();

	// マネージャ取得
	Application& app = Application::GetInstance();

	app.Run();	   // 実行処理

	app.Destroy(); // アプリケーションマネージャ削除処理	
	
	return 0;    //ゲーム終了
}