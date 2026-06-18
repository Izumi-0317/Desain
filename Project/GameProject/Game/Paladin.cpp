#include "Paladin.h"
#include "Field.h"
#include "Shield.h"
#include "Sword.h"

namespace {
	constexpr float MOVE_SPEED = 0.02f;
	constexpr float PALADIN_HEIGHT = 1.73f;
}

Paladin::Paladin(const CVector3D& pos)
	: CharaBase(eEnemy)
	, m_moveTime(0.0f){
	m_model = COPY_RESOURCE("Paladin", CModelA3M);
	m_pos = pos;
	m_state = SIdle;
	m_hp = 100;
	m_rad = 0.2f;
	Base::Add(new Shield());
	Base::Add(new Sword());
}

void Paladin::Update(){
	CharaBase::Update();
	m_model.UpdateAnimation();
	m_capusle = CCapsule(m_pos + CVector3D(0, PALADIN_HEIGHT - m_rad, 0), m_pos + CVector3D(0, m_rad, 0), m_rad);
	(m_moveTime > 0.0f) ? m_moveTime -= 0.1f : m_moveTime = 0.0f;
	if (m_model.isAnimationEnd()) m_pos.y += -0.001f;
}

void Paladin::Render(){
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot + CVector3D(0, DtoR(90), 0));
	m_model.SetScale(0.01f, 0.01f, 0.01f);
	m_model.Render();
	//Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 0.5f));
}

void Paladin::Collision(Base* b){
	switch (b->GetType()) {
	case eField: {
		CVector3D v(0, 0, 0);
		auto tri = b->GetModel()->CollisionCapsule(m_capusle);
		for (auto& t : tri) {
			float max_y = max(t.m_vertex[0].y, max(t.m_vertex[1].y, t.m_vertex[2].y));
			if (t.m_normal.y < -0.5f) {
				if (m_vec.y > 0) m_vec.y = 0;
			}
			else if (t.m_normal.y > 0.8f) {
				if (m_vec.y < 0) m_vec.y = 0;
			}
			CVector3D nv = t.m_normal * (m_rad - t.m_dist);
			v.y = fabs(v.y) > fabs(nv.y) ? v.y : nv.y;
			if (max_y > m_pos.y + 0.2f) {
				v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
				v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
			}
		}
		m_pos += v;
	}
			   break;
	}
}

void Paladin::StateIdle() {
	/*if (m_moveTime == 0.0f) m_model.ChangeAnimation(Idle, false);
	else {
		m_pos += m_dir * MOVE_SPEED;
		m_model.ChangeAnimation(Walk);
	}
	if (m_model.isAnimationEnd()) {
		m_rot.y = Base::GetRand(0.0f, 360.0f);
		m_dir = CVector3D(cos(m_rot.y), 0, sin(m_rot.y));
		m_moveTime = Base::GetRand(3.0f, 5.0f);
	}*/
	//移動
	if (m_moveTime > 0.0f) {
		m_pos += m_dir * MOVE_SPEED;
		m_model.ChangeAnimation(Walk);
	}
	//移動中でなければ待機アニメーション
	if (m_moveTime == 0.0f && (int)m_model.GetAnimation() != Idle) {
		m_model.ChangeAnimation(Idle, false);
		//アニメーション終了で再設定
		if (m_model.isAnimationEnd()) {
			if (Field* f = dynamic_cast<Field*>(Base::FindObject(eField))) {
				bool b = false;
				do {
					m_rot.y = DtoR(Utility::NormalizeAngle(Base::GetRand(0.0f, 360.0f)));
					m_dir = CVector3D(cos(m_rot.y), 0, sin(m_rot.y));
					m_moveTime = Base::GetRand(5.0f, 10.0f);
					CVector3D afterPos = m_pos + (m_dir.GetNormalize() * MOVE_SPEED) * m_moveTime, c, n;
					b = (f->GetModel()->CollisionRay(&c, &n,
						m_pos, afterPos))
						? true : false;
					//行き先までがフィールドと接触したら再設定
				} while (b);
			}
		}
	}
}

void Paladin::StateAttack(){
	m_model.ChangeAnimation(Attack, false);

}

void Paladin::StateDamage(){
	m_model.ChangeAnimation(Damage, false);

}

void Paladin::StateDeath(){
	m_model.ChangeAnimation(Death, false);

}
