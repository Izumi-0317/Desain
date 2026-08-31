#include "ObjectManager.h"
#include "ObjectBase.h"

ObjectManager* ObjectManager::GetInstance()
{
	static ObjectManager instance;
	return &instance;
}

void ObjectManager::UpdateAll()
{
	for (auto& obj : m_list) {
		if (!obj->m_kill) obj->Update();
	}
}

void ObjectManager::RenderAll()
{
	for (auto& obj : m_list) {
		if (!obj->m_kill) obj->Render();
	}
}

void ObjectManager::DrawAll()
{
	for (auto& obj : m_list) {
		if (!obj->m_kill) obj->Draw();
	}
}

void ObjectManager::CollisionAll()
{
	static std::vector<ObjectBase*> list;
	//初期化　サイズ確保
	list.clear();
	list.reserve(m_list.size());

	for (auto& ptr : m_list) {
		//削除予定のオブジェクトでなければlistに追加
		if (!ptr->m_kill) list.push_back(ptr.get());
	}

	for (auto& obj1 :list) {
		for (auto& obj2 : list) {
			//同じオブジェクトならスルー
			if (obj1 == obj2) continue;
			//当たり判定
			obj1->Collision(obj2);
		}
	}
}

void ObjectManager::CheckKillAll()
{
	auto kill = [](const std::unique_ptr<ObjectBase>& ptr) { return ptr->m_kill; };
	std::erase_if(m_list, kill);
}

void ObjectManager::Add(ObjectBase* obj)
{
	if (obj == nullptr) return;

	int type = obj->GetType();

	//ptrのm_typeがobjのm_typeより大きければ
	//その位置のイテレータを取得
	//なければend()のイテレータを取得
	auto itr = std::find_if(m_list.begin(), m_list.end(),
		[&](const auto& ptr) { return ptr->GetType() > obj->GetType(); });

	m_list.insert(itr, std::unique_ptr<ObjectBase>(obj));
}

void ObjectManager::KillAll()
{
	for (auto& obj : m_list) {
		obj->SetKill();
	}
}

void ObjectManager::Kill(unsigned int mask)
{
	for (auto& b : m_list) {
		if ((1 << (int)b->m_type) & (mask))
			b->SetKill();
	}
}
