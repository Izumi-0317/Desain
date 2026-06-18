#include "Bullet.h"

namespace {
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
	Utility::DrawCapsule(m_capusle, CVector4D(0, 0, 0, 1));
}

void Bullet::Collision(Base* b){
	switch (b->GetType()){
	case eField:
		CVector3D c, n;
		if (b->GetModel()->CollisionRay(&c, &n, m_posOld, m_pos)) {
			SetKill();
		}
		break;
	}
}
