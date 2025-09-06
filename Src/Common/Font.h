#pragma once
#include<string>
#include<unordered_map>
#include<DxLib.h>

class Font
{
public:

	// ハッシュ関数の定義
	struct PairHash
	{
		std::size_t operator()(const std::pair<int, int>& p) const noexcept
		{
			return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
		}
	};


	// 通常フォント
	static constexpr unsigned int FONT_TYPE_NORMAL = DX_FONTTYPE_NORMAL;

	// 縁取りフォント
	static constexpr unsigned int FONT_TYPE_EDGE = DX_FONTTYPE_EDGE;

	// アンチエイリアスフォント
	static constexpr unsigned int FONT_TYPE_ANTIALIASING = DX_FONTTYPE_ANTIALIASING;

	// アンチエイリアス + 縁取りフォント
	static constexpr unsigned int FONT_TYPE_ANTIALIASING_EDGE = DX_FONTTYPE_ANTIALIASING_EDGE;

	// ゲーム用フォントのパス
	static const std::string FONT_GAME_PATH;

	// ゲーム用フォント
	static const std::string FONT_GAME;

	// ゲーム用フォントの元の名前
	static const std::string GAME_FONT_NAME;

	// フォントの幅
	static constexpr int GAME_FONT_WIDTH = 10;

	// フォントのサイズ
	static constexpr int GAME_FONT_SIZE = 25;



	/// <summary>
	/// デストラクタ
	/// </summary>
	~Font(void);

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	//インスタンス取得
	static Font& GetInstance(void);

	//フォントの初期化
	void Init(void);

	//フォントの追加
	bool AddFont(const std::string& fontId, const std::string& internalFontName, const std::string& fontPath,
		int fontSize, int fontWeight, int fontType);

	//フォントの削除
	void RemoveFont(const std::string& fontId);

	//デフォルトフォントの設定
	void SetDefaultFont(const std::string& fontId);

	//テキスト描画
	void DrawText(const std::string& fontId, int x, int y, const char* text, int color, int fontSize = -1, int fontType = -1);

	/// <summary>
	/// デフォルトフォントで描画
	/// </summary>
	/// <param name="x">横位置</param>
	/// <param name="y">縦位置</param>
	/// <param name="text">テキスト</param>
	/// <param name="color">テキストの色</param>
	/// <param name="fontSize">テキストのサイズ</param>
	/// <param name="fontType">フォントの種類</param>
	void DrawDefaultText(int x, int y, const char* text, int color, int fontSize = -1, int fontType = -1);

	//文字の横幅を取得
	int GetDefaultTextWidth(const std::string& text) const;


	/// <summary>
	/// メモリ解放
	/// </summary>
	void Destroy(void);


private:

	//シングルトンインスタンス
	static Font* instance_;

	//フォントハンドルのマップ
	std::unordered_map<std::string, std::unordered_map<std::pair<int, int>, int, PairHash>> fontHandles_;

	// 動的フォントサイズとタイプのキャッシュ
	std::unordered_map<std::pair<int, int>, int, PairHash> dynamicFontHandles_;

	//デフォルトフォント
	std::string defaultFont_;

	// フォントIDに対応した内部フォント名のマップ
	std::unordered_map<std::string, std::string> fontNameMap_;

	//コンストラクタはプライベート
	Font(void);

	//一時的なフォントを取得または生成

	/// <summary>
	/// フォント表示設定
	/// </summary>
	/// <param name="fontSize">フォントの大きさ</param>
	/// <param name="fontWeight">フォントの幅</param>
	/// <param name="fontType">フォントのタイプ</param>
	int GetDynamicFontHandle(const std::string& internalFontName, int fontSize, int fontWeight, int fontType);

};