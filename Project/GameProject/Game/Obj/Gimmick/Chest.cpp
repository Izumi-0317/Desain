#include "Chest.h"
#include "Effekseer/EffekseerEffect.h"
#include "Game/Obj/Chara/Paladin.h"
#include "Game/Obj/Gimmick/Potion.h"

Chest::Chest(const CVector3D& pos, float roty, int enemyCnt)
	: GimmickBase(eChest)
	, m_isOpen(true){
	m_gimmick = COPY_RESOURCE("ChestClose", CModelObj);
	m_pos = pos;
	m_rot.y = DtoR(roty);
	m_scale = CVector3D(3, 3, 3);
	for (int i = 0;i < enemyCnt;i++) {
		float randAng = DtoR(Base::GetRand(0.1, 360.0f));
		float dist = std::sqrt(Base::GetRand(1.0f, 4.0f));
		Base* ptr = new Paladin(m_pos + CVector3D(cos(randAng) * dist, 0, sin(randAng) * dist));
		Base::Add(ptr);
		m_enemyList.push_back(ptr);
	}
}

void Chest::Update(){
	//インタラクト可能かつ開けられるかつ守っている敵がいなければ
	if (m_isInteractable && m_isOpen && m_enemyList.empty()) {
		m_gimmick = COPY_RESOURCE("ChestOpen", CModelObj);
		Base::Add(new Potion(m_pos));
		m_isOpen = false;
	}

	if (!m_enemyList.empty()) {
		auto itr = m_enemyList.begin();
		while (itr != m_enemyList.end()) {
			if ((*itr)->m_kill) {
				itr = m_enemyList.erase(itr);
				continue;
			}
			else {
				//敵と宝箱を繋ぐエフェクト
			}
			itr++;
		}
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
