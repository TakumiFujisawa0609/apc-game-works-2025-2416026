
#include "Resource.h"
#include "ResourceManager.h"
#include <DxLib.h>
#include <string>
#include "../Application.h"

ResourceManager* ResourceManager::instance_ = nullptr;

// ファイルパスの割り当て
const std::string ResourceManager::PATH_EFFECT = "Data/Effect/";
const std::string ResourceManager::PATH_FONT   = "Data/Font/";
const std::string ResourceManager::PATH_IMAGE  = "Data/Image/";
const std::string ResourceManager::PATH_MODEL  = "Data/Model/";
const std::string ResourceManager::PATH_SE     = "Data/Sound/SE/";
const std::string ResourceManager::PATH_BGM    = "Data/Sound/BGM/";
const std::string ResourceManager::PATH_MOVIE  = "Data/Movie/";


void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}

	instance_->Init(); // マネージャ初期化
}

ResourceManager& ResourceManager::GetInstance(void)
{
	/*　インスタンス取得処理　*/
	return *instance_;
}

ResourceManager::ResourceManager(void)
{
	
}


void ResourceManager::Init(void)
{
	SetResource(); // リソース取得
}

void ResourceManager::SetResource(void)
{
	Resource res;

	res = Resource(Resource::LOAD_TYPE::MODEL, PATH_MODEL + "Temp/Temp.mv1");
	resourcesMap_.emplace(SRC::TEMP_MODEL, res);

	res = Resource(Resource::LOAD_TYPE::MODEL, PATH_MODEL + "Player/Bear.mv1");
	resourcesMap_.emplace(SRC::MODEL_BEAR, res);
}


void ResourceManager::Release(void)
{
	/* メモリ解放処理 */

	if (loadedMap_.empty()) return;

	for (auto& p : loadedMap_) 
	{
		// 読み込み済みリソース解放
		p.second->Release(); 
		delete p.second;
	}

	// 読み込み済みリソースリストをクリア
	loadedMap_.clear(); 
}

void ResourceManager::Destroy(void)
{
	/*　インスタンス削除処理　*/

	Release(); // リソースマネージャ解放

	if (!resourcesMap_.empty())
	{
		// リソースリストをクリア(空の時は行わない)
		resourcesMap_.clear();
	}

	delete instance_; // インスタンス削除
}


Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src); // 読み込み処理

	if (res == nullptr) return Resource();

	Resource ret = *res;

	return *res;
}

int ResourceManager::LoadHandleId(SRC src)
{
	/*　ハンドルIDの取得処理　*/
	return Load(src).GetHandleId();
}

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

int ResourceManager::LoadModelDuplicate(SRC src)
{
	/* 3Dモデル重複利用時の読み込み */

	// 読み込み処理
	Resource* resource = _Load(src);

	// 読み込み失敗
	if (resource == nullptr) return -1;

	// 重複するモデルのハンドルを取得
	int id = MV1DuplicateModel(resource->GetHandleId());

	// 重複モデルリストにハンドル追加
	resource->SetDuplicateModelId(id);

	return id;
}