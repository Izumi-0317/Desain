#include "Bullet.h"
#include "Base/ObjectManager.h"
#include "Game/Obj/Chara/Boss.h"
#include "Game/Obj/Chara/Paladin.h"
#include "Game/Obj/Effect/EffectParticle.h"
#include "Game/Obj/Weapon/Gun.h"

namespace {
	constexpr int BULLET_DAMAGE = 10;
	constexpr float BULLET_SPEED = 0.5f;
}

Bullet::Bullet(const CVector3D& pos, const CVector3D& dir)
	: WeaponBase(ObjectType::eBullet)
	, m_killCnt(0)
	, m_isCol(false)
	, m_posOld(pos)
	, m_effectPos(pos) 
	, m_effectVec(1, 1, 1){
	m_weapon = COPY_RESOURCE("Bullet", CModelObj);
	m_pos = pos;
	m_dir = dir;
	m_rad = 0.01f;
}

Bullet::~Bullet() {
	//着弾したら着弾エフェクトを生成
	if (m_isCol) {
		for (int i = 0; i < 20; i++) {
			const float speed = 0.02f;
			CVector3D vec(
				Utility::Rand(-speed, speed),
				Utility::Rand(-speed / 2, speed),
				Utility::Rand(-speed, speed)
			);
			new EffectParticle(
				"EBomb",
				m_effectPos,
				vec * m_effectVec,
				CVector3D(0, -0.001f, 0),
				0.2,
				-0.005f,
				-0.01f,
				EffectParticle::eBlendAdd,
				true
			);
		}
	}
}

void Bullet::Update() {
	if (Gun* g = ObjectManager::FindObject<Gun>(ObjectType::eGun)) {
		m_weaponMat = CMatrix::MTranselate(m_pos)
			* CMatrix::MRotation(g->GetMatrix().GetEuler(CMatrix::eZYX));
	}
	m_pos += m_dir * BULLET_SPEED;

	m_capusle = CCapsule(m_posOld, m_pos, m_rad);
	m_posOld = m_pos;
	if (m_killCnt++ >= 120) SetKill();
}

void Bullet::Render() {
	m_weapon.Render(m_weaponMat);
	//Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 0.5));
}

void Bullet::Collision(ObjectBase* b) {
	switch (b->GetType()) {
	case ObjectType::eRoom: {
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
	case ObjectType::eBoss:
	case ObjectType::eEnemy: {
		float dist;
		CVector3D cross, dir;
		if (CCollision::CollisionCapsule(m_capusle, b->m_capusle, &dist, &cross, &dir)) {
			if (Paladin* p = dynamic_cast<Paladin*>(b)) {
				p->TakeDamage(BULLET_DAMAGE);
			}
			if (Boss* boss = dynamic_cast<Boss*>(b)) {
				boss->TakeDamage(BULLET_DAMAGE);
			}
			SetKill();
			m_isCol = true;
			m_effectPos = m_posOld;
		}
		break;
	}
	case ObjectType::eChest:
	case ObjectType::eDoor:
	case ObjectType::eShield:
		if (CCollision::CollisionOBBCapsule(b->m_obb, m_capusle)) {
			SetKill();
			m_isCol = true;
			m_effectPos = m_posOld;
		}
		break;
	}
}
