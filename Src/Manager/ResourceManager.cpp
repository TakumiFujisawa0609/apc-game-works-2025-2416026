
#include "Resource.h"
#include "ResourceManager.h"
#include "../Application.h"
#include <DxLib.h>

ResourceManager* ResourceManager::manager_ = nullptr;

/// <summary>
/// インスタンス生成
/// </summary>
void ResourceManager::CreateInstance(void)
{
	if (manager_ == nullptr)
	{
		manager_ = new ResourceManager();
	}

	manager_->Init(); // マネージャ初期化
}

/// <summary>
/// インスタンス取得処理
/// </summary>
ResourceManager& ResourceManager::GetInstance(void)
{
	return *manager_;
}

/// <summary>
/// デフォルトコンストラクタ
/// </summary>
/// <param name=""></param>
ResourceManager::ResourceManager(void)
{
	
}


/// <summary>
/// 初期化処理
/// </summary>/// <returns>正常に動作したか否か</returns>
void ResourceManager::Init(void)
{
	SetResource(); // リソース取得
}

/// <summary>
/// リソースを取得する処理
/// </summary>
void ResourceManager::SetResource(void)
{
	Resource res;

	res = Resource(Resource::LOAD_TYPE::MODEL, PATH_MODEL + "Temp/Temp.mv1");
	resourcesMap_.emplace(SRC::TEMP_MODEL, res);

	res = Resource(Resource::LOAD_TYPE::MODEL, PATH_MODEL + "Player/Bear.mv1");
	resourcesMap_.emplace(SRC::MODEL_BEAR, res);
}

/// <summary>
/// 解放処理
/// </summary>
void ResourceManager::Release(void)
{
	if (loadedMap_.empty()) return;

	for (auto& p : loadedMap_) 
	{
		p.second->Release(); // 読み込み済みリソース解放
		delete p.second;	 // リソース要素削除
	}

	loadedMap_.clear(); // 読み込み済みリソースリストをクリア
}

/// <summary>
/// インスタンス削除処理
/// </summary>
void ResourceManager::Destroy(void)
{
	Release(); // リソースマネージャ解放

	if (!resourcesMap_.empty())
	{
		resourcesMap_.clear(); // リソースリストをクリア(空の時は行わない)
	}

	delete manager_; // インスタンス削除
}


/// <summary>
/// リソースのロード
/// </summary>
/// <param name="src">読み込み対象</param>
/// <returns>リソース</returns>
Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src); // 読み込み処理

	if (res == nullptr) return Resource();

	Resource ret = *res;

	return *res;
}

/// <summary>
/// 読み込み処理
/// </summary>
/// <param name="src">読み込み対象</param>
/// <returns>リソース</returns>
Resource* ResourceManager::_Load(SRC src)
{
	// 読み込み済みリストを検索
	const auto& loaded = loadedMap_.find(src);

	//読み込み済みリストに対象がある時、要素を返す
	if (loaded != loadedMap_.end()) return loaded->second;


	// リソースリスト内を検索
	const auto& resource = resourcesMap_.find(src);

	// リソースリストに登録されてない時、NULLを返す
	if (resource == resourcesMap_.end()) return nullptr;


	// リソースリスト登録済み時、読み込み処理
	resource->second.Load();

	// 念のためにコピーコンストラクタ
	Resource* ret = new Resource(resource->second);

	// 読み込み済みリストに格納
	loadedMap_.emplace(src, ret);

	return ret;
}

/// <summary>
/// 3Dモデル重複利用時の読み込み
/// </summary>
/// <param name="src">読み込み対象</param>
/// <returns>重複するモデルのハンドル</returns>
int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* resource = _Load(src); // 読み込み処理

	// 読み込み失敗
	if (resource == nullptr) return -1;

	// 重複するモデルのハンドルを取得
	int id = MV1DuplicateModel(resource->GetHandleId());

	// 重複モデルリストにハンドル追加
	resource->SetDuplicateModelId(id);

	return id;
}