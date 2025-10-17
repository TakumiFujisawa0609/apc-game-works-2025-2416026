#pragma once

#include <map>
#include <string>
class Resource;

/// <summary>
/// リソース管理マネージャ
/// </summary>
class ResourceManager
{
public:

	/// <summary>
	/// リソース識別列挙
	/// </summary>
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

	// BGMファイルパス
	static const std::string PATH_BGM;

	// 効果音ファイルパス
	static const std::string PATH_SE;

	// 動画ファイルパス
	static const std::string PATH_MOVIE;



	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	static ResourceManager& GetInstance(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Load(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	void Release(void);

	/// <summary>
	/// インスタンス削除
	/// </summary>
	void Destroy(void);


	/// <summary>
	/// リソースのロード
	/// </summary>
	/// <param name="src">読み込み対象</param>
	/// <returns>リソース</returns>
	Resource Load(SRC src);

	/// <summary>
	/// リソースのハンドルを取得
	/// </summary>
	/// <param name="src">読み込み対象</param>
	int LoadHandleId(SRC src);

	/// <summary>
	/// 3Dモデル重複利用時の読み込み
	/// </summary>
	/// <param name="src">読み込み対象</param>
	/// <returns>重複するモデルのハンドル</returns>
	int LoadModelDuplicate(SRC src);



private:

	//静的インスタンス
	static ResourceManager* instance_;

	// リソース管理対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	std::map<int, std::string> path_;


	/// <summary>
	/// デフォルトコンストラクタ(private化で外部生成不可化)
	/// </summary>
	ResourceManager(void);

	/// <summary>
	/// デストラクタ(private化で外部で生成不可化)
	/// </summary>
	~ResourceManager(void) = default;

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	ResourceManager(const ResourceManager& other) = default;


	/// <summary>
	/// リソースを取得する処理
	/// </summary>
	void SetResource(void);

	/// <summary>
	/// 内部ロード
	/// </summary>
	/// <param name="source">読み込み対象</param>
	/// <returns></returns>
	Resource* _Load(SRC source);
};