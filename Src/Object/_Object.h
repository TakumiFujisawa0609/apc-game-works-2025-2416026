#pragma once
#include <DxLib.h>
#include <iostream>
#include <list>
#include <memory>
#include <map>
#include <string>
#include <algorithm>

class AnimationCoontroller;
class ComponentBase;


class _Object
{
public:

	/// <summary>
	/// コンポーネント種類
	/// </summary>
	enum class COMP_TYPE
	{
		NONE = -1,
		MOVE,
	};

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	_Object(void);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~_Object(void) = default;

	/// <summary>
	/// ロード処理
	/// </summary>
	virtual void Load(void) = 0;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init(void) = 0;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(void);

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw(void);

	/// <summary>
	/// 解放処理
	/// </summary>
	virtual void Release(void);


	template<class T, typename... Args>
	T* AddComponent(Args&&... args)
	{
		// 完全転送[スマートポインタを生成してリストに格納]
		auto compo  = std::make_unique<T>(new T(std::forward<Args>(args)...));
		T* compoPtr = compo.get();
		components_.emplace(std::move(compo));
		return compoPtr;
	};

	template<typename T>
	T* GetComponent(void)
	{
		for (auto& compo : components_)
		{
			T* res = dynamic_cast<T*>(compo.get());
			if (res) return res;
		}
		return nullptr;
	}

	/// <sammary>
	/// 指定のコンポーネントがあるか判定
	/// </samarry>
	template<typename T>
	bool IsCheckComponent(void)
	{
		return (GetComponent<T>() != nullptr);
	};


	/// <summary>
	/// オブジェクトの有効判定割り当て
	/// </summary>
	void SetIsActive(bool flag) { isActive_ = flag; };

	/// <summary>
	/// オブジェクトの有効判定取得
	/// </summary>
	bool GetIsActive(void) { return isActive_; };

	const std::string_view GetName(void) {};



protected:

	// コンポーネントとインスタンスを格納するコンテナ
	std::map<COMP_TYPE, std::unique_ptr<ComponentBase>> components_;

	// 有効判定
	bool isActive_;

	// 更新処理
	void UpdateComponent(void);

	void DrawComponent(void);

private:


};
