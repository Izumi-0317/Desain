#include "Bullet.h"
#include "Game/Obj/Chara/Paladin.h"
#include "Game/Obj/Effect/EffectParticle.h"
#include "Game/Obj/Weapon/Gun.h"

namespace {
	constexpr int BULLET_DAMAGE = 10;
	constexpr float BULLET_SPEED = 0.5f;
}

Bullet::Bullet(const CVector3D& pos, const CVector3D& dir)
	: Base(eBullet)
	, m_killCnt(0)
	, m_isCol(false)
	, m_posOld(pos)
	, m_effectPos(pos) 
	, m_effectVec(1, 1, 1){
	m_bullet = COPY_RESOURCE("Bullet", CModelObj);
	m_pos = pos;
	m_dir = dir;
	m_rad = 0.01f;
}

Bullet::~Bullet() {
	//着弾したら着弾エフェクトを生成
	if (m_isCol) {
		for (int i = 0; i < 20; i++) {
			const float speed = 0.02f;
			CVector3D vec(Utility::Rand(-speed, speed), Utility::Rand(-speed / 2, speed), Utility::Rand(-speed, speed));
			Base::Add(new EffectParticle("EBomb", m_effectPos, vec * m_effectVec, CVector3D(0, -0.001f, 0), 0.2, -0.005f, -0.01f, EffectParticle::eBlendAdd, true));
		}
	}
}

void Bullet::Update() {
	if (Gun* g = dynamic_cast<Gun*>(Base::FindObject(eGun))) {
		m_bulletMat = CMatrix::MTranselate(m_pos)
			* CMatrix::MRotation(g->GetMatrix().GetEuler(CMatrix::eZYX));
	}
	m_pos += m_dir * BULLET_SPEED;

	m_capusle = CCapsule(m_posOld, m_pos, m_rad);
	m_posOld = m_pos;
	if (m_killCnt++ >= 120) SetKill();
}

void Bullet::Render() {
	m_bullet.Render(m_bulletMat);
	//Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 0.5));
}

void Bullet::Collision(Base* b) {
	switch (b->GetType()) {
	case eRoom: {
		auto tri = b->GetModel()->CollisionCapsule(m_capusle);
		if (!tri.empty()) {
			/*m_effectPos = tri[0].m_cross;
			CVector3D v(m_pos - tri[0].m_cross);
			m_effectPos += v * 0.05f;*/
			m_effectPos = m_pos;
			m_effectVec.y = tri[0].m_normal.y;
			SetKill();
			m_isCol = true;
		}
		break;
	}
	case eEnemy: {
		float dist;
		CVector3D cross, dir;
		if (CCollision::CollisionCapsule(m_capusle, b->m_capusle, &dist, &cross, &dir)) {
			if (Paladin* p = dynamic_cast<Paladin*>(b)) {
				p->TakeDamage(BULLET_DAMAGE);
				SetKill();
				m_isCol = true;
				m_effectPos = m_posOld;
			}
		}
		break;
	}
	case eChest:
	case eDoor:
	case eShield:
		if (CCollision::CollisionOBBCapsule(b->m_obb, m_capusle)) {
			SetKill();
			m_isCol = true;
			m_effectPos = m_posOld;
		}
		break;
	}
}
