#include "StatusData.h"
#include <DxLib.h>
#include <fstream>
#include <sstream>
#include <string>
#include "../../Manager/ResourceManager.h"


StatusData* StatusData::instance_ = nullptr;

StatusData* StatusData::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new StatusData;
	}
	return instance_;
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
	int max = static_cast<int>(DATA_PLAYER::MAX);
	for (int y = 0; y < SAVE_LENGTH; y++)
	{
		for (int x = 0; x < max; x++)
		{
			loadString_[y][x] = "";
			//if (x < (max - 1) && y < (SAVE_LENGTH - 1)) loadValue_[y][x] = 0.0f;
		}
	}
}

void StatusData::LoadPlayer(void)
{

}

void StatusData::LoadEnemy(void)
{
}

void StatusData::LoadWeapon(void)
{
}


void StatusData::DrawDebug(void)
{
	//int font = Font::GetInstance()->GetFontHandle();
	int param = static_cast<int>(DATA_PLAYER::MAX) - 1;
	for (int y = 0; y < (SAVE_LENGTH - 1); y++)
	{
		//int font = Font::GetInstance()->GetFontHandle();
		//float score = loadValue_[y][0];
		//float time  = loadValue_[y][1];
		//DrawText (0, y, loadValue_[y][0], )
		//DrawFormatString(0,y * 16,0xFFFFFF, "score：%.f, time：%.f", score, time);
		//DrawFormatStringToHandle(0, (y * 16), 0xFFFF, font, "score：%.3f, time：%3.f", score, time);
	}
}


bool StatusData::LoadCSV(const char* _filePath)
{
	/*　csvファイル読み込み処理　*/

	// 行
	std::string line;

	// セーブファイル読み込み
	std::ifstream file(_filePath);

	int target = 0;
	int param  = 0; // 種類

	if (file.is_open())
	{
		// 行読み込み
		while (getline(file, line))
		{
			std::stringstream ss(line);
			std::string value;

			// コンマごとに取得
			while (getline(ss, value, ','))
			{
				// カンマごとに区別されていない
				if (target < SAVE_LENGTH)
				{
					loadString_[param][target] = value;
					target++;
				}
			}
			target = 0;
			param++;
		}
		file.close();
	}
	else
	{
#ifdef _DEBUG
		OutputDebugString("\n保存ファイルが開かれませんでした。\n\n");
#endif
		return false;
	}

	/*　数値読込　*/
	int max = static_cast<int>(DATA_PLAYER::MAX);
	for (int y = 0; y < (SAVE_LENGTH - 1); y++)
	{
		// ラベル以外を取得
		for (int x = 0; x < (max - 1); x++)
		{
			// string型 → float型 変換読み込み
			//loadValue_[y][x] = std::stof(loadString_[y + 1][x + 1]);
		}
	}

	return true;
}


bool StatusData::SaveCSV(void)
{
	/* CSV書き込み処理 */

	int max = static_cast<int>(DATA_PLAYER::MAX);

	/*　データ保存処理　*/
	for (int y = 0; y < (SAVE_LENGTH - 1); y++)
	{
		// ラベル以外を書き出す
		for (int x = 0; x < (max - 1); x++)
		{
			// 数値 → 文字列 変換読み込み
			//loadString_[y + 1][x + 1] = std::to_string(loadValue_[y][x]);
		}
	}

	//保存ファイル読み込み
	std::ofstream outputFile(DATA_HANDLE);
	if (!outputFile.is_open())
	{
		OutputDebugString("\n保存ファイルが開かれませんでした。\n");
		return false;
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
	OutputDebugString("\n正常に保存完了しました\n");
	return true;
}


/// <summary>
/// 数値取得処理
/// </summary>
/// <param name="load">読み込む対象</param>
/// <param name="type">読み込む種類</param>
float StatusData::GetScore(int load, DATA_PLAYER type)
{
	int loadType = static_cast<int>(type);
	int max		 = static_cast<int>(DATA_PLAYER::MAX);

	if (load < 0	 || load >= (SAVE_LENGTH - 1) ||
		loadType < 0 || loadType >= (max - 1))
	{
		// ラベル含め取得範囲外時、処理終了
		OutputDebugString("\n数値の読み込みに失敗しました。\n");
		return 0.0f;
	}

	// 数値読み込み
	//return loadValue_[load][loadType];
	return 0.f;
}


void StatusData::SetScore(int save, STATUS_TYPE type, float saveValue)
{
	int saveType = static_cast<int>(type);
	int max		 = static_cast<int>(DATA_PLAYER::MAX);

	if (save < 0	 || save >= (SAVE_LENGTH - 1) ||
		saveType < 0 || saveType >= (max - 1))
	{
		// ラベル含め取得範囲外時、処理終了
		OutputDebugString("\n数値の保存に失敗しました。\n");
		return;
	}

	// 数値保存
	//loadValue_[save][saveType] = saveValue;
}