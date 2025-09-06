#include "UtilityCommon.h"
#include <cassert>
#include <algorithm>


void UtilityCommon::ChangeString(const std::string& text, int& target, int minNum)
{
    /* 文字列→int */

    if (std::all_of(text.cbegin(), text.cend(), isdigit))
    {
        // 文字列が全て数字のとき
        target = stoi(text);

        // 最小値より小さいとき最小値にする
        target = ((target < minNum) ? minNum : target);

    }
    else if (text != " " || text != "")
    {
        // 空白以外はエラー
        std::string error = "\n文字列の値に誤りがあります。 ";
        error += text;
        assert(error.c_str());
    }
}

void UtilityCommon::ChangeString(const std::string& text, int& target)
{
    /* 文字列→int */

    if (std::all_of(text.cbegin(), text.cend(), isdigit))
    {
        // 文字列が全て数字のとき
        target = stoi(text);
    }
    else if (text != " " || text != "")
    {
        // 空白以外はエラー
        std::string error = "\n文字列の値に誤りがあります。 ";
        error += text;
        assert(error.c_str());
    }
}

void UtilityCommon::ChangeString(const std::string& text, float& target, float minNum)
{
    /* 文字列→float */

    if (std::all_of(text.cbegin(), text.cend(), isdigit))
    {
        // 文字列が全て数字のとき
        target = stoi(text);

        // 最小値より小さいとき最小値にする
        target = ((target < minNum) ? minNum : target);

    }
    else if (text != "" || text != " ")
    {
        // 空白以外はエラー
        std::string error = "\n文字列の値に誤りがあります。 ";
        error += text;
        assert(error.c_str());
    }
}

void UtilityCommon::ChangeString(const std::string& text, float& target)
{
    /* 文字列→float */

    if (std::all_of(text.cbegin(), text.cend(), isdigit))
    {
        // 文字列が全て数字のとき
        target = stoi(text);

    }
    else if (text != "" || text != " ")
    {
        // 空白以外はエラー
        std::string error = "\n文字列の値に誤りがあります。 ";
        error += text;
        assert(error.c_str());
    }
}
