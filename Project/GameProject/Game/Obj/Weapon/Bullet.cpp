#include "Bullet.h"
#include "Game/Obj/Chara/Paladin.h"

namespace {
	constexpr int BULLET_DAMAGE = 10;
	constexpr float BULLET_SPEED = 0.5f;
}

Bullet::Bullet(const CVector3D& pos, const CVector3D& dir)
	: Base(eBullet)
	, m_killCnt(0)
	, m_posOld(pos){
	//m_bullet = COPY_RESOURCE("Bullet", CModelObj);
	m_pos = pos;
	m_dir = dir;
	m_rad = 0.01f;
}

void Bullet::Update(){
	m_pos += m_dir * BULLET_SPEED;
	m_capusle = CCapsule(m_posOld, m_pos, m_rad);
	m_posOld = m_pos;
	if (m_killCnt++ >= 60) SetKill();
}

void Bullet::Render(){
	Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 1));
}

void Bullet::Collision(Base* b){
	switch (b->GetType()){
	case eField: {
		CVector3D cross, normal;
		if (b->GetModel()->CollisionRay(&cross, &normal, m_posOld, m_pos)) {
			SetKill();
		}
		break;
	}
	case eEnemy:
		float dist;
		CVector3D cross, dir;
		if (CCollision::CollisionCapsule(m_capusle, b->m_capusle, &dist, &cross, &dir)) {
			if (Paladin* p = dynamic_cast<Paladin*>(b)) {
				p->TakeDamage(BULLET_DAMAGE);
				SetKill();
			}
		}
		break;
	}
}
