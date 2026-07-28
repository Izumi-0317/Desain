#include "Boss.h"
#include "Game/Game.h"
#include "Game/Obj/Room.h"
#include "Player.h"

namespace {
	constexpr int BOSS_HP = 500;
	constexpr int PUNCH_DMG = 10;				//殴打のダメージ
	constexpr int HAYMAKER_DMG = 20;			//猛撃のダメージ
	constexpr int JUMPATK_DMG = 50;				//跳躍攻撃のダメージ
	constexpr int PUNCH_CT = 120;				//殴打のクールタイム
	constexpr int HAYMAKER_CT = 600;			//猛撃のクールタイム
	constexpr int JUMPATK_CT = 1800;			//跳躍攻撃のクールタイム
	constexpr float PUNCH_RAD = 2.0f;			//殴打の半径
	constexpr float HAYMAKER_RAD = 3.0f;		//猛撃の半径
	constexpr float JUMPATK_RAD = 5.5f;			//跳躍攻撃の半径
	constexpr float BOSS_HEIGHT = 5.0f;
	constexpr float MOVE_SPEED = 0.02f;
	constexpr float ATK_LENGTH = 3.5f;			//攻撃範囲
}

Boss::Boss(const CVector3D& pos)
	: CharaBase(eBoss)
	, m_atkCT(0, HAYMAKER_CT, JUMPATK_CT)
	, m_atkDmg(0)
	, m_atkRad(0.0f)
	, m_isRoar(false)
	, m_isAtk(true, false, false)
	, m_isHit(false)
	, m_spawnPos(pos)
	, m_atkPos(pos)
	, m_jumpPos(CVector3D::zero)
	, m_landingPos(CVector3D::zero)
	, mp_target(nullptr) {
	m_model = COPY_RESOURCE("Boss", CModelA3M);
	m_pos = pos;
	m_hp = BOSS_HP;
	m_rad = 2.0f;
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
	for (int i = 0;i < std::size(m_atkCT);i++) {
		if (m_atkCT[i] > 0) m_atkCT[i]--;
		else m_isAtk[i] = true;
	}
}

void Boss::Render(){
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(0.03f, 0.03f, 0.03f);
	m_model.Render();
	//Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 0.5));
	//Utility::DrawSphere(m_pos + m_dir * 2, PUNCH_RAD, CVector4D(1, 0, 0, 0.5));
	//Utility::DrawSphere(m_pos + m_dir * 3, HAYMAKER_RAD, CVector4D(0, 1, 0, 0.5));
	//Utility::DrawSphere(m_pos + m_dir * 2.5f, JUMPATK_RAD, CVector4D(0, 0, 1, 0.5));
}

void Boss::Collision(Base* b) {
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
		//部屋にプレイヤーが入ったら追跡
		if (Room* r = dynamic_cast<Room*>(b)) {
			if (r->IsCollision()) {
				mp_target = Base::FindObject(ePlayer);
			}
			else mp_target = nullptr;
		}
		break;
	}
	case ePlayer:
		if (Player* p = dynamic_cast<Player*>(b)) {
			float dist;
			CVector3D cross, dir;

			//半径が0ならスルー
			if (m_atkRad == 0) return;
			if (!m_isHit && CCollision::CollisionCapsuleShpere(p->m_capusle, m_atkPos, m_atkRad, &dist, &cross, &dir)) {
				p->TakeDamage(m_atkDmg);
				m_isHit = true;
				if (m_state == SJumpAttack) {
					if (Player* p = dynamic_cast<Player*>(mp_target)) {
						float force = (float)JUMPATK_DMG / 30;
						p->AddForce(dir, -force);
					}
				}
			}
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
		break;
	}
	}
}

void Boss::StateIdle(){
	if (mp_target == nullptr) {
		//初期位置から離れていたら戻る
		if (abs(m_spawnPos.x) - abs(m_pos.x) >= 0.1f ||
			abs(m_spawnPos.z) - abs(m_pos.z) >= 0.1f) {
			m_pos = m_spawnPos;
		}
		else {
			m_model.ChangeAnimation(Idle);
			ResetStatus();
		}
	}
	else {
		//最初だけ咆哮する
		if (!m_isRoar) {
			m_model.ChangeAnimation(Roaring, false);
			if (m_model.isAnimationEnd()) m_isRoar = true;
		}
		else {
			m_isHit = false;
			CVector3D vec(mp_target->m_pos - m_pos);
			m_rot.y = atan2(vec.x, vec.z);
			m_dir = CVector3D(sin(m_rot.y), 0, cos(m_rot.y));

			if (m_isAtk[2]) NextState(SJumpAttack);
			//ターゲットへ移動
			else if (vec.Length() > ATK_LENGTH) {
				m_pos += m_dir * (MOVE_SPEED * 2);
				m_model.ChangeAnimation(Run);
			}
			else {
				if (m_isAtk[1]) NextState(SHaymaker);
				else if (m_isAtk[0]) NextState(SAttack);
				else m_model.ChangeAnimation(Run);//TODO::ターゲットの背後に回る
			}
		}
	}
}

void Boss::StateAttack(){
	m_model.SetAnimationSpeed(0.5f);
	m_model.ChangeAnimation(Punch, false);
	switch (m_stateStep){
	case 0:
		if (m_model.GetAnimationFrame() >= 8) {
			//球生成
			SetAtkShpere(m_pos + m_dir * 2, PUNCH_DMG, PUNCH_RAD);
			m_stateStep++;
		}
		break;
	case 1:
		if (m_model.GetAnimationFrame() >= 13) {
			//球削除
			ClearAtkShpere();
			m_stateStep++;
		}
		break;
	case 2:
		m_model.SetAnimationSpeed(1.0f);
		break;
	}
	m_atkCT[0] = PUNCH_CT;
	m_isAtk[0] = false;
	//攻撃が当たったら隙を作る
	if (m_model.isAnimationEnd()) (m_isHit) ? NextState(SShowOff) : NextState(SIdle);
}

void Boss::StateHaymaker(){
	m_model.ChangeAnimation(Haymaker, false);
	switch (m_stateStep) {
	case 0:
		if (m_model.GetAnimationFrame() >= 39) {
			SetAtkShpere(m_pos + m_dir * 2.0f, HAYMAKER_DMG, HAYMAKER_RAD);
			m_stateStep++;
		}
		break;
	case 1:
		if (m_model.GetAnimationFrame() >= 44) {
			ClearAtkShpere();
			m_stateStep++;
		}
		break;
	}
	m_atkCT[1] = HAYMAKER_CT;
	m_isAtk[1] = false;
	if (m_model.isAnimationEnd()) (m_isHit) ? NextState(SShowOff) : NextState(SIdle);
}

void Boss::StateJumpAttack(){
	m_model.ChangeAnimation(JumpAttack, false);
	switch (m_stateStep) {
	case 0: {
		//現在の座標とターゲットの座標を設定
		if (m_landingPos == CVector3D::zero) {
			m_jumpPos = m_pos;
			m_landingPos = mp_target->m_pos;
		}
		float t = m_model.GetAnimationFrame() / 52;
		if (m_model.GetAnimationFrame() < 8) t = 0.0f;
		if (t > 1.0f) t = 1.0f;
		//設定された地点へ向かう
		m_pos = CVector3D::Leap(m_jumpPos, m_landingPos, t);
		if (m_model.GetAnimationFrame() >= 52) {
			SetAtkShpere(m_pos + m_dir * 2.5f, JUMPATK_DMG, JUMPATK_RAD);
			m_stateStep++;
		}
		break;
	}
	case 1:
		if (m_model.GetAnimationFrame() >= 54) {
			ClearAtkShpere();
			m_landingPos = CVector3D::zero;
			m_stateStep++;
		}
		break;
	}
	m_atkCT[2] = JUMPATK_CT;
	m_isAtk[2] = false;
	if (m_model.isAnimationEnd()) (m_isHit) ? NextState(SShowOff) : NextState(SIdle);
}

void Boss::StateShowOff(){
	m_model.ChangeAnimation(ShowOff, false);
	if (m_model.isAnimationEnd()) NextState(SIdle);
}

void Boss::StateDeath(){
	m_model.ChangeAnimation(Death, false);
	if (m_model.isAnimationEnd()) 
		if (Game* g = dynamic_cast<Game*>(Base::FindObject(eScene))) {
			g->Game::markAsComplete();
		}
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

void Boss::SetAtkShpere(const CVector3D& pos, int dmg, float rad){
	m_atkPos = pos;
	m_atkPos.y = 1.0f;
	m_atkDmg = dmg;
	m_atkRad = rad;
}

void Boss::ClearAtkShpere(){
	m_atkPos = m_pos;
	m_atkDmg = 0;
	m_atkRad = 0.0f;
}

void Boss::ResetStatus(){
	m_atkCT[0] = 0;
	m_atkCT[1] = HAYMAKER_CT;
	m_atkCT[2] = JUMPATK_CT;
	m_atkDmg = 0;
	m_atkRad = 0.0f;
	m_isRoar = false;
	m_isAtk[0] = true;
	m_isAtk[1] = false;
	m_isAtk[2] = false;
	m_isHit = false;
	m_hp = 500;
	m_rot = CVector3D::zero;
	m_state = SIdle;
}
