#pragma once
#include "ObjectType.h"

#define GRAVITY (9.8f * 0.001f)

class ObjectManager {
	friend class ObjectBase;
private:
	inline static std::list<std::unique_ptr<ObjectBase>> m_list;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObjectManager() = default;
	/// <summary>
	/// objのtypeに応じてlistに追加
	/// </summary>
	/// <param name="obj"></param>
	void Add(ObjectBase* obj);
public:
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static ObjectManager* GetInstance();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ObjectManager() = default;
	/// <summary>
	/// 全オブジェクトの削除チェック
	/// </summary>
	static void CheckKillAll();
	/// <summary>
	/// 全オブジェクトの更新
	/// </summary>
	static void UpdateAll();
	/// <summary>
	/// 全オブジェクトの3D描画
	/// </summary>
	static void RenderAll();
	/// <summary>
	/// 全オブジェクトの2D描画
	/// </summary>
	static void DrawAll();
	/// <summary>
	/// 全オブジェクトの当たり判定チェック
	/// </summary>
	static void CollisionAll();

	/// <summary>
	/// 全オブジェクト削除
	/// </summary>
	void KillAll();
	/// <summary>
	/// 指定したtypeのオブジェクトを削除
	/// </summary>
	/// <param name="mask">1 << type</param>
	void Kill(unsigned int mask);

	/// <summary>
	/// 指定したオブジェクトの探索
	/// </summary>
	/// <typeparam name="T">探索対象のクラス名</typeparam>
	/// <param name="type">探索対象の種類</param>
	/// <returns>探索対象のポインタ</returns>
	template <typename T>
	static T* FindObject(int type)
	{
		for (auto& obj : m_list) {
			if (obj->GetType() == type && !obj->m_kill)
				return dynamic_cast<T*>(obj.get());
		}
		return nullptr;
	}
	/// <summary>
	/// 指定したオブジェクトの探索(複数)
	/// </summary>
	/// <typeparam name="T">探索対象のクラス名</typeparam>
	/// <param name="type">探索対象の種類</param>
	/// <returns>探索対象のリスト(vector)</returns>
	template <typename T>
	static std::vector<T*> FindObjects(int type)
	{
		std::vector<T*> list;

		for (auto& obj : m_list) {
			if (obj->GetType() == type && !obj->m_kill)
				list.push_back(dynamic_cast<T*>(obj.get()));
		}
		return list;
	}
};
