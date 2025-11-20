#pragma once
#include <string>
#include <DxLib.h>

class UtilityCommon
{
public:

	struct Color
	{
		int r = 0, g = 0, b = 0, a = 1;
	};

	/// @brief 色を16進数で生成
	static unsigned int SetColor(Color _color);

	/// @brief 色を16進数で生成
	static unsigned int SetColor(COLOR_F _color);

	/// @brief 色を16進数で生成
	static unsigned int SetColor(float _r, float _g, float _b);


	/// @brief 文字列→int型変換																							
	/// @param _text 対象のテキスト
	/// @param _target 数値割り当て先
	/// @param _minNum 最小値
	static void ChangeString(const std::string& _text, int& _target, int _minNum);

	/// @brief 文字列→int型変換
	/// @param _text 対象のテキスト
	/// @param _target 数値割り当て先
	static void ChangeString(const std::string& _text, int& _target);

	/// @brief 文字列→float型変換
	/// @param _text 対象のテキスト
	/// @param _target 数値割り当て先
	/// @param _minNum 最小値
	static void ChangeString(const std::string& _text, float& _target, float _minNum);

	/// @brief 文字列→float型変換
	/// @param _text 対象のテキスト
	/// @param _target 数値割り当て先
	static void ChangeString(const std::string& _text, float& _target);
};