#include "ResourceManager.h"
#include "Resource.h"
#include <DxLib.h>
#include <string>
#include <unordered_map>
#include "../Application.h"
#include "../Object/Status/StatusData.h"
#include "../Object/Status/StatusEnemy.h"
#include "../Object/Status/StatusWeapon.h"

ResourceManager* ResourceManager::instance_ = nullptr;

// ファイルパスの割り当て
const std::string ResourceManager::PATH_EFFECT = "Data/Effect/";
const std::string ResourceManager::PATH_FONT   = "Data/Font/";
const std::string ResourceManager::PATH_IMAGE  = "Data/Image/";
const std::string ResourceManager::PATH_MODEL  = "Data/Model/";
const std::string ResourceManager::PATH_ANIM  = "Data/Model/Animation/";
const std::string ResourceManager::PATH_SE     = "Data/Sound/SE/";
const std::string ResourceManager::PATH_BGM    = "Data/Sound/BGM/";
const std::string ResourceManager::PATH_MOVIE  = "Data/Movie/";


void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}

	instance_->Load();
}

ResourceManager::ResourceManager(void)
{
	
}


void ResourceManager::Load(void)
{
	SetResource(); // リソース取得
}

void ResourceManager::SetResource(void)
{
	
	StatusData& data = StatusData::GetInstance();
	Resource res;
	int max = 0;
	int src = 0;

	// プレイヤー
	res = Resource(Resource::LOAD_TYPE::MODEL, PATH_MODEL + data.GetHandlePathPlayer());
	resourcesMap_.emplace(static_cast<SRC>(src), res);

	// 敵
	max = static_cast<int>(StatusEnemy::TYPE::MAX) - 1;
	src = static_cast<int>(SRC::MODEL_ENEMY);
	for (int i = 0; i < max; i++)
	{
		src++;
		res = Resource(Resource::LOAD_TYPE::MODEL, PATH_MODEL + data.GetHandlePathEnemy(i));
		resourcesMap_.emplace(static_cast<SRC>(src), res);
	}

	// 武器
	max = static_cast<int>(StatusWeapon::TYPE::MAX) - 1;
	src = static_cast<int>(SRC::MODEL_WEAPON);
	for (int i = 0; i < max; i++)
	{
		src++;
		res = Resource(Resource::LOAD_TYPE::MODEL, PATH_MODEL + data.GetHandlePathWeapon(i));
		resourcesMap_.emplace(static_cast<SRC>(src), res);
	}

	res = Resource(Resource::LOAD_TYPE::MODEL, PATH_MODEL + "Stage/Road.mv1");
	resourcesMap_.emplace(SRC::MODEL_ROAD, res);

	res = Resource(Resource::LOAD_TYPE::MODEL, PATH_MODEL + "Stage/mapObject.mv1");
	resourcesMap_.emplace(SRC::MODEL_STAGE, res);

	// 画像
	res = Resource(Resource::LOAD_TYPE::IMAGE, PATH_IMAGE + "Title.png");
	resourcesMap_.emplace(SRC::IMG_TITLE, res);

	res = Resource(Resource::LOAD_TYPE::IMAGE, PATH_IMAGE + "GameClear.png");
	resourcesMap_.emplace(SRC::IMG_CLEAR, res);

	res = Resource(Resource::LOAD_TYPE::IMAGE, PATH_IMAGE + "SpawnSkeleton.png");
	resourcesMap_.emplace(SRC::IMG_SPAWNCIRCLE, res);


	// アニメーション
	res = Resource(Resource::LOAD_TYPE::ANIM, PATH_ANIM + "SwordAnim1.mv1");
	resourcesMap_.emplace(SRC::ANIM_JUB_1_SWORD, res);

	res = Resource(Resource::LOAD_TYPE::ANIM, PATH_ANIM + "SwordAnim2.mv1");
	resourcesMap_.emplace(SRC::ANIM_JUB_2_SWORD, res);

	res = Resource(Resource::LOAD_TYPE::ANIM, PATH_ANIM + "SwordAnim3.mv1");
	resourcesMap_.emplace(SRC::ANIM_JUB_3_SWORD, res);


	res = Resource(Resource::LOAD_TYPE::ANIM, PATH_ANIM + "SwordAnim3.mv1");
	resourcesMap_.emplace(SRC::ANIM_SPECIAL_SWORD, res);


	res = Resource(Resource::LOAD_TYPE::ANIM, PATH_ANIM + "SwordAnim1.mv1");
	resourcesMap_.emplace(SRC::ANIM_STRONG_1_SWORD, res);

	res = Resource(Resource::LOAD_TYPE::ANIM, PATH_ANIM + "SwordAnim2.mv1");
	resourcesMap_.emplace(SRC::ANIM_STRONG_2_SWORD, res);

	res = Resource(Resource::LOAD_TYPE::ANIM, PATH_ANIM + "SwordAnim3.mv1");
	resourcesMap_.emplace(SRC::ANIM_STRONG_3_SWORD, res);

	res = Resource(Resource::LOAD_TYPE::ANIM, PATH_ANIM + "DanceAnim.mv1");
	resourcesMap_.emplace(SRC::ANIM_DANCE, res);


	// エフェクト
	res = Resource(Resource::LOAD_TYPE::EFFECT, PATH_EFFECT + "SpawnSkeleton.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_SPAWN_SKELETON, res);

	res = Resource(Resource::LOAD_TYPE::EFFECT, PATH_EFFECT + "SwordSlash.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_SWORD_SLASH, res);

	res = Resource(Resource::LOAD_TYPE::EFFECT, PATH_EFFECT + "SwordSlash.efkefc");
	resourcesMap_.emplace(SRC::EFFECT_SWORD_SLASH, res);
}


void ResourceManager::Release(void)
{
	/* メモリ解放処理 */

	if (!resourcesMap_.empty())
	{
		// リソースリストをクリア(空の時は行わない)
		resourcesMap_.clear();
	}
	if (!loadedMap_.empty())
	{
		for (auto& [src, resource] : loadedMap_)
		{
			// 読み込み済みリソース解放
			resource->Release();
			delete resource;
		}

		// 読み込み済みリソースリストをクリア
		loadedMap_.clear();
	}
}

void ResourceManager::Destroy(void)
{
	/*　インスタンス削除処理　*/

	instance_->Release();
	delete instance_;
}


Resource ResourceManager::Load(SRC src)
{
	// 読み込み処理
	Resource* res = _Load(src);

	if (res == nullptr) return Resource();

	Resource ret = *res;

	return *res;
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


int ResourceManager::LoadHandleId(SRC src)
{
	/*　ハンドルIDの取得処理　*/
	return Load(src).GetHandleId();
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	/* 3Dモデル重複利用時の読み込み */

	// 読み込み処理
	Resource* resource = _Load(src);

	// 読み込み失敗
	if (resource == nullptr)
	{
		return -1;
	}

	// 重複するモデルのハンドルを取得
	int id = MV1DuplicateModel(resource->GetHandleId());

	// 重複モデルリストにハンドル追加
	resource->SetDuplicateModelId(id);

	return id;
}