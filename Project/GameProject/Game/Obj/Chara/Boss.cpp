#include "Boss.h"
#include "Player.h"

namespace {
	constexpr int PUNCH_CT = 120;				//殴打のクールタイム
	constexpr int HAYMAKER_CT = 600;			//猛撃のクールタイム
	constexpr int JUMPATTACK_CT = 1800;			//跳躍攻撃のクールタイム
	constexpr float BOSS_HEIGHT = 3.5f;
	constexpr float MOVE_SPEED = 0.03f;
	constexpr float ATTACK_LENGTH = 2.2f;		//攻撃範囲
}

Boss::Boss(const CVector3D& pos)
	: CharaBase(eBoss)
	, m_attackCT(0, HAYMAKER_CT, JUMPATTACK_CT)
	, m_isRoar(false)
	, m_isAttack(true, false, false)
	, mp_target(nullptr) {
	m_model = COPY_RESOURCE("Boss", CModelA3M);
	m_pos = pos;
	m_hp = 500;
	m_rad = 1.0f;
	m_state = SIdle;
}

void Boss::Update(){
	CharaBase::Update();
	switch (m_state){
	case SIdle: StateIdle(); break;
	case SAttack: StateAttack(); break;
	case SHaymaker: StateHaymaker(); break;
	case SJumpAttack: StateJumpAttack(); break;
	case SShowOff: StateShowOff(); break;
	case SDeath: StateDeath(); break;
	}
	m_capusle = CCapsule(m_pos + CVector3D(0, BOSS_HEIGHT - m_rad, 0), m_pos + CVector3D(0, m_rad, 0), m_rad);
	m_model.UpdateAnimation();
	//攻撃クールタイム減少　0なら攻撃できる
	for (int i = 0;i < std::size(m_attackCT);i++) {
		if (m_attackCT[i] > 0) m_attackCT[i]--;
		else m_isAttack[i] = true;
	}
}

void Boss::Render(){
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(0.02f, 0.02f, 0.02f);
	m_model.Render();
	//Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 0.5));
}

void Boss::Collision(Base* b){
	switch (b->GetType()){
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
			//TODO::AABBで部屋に入ったら追跡
			mp_target = p;
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

void Boss::StateIdle(){
	if (mp_target == nullptr) {
		m_model.ChangeAnimation(Idle);
	}
	else {
		//最初だけ咆哮する
		if (!m_isRoar) {
			m_model.ChangeAnimation(Roaring, false);
			if (m_model.isAnimationEnd()) m_isRoar = true;
		}
		else {
			CVector3D vec(mp_target->m_pos - m_pos);
			m_rot.y = atan2(vec.x, vec.z);
			m_dir = CVector3D(sin(m_rot.y), 0, cos(m_rot.y));
			//ターゲットへ移動
			if (vec.Length() > ATTACK_LENGTH) {
				m_pos += m_dir * (MOVE_SPEED * 2);
				m_model.ChangeAnimation(Run);
			}
			else {
				if (m_isAttack[2]) NextState(SJumpAttack);
				else if (m_isAttack[1]) NextState(SHaymaker);
				else if (m_isAttack[0]) NextState(SAttack);
				else;//TODO::ターゲットの背後に回る
			}
		}
	}
}

void Boss::StateAttack(){
	m_model.ChangeAnimation(Punch, false);
	switch (m_stateStep){
	case 0:
		if (m_model.GetAnimationFrame() >= 8) {
			//球生成
			m_stateStep++;
		}
		break;
	case 1:
		if (m_model.GetAnimationFrame() >= 13) {
			//球削除
			m_stateStep++;
		}
		break;
	}
	m_attackCT[0] = PUNCH_CT;
	m_isAttack[0] = false;
	//TODO::当たったらSHowoff
	if (m_model.isAnimationEnd()) NextState(SShowOff);
}

void Boss::StateHaymaker(){
	m_model.ChangeAnimation(Haymaker, false);
	switch (m_stateStep) {
	case 0:
		if (m_model.GetAnimationFrame() >= 39) {
			//球生成
			m_stateStep++;
		}
		break;
	case 1:
		if (m_model.GetAnimationFrame() >= 44) {
			//球削除
			m_stateStep++;
		}
		break;
	}
	m_attackCT[1] = HAYMAKER_CT;
	m_isAttack[1] = false;
	//TODO::当たったらSHowoff
	if (m_model.isAnimationEnd()) NextState(SShowOff);
}

void Boss::StateJumpAttack(){
	m_model.ChangeAnimation(JumpAttack, false);
	switch (m_stateStep) {
	case 0:
		//TODO::後ろに下がる
		break;
	case 1:
		if (m_model.GetAnimationFrame() >= 52) {
			//一瞬だけ巨大な球生成　プレイヤーを吹っ飛ばす
			m_stateStep++;
		}
		break;
	case 2:
		if (m_model.GetAnimationFrame() >= 53) {
			//球削除
			m_stateStep++;
		}
		break;
	}
	m_attackCT[2] = JUMPATTACK_CT;
	m_isAttack[2] = false;
	//TODO::当たったらSHowoff
	if (m_model.isAnimationEnd()) NextState(SShowOff);
}

void Boss::StateShowOff(){
	m_model.ChangeAnimation(ShowOff, false);
	if (m_model.isAnimationEnd()) NextState(SIdle);
}

void Boss::StateDeath(){
	m_model.ChangeAnimation(Death, false);
	if (m_model.isAnimationEnd()) SetKill();//TODO::ゲームクリアのフラグを立てる
}

void Boss::TakeDamage(int damage){
	if (m_hp == 0) return;
	if (m_hp - damage > 0) {
		m_hp -= damage;
	}
	else {
		m_hp = 0;
		NextState(SDeath);
	}
}
