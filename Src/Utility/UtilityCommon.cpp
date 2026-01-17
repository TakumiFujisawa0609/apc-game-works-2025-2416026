#include "UtilityCommon.h"
#include <DxLib.h>
#include <cassert>
#include <algorithm>
#include "./AsoUtility.h"


void UtilityCommon::ChangeString(const std::string& text, int& target, int minNum)
{
    /* 文字列→int */

    std::string txt = text;
    int num = target;

    // 文字列ミス判定フラグ
    bool isCheck = true;

    // マイナス
    const char& minus = *"-";


    // 文字が空白のとき、最小値の文字列にする
    if (text == "") { txt = std::to_string(minNum); }


    for (char t : txt)
    {
        // 数字・マイナスではない文字列のとき、false
        if (!std::isdigit(static_cast<unsigned char>(t)) &&
            t != minus)
        {
            isCheck = false;
            break;
        }
    }

    if (isCheck)
    {
        // 最小値より小さいとき最小値にする
        num = ((target < minNum) ? minNum : target);

        // 文字列→int変換
        num = stoi(text);

        // 数値に反映
        target = num;
    }
    else
    {
        // エラー
        std::string error = "\n文字列の値に誤りがあります。 ";
        error += text;
        assert(error.c_str());

    }
}

void UtilityCommon::ChangeString(const std::string& text, int& target)
{
    /* 文字列→int */

    std::string txt = text;
    int num = target;

    // 文字列ミス判定フラグ
    bool isCheck = true;

    // マイナス
    const char& minus = *"-";


    // 文字が空白のとき、0にする
    if (text == "") { txt = std::to_string(0); }


    for (char t : txt)
    {
        // 数字・マイナスではない文字列のとき、false
        if (!isdigit(static_cast<unsigned char>(t)) &&
            t != minus)
        {
            isCheck = false;
            break;
        }
    }

    if (isCheck)
    {
        // 文字列→int変換
        num = stoi(text);

        // 数値に反映
        target = num;
    }
    else
    {
        // エラー
        std::string error = "\n文字列の値に誤りがあります。 ";
        error += text;
        assert(error.c_str());

    }
}

void UtilityCommon::ChangeString(const std::string& text, float& target, float minNum)
{
    /* 文字列→float */

    std::string txt = text;
    float num = target;

    // 文字列ミス判定フラグ
    bool isCheck = true;

    // 小数点
    const char& dot = *".";

    // マイナス
    const char& minus = *"-";


    // 文字が空白のとき、最小値の文字列にする
    if (text == "")
    {
        txt = std::to_string(minNum);
        return;
    }


    for (auto t : text)
    {
        // 数字・小数点・マイナスではない文字列のとき、false
        if (!isdigit(static_cast<unsigned char>(t)) &&
            t != dot && t != minus)
        {
            isCheck = false;
            break;
        }
    }

    if (isCheck)
    {
        // 最小値より小さいとき最小値にする
        num = ((target < minNum) ? minNum : target);

        // 文字列→float変換
        num = stof(text);

        // 数値に反映
        target = num;
    }
    else
    {
        // エラー
        std::string error = "\n文字列の値に誤りがあります。 ";
        error += text;
        assert(error.c_str());

    }
}

void UtilityCommon::ChangeString(const std::string& text, float& target)
{
    /* 文字列→float */

    /* 文字列→float */
    std::string txt = text;

    // 文字列ミス判定フラグ
    bool isCheck = true;

    float num = target;

    // 小数点
    const char& dot = *".";

    // マイナス
    const char& minus = *"-";


    // 文字が空白のとき、0にする
    if (text == "") { txt = std::to_string(0); }


    for (auto t : text)
    {
        // 数字・小数点・マイナスではない文字列のとき、false
        if (!isdigit(static_cast<unsigned char>(t)) &&
            t != dot && t != minus)
        {
            isCheck = false;
            break;
        }
    }

    if (isCheck)
    {
        // 文字列→float変換
        num = stof(text);

        // 数値に反映
        target = num;
    }
    else
    {
        // エラー
        std::string error = "\n文字列の値に誤りがあります。 ";
        error += text;
        assert(error.c_str());

    }
}



unsigned int UtilityCommon::SetColor(Color _color)
{
    return SetColor(_color.r, _color.g, _color.b);
}
unsigned int UtilityCommon::SetColor(COLOR_F _color)
{
    return SetColor(_color.r, _color.g, _color.b);
}
unsigned int UtilityCommon::SetColor(int _red, int _green, int _blue)
{
    return GetColor(_red, _green, _blue);
}

unsigned int UtilityCommon::SetColor(float _red, float _green, float _blue)
{
    COLOR_F color = { _red, _green, _blue };
    return GetColor(color.r, color.g, color.b);
}

bool UtilityCommon::WrapValue(VECTOR& _vec, const VECTOR& _maxVec, const VECTOR& _minVec)
{
    // 最小値を超えている場合は最大値に、最大値を超えている場合は最小値で返す
    bool isWrap = (WrapValue(_vec.x, _maxVec.x, _minVec.x) ||
                   WrapValue(_vec.y, _maxVec.y, _minVec.y) ||
                   WrapValue(_vec.z, _maxVec.z, _minVec.z));
    return isWrap;
}
bool UtilityCommon::WrapValue(Vector2& _vec, const Vector2& _maxVec, const Vector2& _minVec)
{
    bool isWrap = (WrapValue(_vec.x, _maxVec.x, _minVec.x) ||
                   WrapValue(_vec.y, _maxVec.y, _minVec.y));
    return isWrap;
}
bool UtilityCommon::WrapValue(int& _num, int _maxNum, int _minNum)
{
    bool isWrap = false;

    // 最小値を超えている場合は最大値に、最大値を超えている場合は最小値で返す
    if (_num < _minNum)
    {
        isWrap = true;
        _num = _maxNum;
    }
    else if (_num > _maxNum)
    {
        isWrap = true;
        _num = _minNum;
    }

    return isWrap;
}
bool UtilityCommon::WrapValue(float& _num, float _maxNum, float _minNum)
{
    bool isWrap = false;

    // 最小値を超えている場合は最大値に、最大値を超えている場合は最小値で返す
    if (_num < _minNum)
    {
        isWrap = true;
        _num = _maxNum;
    }
    else if (_num > _maxNum)
    {
        isWrap = true;
        _num = _minNum;
    }

    return isWrap;
}