#include "../Common/Vector2.h"
#include <string>

class HPBer
{
public:

	
	// デフォルトコンストラクタ
	HPBer(void);

	// デストラクタ
	~HPBer(void);

	/// <summary>
	/// HP情報 取得
	/// </summary>
	/// <param name="maxHP">最大HP</param>
	/// <param name="pos"></param>
	/// <param name="sizeX"></param>
	/// <param name="sizeY"></param>
	/// <param name="color"></param>
	/// <param name="backColor"></param>
	void InitHP(int maxHP, const Vector2& pos, const Vector2& size, unsigned int color, unsigned int backColor);

	// HPバー情報取得
	void InitHPBer(Vector2 pos, int sizeX, int sizeY, int color, char* text, int textPosX, int textPosY, int textColor);

	// HP描画処理(毎フレーム処理)
	void DrawHP(void);

	/*　HPバー描画処理　*/
	void DrawHPBer(void);


	// 位置 割り当て
	void SetPos(Vector2 pos);

	// HP情報 割り当て
	void SetHP(int HP);


private:

	struct Ber
	{
		Vector2 pos;
		Vector2 size;
		unsigned int color;
		int curHp;
		int maxHp;
	};
	Ber ber_;

	struct Back
	{
		Vector2 pos;
		Vector2 size;
		unsigned int color;
	};
	Back back_;

	struct Text
	{
		char* text;
		Vector2 pos;
		unsigned int color;
	};
	Text text_;
};