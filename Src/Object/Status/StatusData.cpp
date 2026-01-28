#include "StatusData.h"
#include <DxLib.h>
#include <fstream>
#include <sstream>
#include <array>
#include <string>
#include <memory>
#include <cassert>
#include <map>

#include "./StatusPlayer.h"
#include "./StatusEnemy.h"
#include "./StatusWeapon.h"
#include "../../Manager/ResourceManager.h"

StatusData* StatusData::instance_ = nullptr;

void StatusData::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new StatusData();
	}
	// マネージャ読み込み処理
	instance_->Load();
}

StatusData& StatusData::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		CreateInstance();
	}
	return *instance_;
}

void StatusData::Destroy(void)
{
	if (instance_ != nullptr)
	{
		delete instance_;
	}
}

StatusData::StatusData(void)
{
#ifndef _DEBUG
	// リリースビルド時のみDXアーカイブを使用
	SetUseDXArchiveFlag(TRUE);
#endif
}



void StatusData::Load(void)
{
	LoadPlayerStatus();

	LoadPlayerMotion();

	LoadEnemy();

	LoadWeapon();
}

std::string StatusData::ReadCsvFile(const std::string& path)
{
	std::string content;

#ifndef _DEBUG
	// デバッグビルド：通常のファイルシステムから読み込み
	std::ifstream file(path);

	if (!file.is_open())
	{
		std::string error = "\ncsvファイルが読み込まれませんでした。\nファイルパス:";
		error += path;
		OutputDebugString(error.c_str());
		assert(false);
		return "";
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	content = buffer.str();
	file.close();

#else
	// リリースビルド：DXアーカイブから読み込み
	int fileHandle = FileRead_open(path.c_str());

	if (fileHandle == 0)
	{
		std::string error = "\ncsvファイルが読み込まれませんでした。\nファイルパス:";
		error += path;
		OutputDebugString(error.c_str());
		assert(false);
		return "";
	}

	// バッファ確保
	int fileSize = FileRead_size(path.c_str());
	char* buffer = new char[fileSize + 1];

	FileRead_read(buffer, fileSize, fileHandle);
	buffer[fileSize] = '\0';
	FileRead_close(fileHandle);

	content = std::string(buffer);
	delete[] buffer;
#endif

	return content;
}


void StatusData::LoadPlayerStatus(void)
{
	/*　csvファイル読み込み処理　*/

	// 文字列の一時格納配列
	int max = static_cast<int>(StatusPlayer::PARAM::MAX);
	std::string dataText[static_cast<int>(StatusPlayer::PARAM::MAX)];

	// 行
	std::string line;
	std::string path = (ResourceManager::PATH_CSV + PATH_PLAYER);

	// セーブファイルパス

	// CSVファイルの内容を取得
	std::string fileContent = ReadCsvFile(path);
	std::istringstream fileStream(fileContent);

	int length = 0;
	int param = 0; // 種類


	// 行読み込み
	while (getline(fileStream, line))
	{
		std::stringstream ss(line);
		std::string text;

		if (param == 0)
		{
			// ラベルはスキップ
			param++;
			length = 0;
			continue;
		}

		// 列読み込み(コンマごと)
		while (getline(ss, text, ','))
		{
			// カンマごとに区別されていない
			if (length < max)
			{
				dataText[length] = text;
				length++;
			}
		}

		length = 0;
		param++;
	}


	// 数値読込
	player_ = std::make_unique<StatusPlayer>();
	player_->LoadStatusParam(std::to_array(dataText));
}

void StatusData::LoadPlayerMotion(void)
{
	/*　csvファイル読み込み処理　*/

	// 文字列の一時格納配列
	int max = static_cast<int>(StatusPlayer::MOTION_TYPE::MAX);
	std::string dataText[static_cast<int>(StatusPlayer::MOTION_TYPE::MAX)][static_cast<int>(StatusPlayer::MOTION_PARAM::MAX)];

	// 行
	std::string line;
	std::string path = (ResourceManager::PATH_CSV + PATH_PLAYER_MOTION);


	// CSVファイルの内容を取得
	std::string fileContent = ReadCsvFile(path);
	std::istringstream fileStream(fileContent);

	int length = 0;
	int param = 0; // 種類

	// 行読み込み
	while (getline(fileStream, line))
	{
		std::stringstream ss(line);
		std::string text;

		if (param == 0)
		{
			// ラベルはスキップ
			param++;
			length = 0;
			continue;
		}

		// 列読み込み(コンマごと)
		while (getline(ss, text, ','))
		{
			// カンマごとに区別されていない
			if (length < max)
			{
				dataText[(param - 1)][length] = text;
				length++;
			}
		}

		length = 0;
		param++;
	}

	for (int i = 0; i < (param - 1); i++)
	{
		// プレイヤーのモーション時間の登録
		StatusPlayer::MOTION_TYPE type = static_cast<StatusPlayer::MOTION_TYPE>(i);

		// モーション格納
		player_->LoadMotionParam(type, std::to_array(dataText[i]));
	}
}

void StatusData::LoadEnemy(void)
{
	/*　csvファイル読み込み処理　*/

	// 文字列の一時格納配列
	int max = static_cast<int>(StatusEnemy::PARAM::MAX);
	std::string dataText[static_cast<int>(StatusEnemy::TYPE::MAX)][static_cast<int>(StatusEnemy::PARAM::MAX)];

	// 行
	std::string line;
	std::string path = (ResourceManager::PATH_CSV + PATH_ENEMY);

	// CSVファイルの内容を取得
	std::string fileContent = ReadCsvFile(path);
	std::istringstream fileStream(fileContent);

	int length = 0;
	int param = 0; // 種類

	// 行読み込み
	while (getline(fileStream, line))
	{
		std::stringstream ss(line);
		std::string text;

		if (param == 0)
		{
			// ラベルはスキップ
			param++;
			length = 0;
			continue;
		}

		// 列読み込み(コンマごと)
		while (getline(ss, text, ','))
		{
			// カンマごとに区別されていない
			if (length < max)
			{
				dataText[(param - 1)][length] = text;
				length++;
			}
		}

		length = 0;
		param++;
	}

	for (int i = 0; i < (param - 1); i++)
	{
		// 敵パラメータ登録
		enemy_[i] = std::make_unique<StatusEnemy>();
		enemy_[i]->LoadParam(std::to_array(dataText[i]));
	}
}

void StatusData::LoadWeapon(void)
{
	/*　csvファイル読み込み処理　*/

	// 文字列の一時格納配列
	int max = static_cast<int>(StatusWeapon::PARAM::MAX);
	std::string dataText[static_cast<int>(StatusWeapon::TYPE::MAX)][static_cast<int>(StatusWeapon::PARAM::MAX)];

	// 行
	std::string line;
	std::string path = (ResourceManager::PATH_CSV + PATH_WEAPON);

	// CSVファイルの内容を取得
	std::string fileContent = ReadCsvFile(path);
	std::istringstream fileStream(fileContent);

	int length = 0;
	int param = 0; // 種類


	// 行読み込み
	while (getline(fileStream, line))
	{
		std::stringstream ss(line);
		std::string text;

		if (param == 0)
		{
			// ラベルはスキップ
			param++;
			length = 0;
			continue;
		}

		// 列読み込み(コンマごと)
		while (getline(ss, text, ','))
		{
			// カンマごとに区別されていない
			if (length < max)
			{
				dataText[(param - 1)][length] = text;
				length++;
			}
		}

		length = 0;
		param++;
	}

	for (int i = 0; i < (param - 1); i++)
	{
		// 武器パラメータ登録
		weapon_[i] = std::make_unique<StatusWeapon>();
		weapon_[i]->LoadParam(std::to_array(dataText[i]));
	}
}


void StatusData::SaveCSV(void)
{
	/* CSV書き込み処理 */
	/*

	int max = static_cast<int>(DATA_PLAYER::MAX);

	//　データ保存処理
	for (int y = 0; y < (SAVE_LENGTH - 1); y++)
	{
		// ラベル以外を書き出す
		for (int x = 0; x < (max - 1); x++)
		{
			// 数値 → 文字列 変換読み込み
			//loadString_[y + 1][x + 1] = std::to_string(loadValue_[y][x]);
		}
	}

	// 保存ファイル読み込み
	std::ofstream outputFile(DATA_HANDLE);
	if (!outputFile.is_open())
	{
		OutputDebugString(w"\ncsvファイルが開かれませんでした。\n");
		assert(false);
	}

	for (int y = 0; y < SAVE_LENGTH; ++y)
	{
		for (int x = 0; x < max; ++x)
		{
			// データ入力
			outputFile << loadString_[y][x];

			// カンマ入力
			if (x < (max - 1)) outputFile << ",";
		}
		outputFile << "\n"; // 改行
	}
	outputFile.close(); //保存終了

#ifdef _DEBUG
	OutputDebugString("\n正常に保存完了しました\n");
#endif
	*/
}


std::string& StatusData::GetHandlePathPlayer(void)
{
	return player_->GetHandlePath();
}

std::string& StatusData::GetHandlePathEnemy(int type)
{
	return enemy_[type]->GetHandlePath();
}

std::string& StatusData::GetHandlePathWeapon(int type)
{
	return weapon_[type]->GetHandlePath();
}