/* -----------
　HPバーデータ
 ------------- */
#include "../Utility/Vector2.h"
#include <string>

class HPBer
{
public:

	/* --- <<< 関数プロトタイプ宣言 >>> --- */

	// デフォルトコンストラクタ
	HPBer(void);

	// デストラクタ
	~HPBer(void);

	// HP情報 取得
	void InitHP(int maxHP, Vector2 pos, int sizeX, int sizeY, int color, int backColor);

	// HPバー情報取得
	void InitHPBer(int posX, int posY, int sizeX, int sizeY, int color, char* text, int textPosX, int textPosY, int textColor);

	// HP描画処理(毎フレーム処理)
	void DrawHP(void);

	/*　HPバー描画処理　*/
	void DrawHPBer(void);


	/* --- <<< セッター処理 >>> --- */

	// 位置 割り当て
	void SetPos(Vector2 pos);

	// HP情報 割り当て
	void SetHP(int HP);


private:

	/* --- <<< 変数定義 >>> --- */

	/*　HP系　*/
	int sizeX_; // 縦HPバーサイズ
	int sizeY_; // 横HPバーサイズ

	Vector2 pos_;	// 位置
	int color_;		// 色
	int backColor_; // 背景色
	int curHP_;		// 現在HP
	int maxHP_;		// 最大HP


	/*　HPバー系　*/

	int berSizeX_; // 縦HPバーサイズ
	int berSizeY_; // 横HPバーサイズ
	int berPosX_;  // 縦位置
	int berPosY_;  // 横位置
	int berColor_; // 色

	/*　テキスト系　*/
	char* text_;	// 文字内容
	int textPosX_;	// 縦位置
	int textPosY_;	// 横位置
	int textColor_;	// 色
};