#pragma once
#include <string>

class UtilityCommon
{
public:

	/// <summary>
	/// 文字列→int型変換
	/// </summary>
	/// <param name="text">対象のテキスト</param>
	/// <param name="target">数値割り当て先</param>
	/// <param name="minNum">最小値</param>
	static void ChangeString(const std::string& text, int& target, int minNum);

	/// <summary>
	/// 文字列→int型変換
	/// </summary>
	/// <param name="text">対象のテキスト</param>
	/// <param name="target">数値割り当て先</param>
	static void ChangeString(const std::string& text, int& target);

	/// <summary>
	/// 文字列→float型変換
	/// </summary>
	/// <param name="text">対象のテキスト</param>
	/// <param name="target">数値割り当て先</param>
	/// <param name="minNum">最小値</param>
	static void ChangeString(const std::string& text, float& target, float minNum);

	/// <summary>
	/// 文字列→float型変換
	/// </summary>
	/// <param name="text">対象のテキスト</param>
	/// <param name="target">数値割り当て先</param>
	static void ChangeString(const std::string& text, float& target);
};