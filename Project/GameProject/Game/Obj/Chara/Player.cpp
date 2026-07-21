#include "Player.h"
#include "Game/Camera.h"
#include "Game/Obj/Gimmick/GimmickBase.h"
#include "Game/Game.h"
#include "Game/Obj/Weapon/Bullet.h"
#include "Game/Obj/Weapon/Gun.h"
#include "UI/UIScope.h"

namespace {
	constexpr int PLAYER_HP = 100;
	constexpr int POTION_HEAL_AMOUNT = 30;
	constexpr float PLAYER_HEIGHT = 2.8f;
	constexpr float DEFAULT_MOVE_SPEED = 0.06f;
	constexpr float RUN_SPEED = 2.0f;
	constexpr float HAVE_SPEED = 0.75f;
	constexpr float AIMING_SPEED = 0.25f;
}

Player::Player(const CVector3D& pos)
	: CharaBase(ePlayer)
	, m_fireTime(0)
	, m_potionCnt(0)
	, m_DownToAimFlag(false)
	, m_AimToDownFlag(false)
	, m_attackFlag(false)
	, m_isMaxAmmo(true)
	, m_isDryFiringSound(true)
	, mp_intaractable(nullptr){
	m_model = COPY_RESOURCE("PlayerFPS", CModelA3M);
	m_playerMat = (CMatrix::MTranselate(m_pos) * CMatrix::MRotation(m_rot));
	m_pos = pos;
	m_state = SIdle;
	m_hp = PLAYER_HP;
	m_rad = 0.2f;
	m_model.ChangeAnimation(Idle);
	Base::Add(new Gun("AK47"));
}

void Player::Update(){
	CharaBase::Update();
	if (Camera* f = dynamic_cast<Camera*>(Base::FindObject(eCamera))) {
		m_rot.y = f->m_rot.y;
	}
	switch (m_state){
	case SIdle:	  StateIdle();		 break;
	case SHit:	  StateHit();		 break;
	case SHave:	  StateHave();		 break;
	case SAiming: StateAiming();	 break;
	case SReloaded: StateReloaded(); break;
	case SDamage: StateDamage();	 break;
	case SDeath:  StateDeath();		 break;
	}

	//カプセル
	m_capusle = CCapsule(m_pos + CVector3D(0, PLAYER_HEIGHT - m_rad, 0), m_pos + CVector3D(0, m_rad, 0), m_rad);
	//行列
	m_playerMat = (CMatrix::MTranselate(m_pos) * CMatrix::MRotation(m_rot));
	//アニメーション更新
	m_model.UpdateAnimation();
	//発砲クールタイム
	if (m_fireTime > 0) m_fireTime--;
	//落下防止
	if (m_pos.y < -0.3f) m_pos.y = 0;

	//挙銃のアニメーション遷移
	if (m_DownToAimFlag && (int)m_model.GetAnimation() != DownToAim) {
		m_state = -1;
		m_model.ChangeAnimation(DownToAim, false);
		if (m_model.isAnimationEnd()) {
			m_DownToAimFlag = false;
			NextState(SHave);
		}
	}

	//脱銃のアニメーション遷移
	if (m_AimToDownFlag && (int)m_model.GetAnimation() != AimToDown) {
		m_state = -1;
		m_model.ChangeAnimation(AimToDown, false);
		if (m_model.isAnimationEnd()) {
			m_AimToDownFlag = false;
			NextState(SIdle);
		}
	}

	//リロード
	if ((m_state == SIdle || m_state == SHave) &&
		!m_isMaxAmmo) {
		if (PUSH(CInput::eButton2))
			NextState(SReloaded);
	}

	//デバッグ
	//if (PUSH(CInput::eButton5)) m_vec.y = 5;
	//m_pos += m_vec;
}

void Player::Render(){
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(0.02f, 0.02f, 0.02f);
	m_model.UpdateMatrix();
	if (Camera* f = dynamic_cast<Camera*>(Base::FindObject(eCamera))) {
		f->UpdateCam();
		m_model.BindFrameMatrix(5, CMatrix::MRotation(f->m_rot));
	}
	m_model.Render();
	//Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 0.5f));
	//プレイヤーの更新・描画後に銃を更新
	if (Gun* g = dynamic_cast<Gun*>(Base::FindObject(eGun))) {
		g->UpdateGun();
	}
}

void Player::Collision(Base* b) {
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
			if (max_y > m_pos.y + 0.5f) {
				v.x = fabs(v.x) > fabs(nv.x) ? v.x : nv.x;
				v.z = fabs(v.z) > fabs(nv.z) ? v.z : nv.z;
			}
		}
		m_pos += v;
	}
			  break;
	case eEnemy: {
		float dist;
		CVector3D cross, dir;
		if (CCollision::CollisionCapsule(m_capusle, b->m_capusle, &dist, &cross, &dir)) {
			float s = (m_capusle.GetRadius() + b->m_capusle.GetRadius()) - dist;
			b->m_pos += dir * s * 0.5f;
			m_pos -= dir * s * 0.5f;
		}
		//近接攻撃
		if (m_attackFlag && CCollision::CollisionCapsuleShpere(*b->GetCapsule(),
			m_pos + CVector3D(0, 1, 0) + m_dir.GetNormalize() * 0.5f, 0.2f,
			&dist, &cross, &dir)) {
			if (Interface* i = dynamic_cast<Interface*>(b)) {
				i->TakeDamage(10);
				m_attackFlag = false;
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
			if (GimmickBase* g = dynamic_cast<GimmickBase*>(b)) {
				//インタラクトする対象に設定
				mp_intaractable = g;
			}
		}
		else {
			if (mp_intaractable == nullptr) mp_intaractable = nullptr;
		}
	}
				break;
	}
}

void Player::StateIdle(){
	Move(RUN_SPEED);
	UsePotion();
	Interact();
	//攻撃
	if (PUSH(CInput::eMouseL)) NextState(SHit);
	if (PUSH(CInput::eNum1) || PUSH(CInput::eNum2)) m_DownToAimFlag = true;
}

void Player::StateHit(){
	m_model.SetAnimationSpeed(2.5f);
	m_model.ChangeAnimation(Hit, false);
	switch (m_stateStep){
	case 0:
		if (m_model.GetAnimationFrame() >= 30) {
			m_attackFlag = true;
			Utility::DrawSphere(m_pos + CVector3D(0, 1, 0) + m_dir.GetNormalize(), 0.2f, CVector4D(1, 0, 0, 1));
			SOUND("Hit")->Play();
			m_stateStep++;
		}
		break;
	case 1:
		if (m_model.GetAnimationFrame() >= 38) {
			m_attackFlag = false;
			m_stateStep++;
		}
		break;
	}
	if (m_model.isAnimationEnd()) {
		m_model.SetAnimationSpeed(1.0f);
		NextState(SIdle);
	}
}

void Player::StateHave(){
	if (!PUSH(CInput::eMouseL)) Move(HAVE_SPEED);
	else Fire();
	Interact();

	if (PUSH(CInput::eButton3)) NextState(SAiming);
	if (PUSH(CInput::eNum3)) m_AimToDownFlag = true;
}

void Player::StateAiming(){
	if (!Base::FindObject(eUIScope)) Base::Add(new UIScope());
	if (!HOLD(CInput::eMouseL)) Move(AIMING_SPEED);
	else Fire();

	if (PUSH(CInput::eButton3)) NextState(SHave);
}

void Player::StateReloaded(){
	if (Gun* g = dynamic_cast<Gun*>(Base::FindObject(eGun))) {
		m_model.ChangeAnimation(Reloading, false);
		if (m_model.GetAnimationFrame() >= 50) {
			//リロード
			if (!m_isMaxAmmo) {
				g->Reloaded();
				m_isMaxAmmo = true;
			}
		}
		if (m_model.isAnimationEnd()) NextState(m_undoState);
	}
}

void Player::StateDamage(){
	m_model.SetAnimationSpeed(2.0f);
	m_model.ChangeAnimation(Damage, false);
	if (m_model.isAnimationEnd()) {
		m_model.SetAnimationSpeed(1.0f);
		NextState(m_undoState);
	}
}

void Player::StateDeath(){
	m_model.ChangeAnimation(Death, false);
	if (m_model.isAnimationEnd()); //TODO::GameOver
}

void Player::Move(float speed){
	CVector3D key_dir(0, 0, 0);
	//移動
	if (HOLD(CInput::eUp)) key_dir.z = 1;
	if (HOLD(CInput::eDown)) key_dir.z = -1;
	if (HOLD(CInput::eLeft)) key_dir.x = 1;
	if (HOLD(CInput::eRight)) key_dir.x = -1;
	if (Camera* f = dynamic_cast<Camera*>(Base::FindObject(eCamera))) {
		if (key_dir.LengthSq() > 0.05f) {
			m_dir = (CMatrix::MRotationY(m_rot.y) * key_dir).GetNormalize();
			switch (m_state){
			case SIdle:
				m_model.ChangeAnimation(Walk);
				if (key_dir.z > 0) {
					if (key_dir.LengthSq() < 0.3f || !HOLD(CInput::eButton4))
						m_pos += m_dir * DEFAULT_MOVE_SPEED;
					else
						m_pos += m_dir * (DEFAULT_MOVE_SPEED * speed);
				}
				else m_pos += m_dir * DEFAULT_MOVE_SPEED;
				/*if (key_dir.z > 0) {
				if (key_dir.LengthSq() < 0.3f || !HOLD(CInput::eButton4)) m_model.ChangeAnimation(Walk);
				else {
					m_pos += m_dir * (MOVE_SPEED * 2.0f);
					m_model.ChangeAnimation(Run);
				}
			}
			else if (key_dir.z < 0) m_model.ChangeAnimation(BackWardWalk);
			else if (key_dir.x > 0) m_model.ChangeAnimation(SideStepL);
			else if (key_dir.x < 0) m_model.ChangeAnimation(SideStepR);*/
				break;
			case SHave:
			case SAiming:
				m_pos += m_dir * (DEFAULT_MOVE_SPEED * speed);
				if (key_dir.z > 0) m_model.ChangeAnimation(StrafeF);
				else if (key_dir.z < 0) m_model.ChangeAnimation(StrafeB);
				else if (key_dir.x > 0) m_model.ChangeAnimation(StrafeL);
				else if (key_dir.x < 0) m_model.ChangeAnimation(StrafeR);
				break;
			}
		}
		else {
			switch (m_state){
			case SIdle: m_model.ChangeAnimation(Idle); break;
			case SHave:
			case SAiming: m_model.ChangeAnimation(Aiming); break;
			}
		}
	}
}

void Player::Fire(){
	if (Gun* g = dynamic_cast<Gun*>(Base::FindObject(eGun))) {
		if (g->GetLoadedAmmo() <= 0) {
			if (m_isDryFiringSound)
				SOUND("DryFiring")->Play3D(g->GetMatrix().GetPosition(), CVector3D(1, 1, 1));
			m_isDryFiringSound = (m_state == SAiming) ? false : true;
			return;
		}
		if (m_fireTime <= 0) {
			m_isMaxAmmo = false;
			g->SetLoadedAmmo(1);
			CVector3D pos = g->GetMatrix() * CVector4D(0, 0, -0.4f, 1);
			CVector3D dir = g->GetMatrix().GetFront().GetNormalize();
			SOUND("Shot")->Play3D(g->GetMatrix().GetPosition(), CVector3D(1, 1, 1));
			Base::Add(new Bullet(pos, -dir));
			m_fireTime = 15;
		}
	}
}

void Player::UsePotion(){
	//ポーションを持っているかつ待機状態かつHPが100未満かつEを押したら
	if (m_potionCnt > 0 && m_state == SIdle &&
		m_hp < 100 && PUSH(CInput::eButton1)) {
		m_hp = min(100, m_hp + POTION_HEAL_AMOUNT);
		m_potionCnt--;
		mp_intaractable = nullptr;
	}
}

void Player::Interact(){
	//インタラクト可能なオブジェクトに近づいているかつ
	//Eを押したらインタラクトする
	if (mp_intaractable && PUSH(CInput::eButton1)) 
		mp_intaractable->Interact();
}
