#pragma once

#include <map>
#include <string>
class Resource;

class ResourceManager
{
public:

	enum SRC
	{
		NONE = 0,

		/*　エフェクトリソース　*/

		/*　画像リソース　*/

		/*　3Dモデルリソース　*/
		TEMP_MODEL,
		MODEL_BEAR,
	};


	const std::string PATH_CSV_FILE = "Data/CSV/";       // CSVファイルパス
	const std::string PATH_EFFECT	= "Data/Effect/";    // エフェクトファイルパス
	const std::string PATH_FONT		= "Data/Font/";	     // フォントファイルパス
	const std::string PATH_IMAGE	= "Data/Image/";     // 画像ファイルパス
	const std::string PATH_MODEL	= "Data/Model/";     // 3Dモデルファイルパス
	const std::string PATH_BGM      = "Data/Sound/BGM/"; // BGMファイルパス
	const std::string PATH_SE		= "Data/Sound/SE/";  // 効果音ファイルパス



	/// <summary>
	/// インスタンス生成
	/// </summary>
	static void CreateInstance(void);

	/// <summary>
	/// インスタンス取得処理
	/// </summary>
	/// <returns>リソースマネージャ</returns>
	static ResourceManager& GetInstance(void);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(void);

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
	/// 3Dモデル重複利用時の読み込み
	/// </summary>
	/// <param name="src">読み込み対象</param>
	/// <returns>重複するモデルのハンドル</returns>
	int LoadModelDuplicate(SRC src);



private:

	//静的インスタンス
	static ResourceManager* manager_;

	// リソース管理対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;


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