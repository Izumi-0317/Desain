#include "Potion.h"
#include "Game/Obj/Chara/Player.h"

Potion::Potion(const CVector3D& pos)
	: GimmickBase(ePotion){
	m_gimmick = COPY_RESOURCE("Potion", CModelObj);
	m_pos = pos;
	m_scale = CVector3D(2, 2, 2);
}

void Potion::Update(){
	m_obb = COBB(m_pos, m_rot, CVector3D(1, 1, 1));
}

void Potion::Render(){
	m_gimmick.SetPos(m_pos);
	m_gimmick.SetScale(m_scale);
	m_gimmick.Render();
	//Utility::DrawOBB(m_obb, CVector4D(0, 1, 0, 0.5));
}

void Potion::Interact(){
	if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
		p->SetPotionCnt(1);
	}
	SetKill();
}
