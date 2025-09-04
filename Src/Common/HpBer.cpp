/* ----------
　HPバー処理
 ------------- */

#include "HpBer.h"
#include <string>
#include <DxLib.h>

/*　デフォルトコンストラクタ　*/
HPBer::HPBer(void)
{
	/*　HP初期化　*/
	sizeX_ = 0;
	sizeY_ = 0;
	color_ = -1;
	backColor_ = -1;
	maxHP_ = -1;
	curHP_ = -1;

	/*　HPバー初期化　*/
	berPosX_  = 0;
	berPosY_  = 0;
	berSizeX_ = 0;
	berSizeY_ = 0;
	berColor_ = -1;

	/*　テキスト初期化　*/
	text_	   = (char*)"";
	textPosX_  = 0;
	textPosY_  = 0;
	textColor_ = -1;
}

/*　デストラクタ　*/
HPBer::~HPBer(void)
{

}


/* --- <<< 処理系 >>> --- */

/*　HP情報初期化処理　*/
void HPBer::InitHP(int maxHP, Vector2 pos, int sizeX, int sizeY, int color, int backColor)
{
	/*　HP初期化　*/
	maxHP_ = maxHP;  // 最大HP
	curHP_ = maxHP_; // 現在HP

	pos_ = pos; // 位置初期化

	/*　サイズ初期化　*/
	sizeX_ = sizeX; // 横サイズ
	sizeY_ = sizeY; // 縦サイズ

	/*　色初期化　*/
	color_	   = color;		// 現在HP色
	backColor_ = backColor; // 背景HP色
}

/*　HPバー情報初期化処理　*/
void HPBer::InitHPBer(int posX, int posY, int sizeX, int sizeY, int color, char* text, int textPosX, int textPosY, int textColor)
{
	/*　位置初期化　*/
	berPosX_ = posX; // 横位置
	berPosY_ = posY; // 縦位置

	/*　サイズ初期化　*/
	berSizeX_ = sizeX; // 横サイズ
	berSizeY_ = sizeY; // 縦サイズ

	berColor_ = color; // 色初期化

	/*　テキスト 初期化　*/
	text_	   = text;		// テキスト内容
	textPosX_  = textPosX;	// テキスト横位置
	textPosY_  = textPosY;	// テキスト縦位置
	textColor_ = textColor; // テキスト色
}

/*　現在HP描画処理　*/
void HPBer::DrawHP(void)
{
	float rateHP = 0; // HP割合
	if (curHP_ != 0)
	{
		// 現在のHP割合
		rateHP = static_cast<float>(curHP_) / static_cast<float>(maxHP_);
	}

	// 現在HP 横サイズ
	float curHPSize = static_cast<float>(sizeX_) * rateHP;

	// 最大HP 描画
	DrawBox(pos_.x , pos_.y, (pos_.x + sizeX_), (pos_.y + sizeY_), backColor_, true);

	// 現在HP 描画
	DrawBox(pos_.x, pos_.y, (pos_.x + static_cast<int>(curHPSize)), (pos_.y + sizeY_), color_, true);
}

/*　HPバー(現在HP含め)描画処理　*/
void HPBer::DrawHPBer(void)
{
	// HPバー 描画
	DrawBox(berPosX_, berPosY_,
			berPosX_ + berSizeX_, berPosY_ + berSizeY_,
			berColor_, true);

	// 名前描画 
	DrawString(textPosX_, (berPosY_ + textPosY_), text_, textColor_);

	DrawHP(); // 現在HP描画
}


/* --- <<< セッター処理 >>> --- */

/*　位置情報 割り当て　*/
void HPBer::SetPos(Vector2 pos)
{
	if (pos_.x != pos.x || pos_.y != pos.y)
	{
		/*　位置情報更新時 割り当て　*/
		pos_ = pos;
	}
}

/*　HP情報 割り当て　*/
void HPBer::SetHP(int HP)
{
	// 現在HPと描画HPが不一致時に変更
	if (curHP_ != HP)
	{
		curHP_ = HP;
	}
}