#pragma once
#include <string>
#include <vector>
#include <variant> // 候補型リスト

class Resource
{
public:

	enum class LOAD_TYPE
	{
		NONE,
		IMAGE,  // 単一画像
		IMAGES, // 複数画像
		MODEL,  // 3Dモデル
		MOVIE,  // 映像ファイル
		EFFECT  // エフェクト
	};


	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	Resource(void);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">読み込むファイルの種類</param>
	/// <param name="path">ファイルパス(読み取り専用)</param>
	Resource(LOAD_TYPE type, const std::string &path);

	/// <summary>
	/// 複数画像時のコンストラクタ
	/// </summary>
	/// <param name="type">読み込むファイルの種類</param>
	/// <param name="path">ファイルパス(読み取り専用)</param>
	/// <param name="numX">横画像数</param>
	/// <param name="numY">縦画像数</param>
	/// <param name="sizeX">画像１枚の横サイズ</param>
	/// <param name="sizeY">画像１枚の縦サイズ</param>
	Resource(LOAD_TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Resource(void) = default;


	/// <summary>
	/// 読み込み処理
	/// </summary>
	void Load(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void);

	/// <summary>
	/// ハンドルコピー処理
	/// </summary>
	/// <param name="images">画像ハンドル</param>
	void CopyHandle(int* images);

	/// <summary>
	/// 重複させる3DモデルID 割り当て処理
	/// </summary>
	/// <param name="id"></param>
	void SetDuplicateModelId(int id);

	/// <summary>
	/// ハンドルID 取得処理
	/// </summary>
	/// <returns>ファイルのハンドル</returns>
	int GetHandleId(void) { return handleId_; };


private:

	LOAD_TYPE resType_; // 読み込むファイルの種類
	std::string path_;  // 読み込み先

	union HandleId
	{
		int id_;	// ハンドルID
		int* ids_; // 複数画像のポインタ
	};

	HandleId handleID_; // ハンドルID

	
	int handleId_;		// 単一読み込みハンドルID


	/* 複数画像読み込み用 */

	int* handleIds_; // 複数Idのポインタ
	int numX_;		 // 横画像数
	int numY_;		 // 縦画像数
	int sizeX_;		 // 横画像サイズ
	int sizeY_;		 // 縦画像サイズ

	// モデル複製用リスト
	std::vector<int> duplicationModelIds_;
};