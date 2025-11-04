#pragma once

#include <map>
#include <string>
class Resource;

class ResourceManager
{
public:

	/// @brief リソース識別列挙
	enum SRC
	{
		/*　3Dモデルリソース　*/
		// プレイヤー
		MODEL_PLAYER = 0,
		// 敵
		MODEL_SKELETON_WARRIOR = 1,
		MODEL_SKELETON_MAGE,

		// 武器
		MODEL_WEAPON,

		/*　エフェクトリソース　*/

		/*　画像リソース　*/
		IMAGE_PAD,

		/*　アニメーションリソース　*/
		ANIM_SWORD_1,
		ANIM_SWORD_2,
		ANIM_SWORD_3,
	};


	/*　各ハンドルパス　*/

	// エフェクトファイルパス
	static const std::string PATH_EFFECT;

	// フォントファイルパス
	static const std::string PATH_FONT;

	// 画像ファイルパス
	static const std::string PATH_IMAGE;

	// 3Dモデルファイルパス
	static const std::string PATH_MODEL;

	// アニメーションファイルパス
	static const std::string PATH_ANIM;

	// BGMファイルパス
	static const std::string PATH_BGM;

	// 効果音ファイルパス
	static const std::string PATH_SE;

	// 動画ファイルパス
	static const std::string PATH_MOVIE;



	/// @brief インスタンス生成
	static void CreateInstance(void);

	/// @brief インスタンス取得処理
	static ResourceManager& GetInstance(void);

	/// @brief 初期化処理
	void Load(void);

	/// @brief 解放処理
	void Release(void);

	/// @brief インスタンス削除
	void Destroy(void);


	/// @brief リソースのロード
	/// @param _src 読み込み対象
	Resource Load(SRC _src);

	/// @brief リソースのハンドルを取得
	/// @param _src 読み込み対象
	int LoadHandleId(SRC _src);

	/// @brief 3Dモデル重複利用時の読み込み
	/// @param src 読み込み対象
	int LoadModelDuplicate(SRC src);



private:

	//静的インスタンス
	static ResourceManager* instance_;

	// リソース管理対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;


	/// @brief デフォルトコンストラクタ(private化で外部生成不可化)
	ResourceManager(void);

	/// @brief デフォルトデストラクタ(private化で外部で生成不可化)
	~ResourceManager(void) = default;

	/// @brief コピーコンストラクタ対策
	ResourceManager(const ResourceManager& _other) = default;


	/// @brief リソースを取得する処理
	void SetResource(void);

	/// @brief 内部ロード処理
	/// @param _source 読み込み対象
	Resource* _Load(SRC _source);
};