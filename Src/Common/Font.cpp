#include "Font.h"
#include "../Manager/ResourceManager.h"


const std::string Font::FONT_GAME_PATH = ResourceManager::PATH_FONT + "GameFont.ttf";
const std::string Font::FONT_GAME = "GameFont";
const std::string Font::GAME_FONT_NAME = "07にくまるフォント";


//シングルトンインスタンスの初期化
Font* Font::instance_ = nullptr;

//コンストラクタ
Font::Font() : defaultFont_("") {}

Font::~Font(void)
{
	/*　デストラクタ　*/

	//全てのフォントハンドルを解放
	for (auto& font : fontHandles_)
	{

		for (auto& innerPair : font.second)
		{
			int fontHandle = innerPair.second;
			if (fontHandle != -1 && fontHandle != DX_DEFAULT_FONT_HANDLE)
			{
				DeleteFontToHandle(fontHandle);
			}
		}
	}

	//動的フォントハンドルを解放
	for (const auto& dynamicFont : dynamicFontHandles_)
	{
		if (dynamicFont.second != -1 && dynamicFont.second != DX_DEFAULT_FONT_HANDLE)
		{
			DeleteFontToHandle(dynamicFont.second);
		}
	}
}

void Font::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Font();
	}
	instance_->Init();
}


Font& Font::GetInstance(void)
{
	/*　シングルトンインスタンス取得　*/
	if (instance_ == nullptr)
	{
		Font::CreateInstance();
	}
	return *instance_;
}

//フォントの初期化
void Font::Init(void)
{
}

//フォントの追加
bool Font::AddFont(const std::string& fontId, const std::string& internalFontName, const std::string& fontPath,
	int fontSize, int fontWeight, int fontType)
{
	int fontFileSize = FileRead_size(fontPath.c_str());
	int fontFileHandle = FileRead_open(fontPath.c_str());

	//フォント読み込み失敗
	if (fontFileSize <= 0 || fontFileHandle == -1)
	{
#ifdef _DEBUG
		OutputDebugString("\nフォントが見つかりませんでした \n");
#endif
		return false;
	}

	// フォントデータをメモリに読み込む
	void* buffer = new char[fontFileSize];
	FileRead_read(buffer, fontFileSize, fontFileHandle);
	FileRead_close(fontFileHandle);

	DWORD fontNum = 0;
	if (AddFontMemResourceEx(buffer, fontFileSize, NULL, &fontNum) == 0)
	{
#ifdef _DEBUG
		OutputDebugString("\nフォントファイルが読み込めませんでした\n");
#endif
		delete[] buffer;
		return false;
	}

	delete[] buffer; // メモリ解放

	fontNameMap_[fontId] = internalFontName;

	//フォントハンドルを生成
	int fontHandle = CreateFontToHandle(internalFontName.c_str(), fontSize, fontWeight, fontType);
	if (fontHandle == -1)
	{
#ifdef _DEBUG
		OutputDebugString("\n指定のハンドルにフォントがありませんでした\n");
#endif
		return false;
	}

	//フォントハンドルを登録
	fontHandles_[fontId][std::make_pair(fontSize, fontType)] = fontHandle;
	return true;
}

void Font::RemoveFont(const std::string& fontId)
{
	/*　フォントの削除　*/
	auto it = fontHandles_.find(fontId);
	if (it != fontHandles_.end())
	{
		// 内側のハンドルを全て削除
		for (auto& innerPair : it->second)
		{
			int fontHandle = innerPair.second;
			if (fontHandle != -1 && fontHandle != DX_DEFAULT_FONT_HANDLE)
			{
				DeleteFontToHandle(fontHandle);
			}
		}
		fontHandles_.erase(it);
	}
}

//デフォルトフォントの設定
void Font::SetDefaultFont(const std::string& fontId)
{
	if (fontHandles_.find(fontId) != fontHandles_.end())
	{
		defaultFont_ = fontId;
	}
}

void Font::DrawText(const std::string& fontId, int x, int y, const char* text,
	int color, int fontSize, int fontType)
{
	/*　テキスト描画　*/

	int fontHandle = -1;
	int useFontType = (fontType >= 0) ? fontType : FONT_TYPE_NORMAL;

	// フォント名取得
	auto itName = fontNameMap_.find(fontId);
	std::string internalFontName = (itName != fontNameMap_.end()) ? itName->second : "";

	if (fontSize > 0)
	{
		auto itFont = fontHandles_.find(fontId);
		if (itFont != fontHandles_.end())
		{
			auto& sizeMap = itFont->second;
			auto itSize = sizeMap.find({ fontSize, useFontType });
			if (itSize != sizeMap.end())
			{
				fontHandle = itSize->second;
			}
		}

		if (fontHandle == -1)
		{
			// ここで登録済みフォント名を使う
			fontHandle = GetDynamicFontHandle(internalFontName, fontSize, 3, useFontType);
		}
	}
	else
	{
		auto itFont = fontHandles_.find(fontId);
		if (itFont != fontHandles_.end())
		{
			auto& sizeMap = itFont->second;
			if (!sizeMap.empty())
			{
				fontHandle = sizeMap.begin()->second;
			}
		}
	}

	if (fontHandle == -1)
	{
		fontHandle = DX_DEFAULT_FONT_HANDLE;

#ifdef _DEBUG
		OutputDebugString("\n指定のフォントがありません\n");
#endif
	}

	DrawFormatStringFToHandle(x, y, color, fontHandle, text);
}

// デフォルトフォントで描画
void Font::DrawDefaultText(int x, int y, const char* text, int color, int fontSize, int fontType)
{
	DrawText(defaultFont_, x, y, text, color, fontSize, fontType);
}

//文字の横幅を取得
int Font::GetDefaultTextWidth(const std::string& text) const
{
	return GetDrawStringWidth(text.c_str(), static_cast<int>(text.size()));
}

//一時的なフォントを取得または生成
int Font::GetDynamicFontHandle(const std::string& internalFontName, int fontSize, int fontWeight, int fontType)
{
	auto key = std::make_pair(fontSize, fontType);
	auto it = dynamicFontHandles_.find(key);

	if (it != dynamicFontHandles_.end())
	{
		return it->second;
	}

	//新しいフォントを生成
	int fontHandle = CreateFontToHandle(internalFontName.c_str(), fontSize, fontWeight, fontType);

	if (fontHandle != -1)
	{
		dynamicFontHandles_[key] = fontHandle;
	}
	return fontHandle;
}
void Font::Destroy(void)
{
	/*　メモリ解放処理　*/
	if (instance_ != nullptr)
	{
		// デストラクタが呼ばれてフォントを解放
		delete instance_;
		instance_ = nullptr;
	}
}