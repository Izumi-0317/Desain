#include "Chest.h"
#include "Game/Obj/Chara/Paladin.h"
#include "Game/Obj/Effect/EffectMagic.h"
#include "Potion.h"

Chest::Chest(const CVector3D& pos, float roty, int enemyCnt)
	: GimmickBase(eChest)
	, m_enemyMaxCnt(enemyCnt)
	, m_isOpen(true)
	, mp_effect(nullptr){
	m_gimmick = COPY_RESOURCE("ChestClose", CModelObj);
	m_pos = pos;
	m_rot.y = DtoR(roty);
	m_scale = CVector3D(3, 3, 3);
	if (enemyCnt > 0) {
		//敵生成
		for (int i = 0;i < enemyCnt;i++) {
			float randAng = DtoR(Base::GetRand(0.1, 360.0f));
			float dist = std::sqrt(Base::GetRand(1.0f, 4.0f));
			Base* ptr = new Paladin(m_pos + CVector3D(cos(randAng) * dist, 0, sin(randAng) * dist), true);
			Base::Add(ptr);
			m_enemyList.push_back(ptr);
		}
		//エフェクト生成
		mp_effect = new EffectMagic("ECircleR", m_pos + CVector3D(0, 0.7f, 0), CVector3D(DtoR(90), 0, 0), 6.5f, -1);
		Base::Add(mp_effect);
	}
}

void Chest::Update(){
	//開けられるかつ守っている敵がいなければ
	if (m_isOpen && m_enemyList.empty()) {
		//エフェクト削除
		if (mp_effect != nullptr) {
			mp_effect->SetKill();
			mp_effect = nullptr;
		}
		//インタラクトしたらポーション生成
		if (m_isInteractable) {
			m_gimmick = COPY_RESOURCE("ChestOpen", CModelObj);
			Base::Add(new Potion(m_pos));
			m_isOpen = false;
		}
	}

	if (!m_enemyList.empty()) Remove();

	if (EffectMagic* e = dynamic_cast<EffectMagic*>(mp_effect)) {
		e->SetAlpha(static_cast<float>(m_enemyList.size()) / m_enemyMaxCnt);
	}

	m_obb = COBB(m_pos + CVector3D(0, 0.7f, 0), m_rot, CVector3D(1, 1, 1));
}

void Chest::Render(){
	m_gimmick.SetPos(m_pos);
	m_gimmick.SetRot(m_rot);
	m_gimmick.SetScale(m_scale);
	m_gimmick.Render();
	//Utility::DrawOBB(m_obb, CVector4D(0, 1, 0, 0.5));
}

void Chest::Remove(){
	auto itr = m_enemyList.begin();
	while (itr != m_enemyList.end()) {
		if ((*itr)->m_kill) {
			itr = m_enemyList.erase(itr);
			continue;
		}
		itr++;
	}
}
