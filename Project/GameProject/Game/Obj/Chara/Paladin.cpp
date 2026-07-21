#include "Paladin.h"
#include "Game/Obj/Effect/EffectMagic.h"
#include "Game/Obj/Room.h"
#include "Game/Obj/Weapon/Shield.h"
#include "Game/Obj/Weapon/Sword.h"
#include "Player.h"

namespace {
	constexpr int PALADIN_HP = 50;
	constexpr int ATTACK_DAMAGE = 10;
	constexpr int SKILLATTACK_DAMAGE = 20;
	constexpr int ATTACK_CT = 120;
	constexpr int CHASE_TIME = 180;
	constexpr float MOVE_SPEED = 0.02f;
	constexpr float PALADIN_HEIGHT = 3.46f;
	const float FOV_ANGLE = DtoR(45.0f);		//視野角度
	constexpr float FOV_LENGTH = 10.0f;			//視野範囲
	constexpr float ATTACK_LENGTH = 2.2f;		//攻撃範囲
}

Paladin::Paladin(const CVector3D& pos, bool servant)
	: CharaBase(eEnemy)
	, m_attackCT(0)
	, m_attackCnt(0)
	, m_chaseTime(0)
	, m_moveTime(0.0f)
	, m_backRotX(0.0f)
	, m_backRotZ(0.0f)
	, m_skillFlag(false)
	, mp_target(nullptr)
	, mp_effect(nullptr){
	m_model = COPY_RESOURCE("Paladin", CModelA3M);
	m_pos = pos;
	m_state = SIdle;
	m_hp = PALADIN_HP;
	m_rad = 0.2f;
	m_rot.y = DtoR(Utility::NormalizeAngle(Base::GetRand(0.0f, 360.0f)));
	m_dir = CVector3D(sin(m_rot.y), 0, cos(m_rot.y));
	mp_shield = new Shield(this);
	Base::Add(mp_shield);
	mp_sword = new Sword(this);
	Base::Add(mp_sword);
	if (servant) {
		mp_effect = new EffectMagic("ECircleR", m_pos, CVector3D(DtoR(90), 0, 0), 4.5f, -1);
		Base::Add(mp_effect);
	}
}

Paladin::~Paladin(){
	if (mp_shield != nullptr) mp_shield->SetKill();
	if (mp_sword != nullptr) mp_sword->SetKill();
	if (mp_effect != nullptr) mp_effect->SetKill();
}

void Paladin::Update(){
	CharaBase::Update();
	//状態遷移
	switch (m_state) {
	case SIdle: StateIdle(); break;
	case SAttack: StateAttack(); break;
	case SSkillAttack: StateSkillAttack(); break;
	case SDamage: StateDamage(); break;
	case SDeath: StateDeath(); break;
	}
	m_model.UpdateAnimation();
	m_capusle = CCapsule(m_pos + CVector3D(0, PALADIN_HEIGHT - m_rad, 0), m_pos + CVector3D(0, m_rad, 0), m_rad);
	
	if (mp_effect != nullptr) mp_effect->m_pos = m_pos + CVector3D(0, 0.25f, 0);
	//移動時間が設定されたら減少させる
	(m_moveTime > 0.0f) ? m_moveTime -= 0.1f : m_moveTime = 0.0f;
	//クールタイム減少
	if (m_attackCT != 0) m_attackCT--;
	//攻撃カウントが2以上ならスキルフラグを立てる
	m_skillFlag = (m_attackCnt >= 2) ? true : false;
	//落下防止
	if (m_pos.y < -0.3f) m_pos.y = 0;

	//追跡中は視野範囲に関係なくプレイヤーを追う
	if (m_chaseTime != 0) {
		m_chaseTime--;
		if (Base::FindObject(ePlayer)) mp_target = Base::FindObject(ePlayer);
	}
}

void Paladin::Render(){
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(0.02f, 0.02f, 0.02f);
	m_model.Render();
	//Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 0.5f));

	//視野範囲
	/*CVector4D color(0.0f, 1.0f, 0.0f, 0.75f);
	if(mp_target) color = CVector4D(1.0f, 1.0f, 0.0f, color.a);
	auto lineWidth = 5.0f;
	Utility::DrawLine(m_pos, m_pos + m_dir * FOV_LENGTH, CVector4D(1, 0, 1, 1), lineWidth);
	Utility::DrawLine(m_pos, m_pos + CMatrix::MRotationY(FOV_ANGLE) * m_dir * FOV_LENGTH, CVector4D(0, 1, 1, 1), lineWidth);
	Utility::DrawLine(m_pos, m_pos + CMatrix::MRotationY(-FOV_ANGLE) * m_dir * FOV_LENGTH, CVector4D(0, 1, 1, 1), lineWidth);
	CMatrix m;
	m.LookAt(m_pos, m_pos + m_dir * FOV_LENGTH, CVector3D(0, 1, 0));
	Utility::DrawSector(m, -FOV_ANGLE, FOV_ANGLE, FOV_LENGTH, color);*/
}

void Paladin::Collision(Base* b) {
	switch (b->GetType()) {
	case eRoom: {
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
	case ePlayer:
		if (Player* p = dynamic_cast<Player*>(b)) {
			float dot(CVector3D::Dot(m_dir, (p->m_pos - m_pos).GetNormalize()));
			//視野範囲内にプレイヤーがいたら
			if (dot > cos(FOV_ANGLE) &&
				(p->m_pos - m_pos).Length() < FOV_LENGTH) {
				//ターゲットに設定
				mp_target = b;
			}
			else if (m_chaseTime == 0) mp_target = nullptr;
		}
		break;
	case eChest:
	case eDoor:
	case ePotion: {
		float length;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(b->m_obb, m_capusle, &axis, &length)) {
			m_pos += axis * (m_rad - length);
		}
	}
			break;
	}
}

void Paladin::StateIdle() {
	m_model.SetAnimationSpeed(1.0f);
	//移動
	//クールタイムが0かつターゲットを発見したら
	if (m_attackCT <= 0 && mp_target) {
		m_backRotX = 0;
		m_backRotZ = 0;
		CVector3D vec(mp_target->m_pos - m_pos);
		m_rot.y = atan2(vec.x, vec.z);
		m_dir = CVector3D(sin(m_rot.y), 0, cos(m_rot.y));
		//ターゲットへ移動
		if (vec.Length() > ATTACK_LENGTH) {
			m_pos += m_dir * (MOVE_SPEED * 2);
			m_model.SetAnimationSpeed(2.0f);
			m_model.ChangeAnimation(Walk);
		}
		//攻撃範囲内まで近づいたら攻撃開始
		//既に攻撃が2回当たっていたらスキル攻撃
		else m_state = (m_attackCnt == 2) ? SSkillAttack : SAttack;
	}
	//クールタイム中なら
	else if (m_attackCT != 0 && mp_target) {
		//ランダムに少し角度をつける
		if (m_backRotX == 0) m_backRotX = Base::GetRand(-5.0f, 5.0f);
		if (m_backRotZ == 0) m_backRotZ = Base::GetRand(-5.0f, 5.0f);
		CVector3D vec(mp_target->m_pos - m_pos);
		m_rot.y = atan2(vec.x, vec.z);
		m_dir = (vec + CVector3D(sin(m_backRotX), 0, cos(m_backRotZ))).GetNormalize();
		//後退する
		m_pos -= m_dir * MOVE_SPEED;
		m_model.ChangeAnimation(WalkBack);
	}
	//ターゲットが未発見なら
	else {
		//移動中でなければ待機アニメーション
		if (m_moveTime == 0.0f && (int)m_model.GetAnimation() != Idle) {
			m_model.ChangeAnimation(Idle, false);
			//アニメーション終了で移動経路を再設定
			if (m_model.isAnimationEnd()) {
				WanderMove();
			}
		}
		//移動
		if (m_moveTime > 0.0f) {
			m_pos += m_dir * MOVE_SPEED;
			m_model.ChangeAnimation(Walk);
		}
	}




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

}

void Paladin::StateAttack(){
	m_model.ChangeAnimation(Attack, false);
	if (Sword* s = dynamic_cast<Sword*>(mp_sword)) {
		switch (m_stateStep) {
		case 0:
			if (m_model.GetAnimationFrame() >= 17) {
				s->SetDamage(ATTACK_DAMAGE);
				s->SetCapsule(true);
				m_stateStep++;
			}
			break;
		case 1:
			if (m_model.GetAnimationFrame() >= 23) {
				s->SetCapsule(false);
				m_stateStep++;
			}
			break;
		}
		m_attackCT = ATTACK_CT;
		if (m_model.isAnimationEnd()) NextState(SIdle);
	}
}

void Paladin::StateSkillAttack(){
	m_model.ChangeAnimation(SkillAttack, false);
	if (Sword* s = dynamic_cast<Sword*>(mp_sword)) {
		switch (m_stateStep) {
		case 0:
			if (m_model.GetAnimationFrame() >= 15) {
				s->SetDamage(SKILLATTACK_DAMAGE);
				s->SetCapsule(true);
				m_stateStep++;
			}
			break;
		case 1:
			if (m_model.GetAnimationFrame() >= 22) {
				s->SetCapsule(false);
				m_attackCnt -= 2;
				m_stateStep++;
			}
			break;
		}
		m_attackCT = ATTACK_CT;
		if (m_model.isAnimationEnd()) NextState(SIdle);
	}
}

void Paladin::StateDamage(){
	//追跡中はリアクションしない
	if (m_chaseTime > 0) NextState(SIdle);
	m_model.SetAnimationSpeed(1.0f);
	m_model.ChangeAnimation(Damage, false);
	//ダメージを受けたら追跡時間を設定
	m_chaseTime = CHASE_TIME;
	if (m_model.isAnimationEnd()) NextState(SIdle);
}

void Paladin::StateDeath(){
	m_model.SetAnimationSpeed(1.0f);
	m_model.ChangeAnimation(Death, false);
	if (m_model.isAnimationEnd()) SetKill();
}

void Paladin::WanderMove(){
	if (Room* r = dynamic_cast<Room*>(Base::FindObject(eRoom))) {
		bool b = false;
		do {
			m_rot.y = DtoR(Utility::NormalizeAngle(Base::GetRand(0.0f, 360.0f)));
			m_dir = CVector3D(sin(m_rot.y), 0, cos(m_rot.y));
			m_moveTime = Base::GetRand(10.0f, 15.0f);
			CVector3D afterPos = m_pos + m_dir.GetNormalize() * MOVE_SPEED * m_moveTime, c, n;
			b = (r->GetModel()->CollisionRay(&c, &n,
				m_pos, afterPos))
				? true : false;
			//移動経路がフィールドと接触したら再設定
		} while (b);
	}
}

void Paladin::TakeDamage(int damage){
	if (m_hp == 0) return;
	if (m_hp - damage > 0) {
		m_hp -= damage;
		if (m_state != SSkillAttack) NextState(SDamage);
	}
	else {
		m_hp = 0;
		NextState(SDeath);
	}
}
