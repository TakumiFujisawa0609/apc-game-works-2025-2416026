#pragma once
#include <string>

class UtilityCommon
{
public:

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