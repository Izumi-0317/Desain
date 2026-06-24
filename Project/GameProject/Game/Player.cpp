#include "Player.h"
#include "Bullet.h"
#include "Camera.h"
#include "Field.h"
#include "Game.h"
#include "Gun.h"
#include "Scope.h"

namespace {
	constexpr float PLAYER_HEIGHT = 1.4f;
	constexpr float DEFAULT_MOVE_SPEED = 0.04f;
	constexpr float RUN_SPEED = 2.0f;
	constexpr float HAVE_SPEED = 0.75f;
	constexpr float AIMING_SPEED = 0.25f;
}

Player::Player(const CVector3D& pos)
	: CharaBase(ePlayer)
	, m_fireTime(0)
	, m_undoState(SIdle)
	, m_animFlag(false)
	, m_attackFlag(false)
	, m_isMaxAmmo(true){
	m_model = COPY_RESOURCE("Player", CModelA3M);
	m_playerMat = (CMatrix::MTranselate(m_pos) * CMatrix::MRotation(m_rot));
	m_pos = pos;
	m_state = SIdle;
	m_hp = 100;
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
	if(m_fireTime > 0) m_fireTime--;

	//挙銃のアニメーション遷移
	if (m_animFlag && (int)m_model.GetAnimation() != DownToAim) {
		m_state = -1;
		m_model.ChangeAnimation(DownToAim, false);
		if (m_model.isAnimationEnd()) {
			m_animFlag = false;
			NextState(SHave);
		}
	}

	//リロード
	if ((m_state == SIdle || m_state == SHave) &&
		!m_isMaxAmmo) {
		if(PUSH(CInput::eButton2))
		NextState(SReloaded);
	}

	//デバッグ
	//if (PUSH(CInput::eButton1)) Game::m_cameraMode = !Game::m_cameraMode;
}

void Player::Render(){
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(0.01f, 0.01f, 0.01f);
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

void Player::StateIdle(){
	Move(RUN_SPEED);
	//攻撃
	if (PUSH(CInput::eMouseL)) NextState(SHit);
	if (PUSH(CInput::eNum1) || PUSH(CInput::eNum2)) m_animFlag = true;
}

void Player::StateHit(){
	//TODO::ステップ式
	//30~38 Hit
	m_model.SetAnimationSpeed(2.5f);
	m_model.ChangeAnimation(Hit, false);
	m_attackFlag = true;
	//Utility::DrawSphere(m_pos + CVector3D(0, 1, 0) + m_dir.GetNormalize() * 0.5f, 0.2f, CVector4D(1, 0, 0, 1));
	if (m_model.isAnimationEnd()) {
		m_model.SetAnimationSpeed(1.0f);
		NextState(SIdle);
		m_attackFlag = false;
	}
}

void Player::StateHave(){
	if (!PUSH(CInput::eMouseL) || HOLD(CInput::eMouseL)) Move(HAVE_SPEED);
	else Fire();

	if (PUSH(CInput::eButton3)) NextState(SAiming);
	if (PUSH(CInput::eNum3)) NextState(SIdle);
}

void Player::StateAiming(){
	if (!Base::FindObject(eScope)) Base::Add(new Scope());
	if (!HOLD(CInput::eMouseL) || PUSH(CInput::eMouseL)) Move(AIMING_SPEED);
	else Fire();

	if (PUSH(CInput::eButton3)) NextState(SHave);
}

void Player::StateReloaded(){
	if (Gun* g = dynamic_cast<Gun*>(Base::FindObject(eGun))) {
		//弾を持っていれば
		if (g->GetStockAmmo() != 0) {
			m_model.ChangeAnimation(Reloading, false);
			if (m_model.GetAnimationFrame() >= 50) {
				g->Reloaded();
			}
			if (m_model.isAnimationEnd()) NextState(m_undoState);
		}
		else NextState(m_undoState);
	}
}

void Player::StateDamage(){
	m_model.SetAnimationSpeed(2.0f);
	m_model.ChangeAnimation(Damage, false);
	if (m_model.isAnimationEnd()) {
		m_model.SetAnimationSpeed(1.0f);
		NextState(SIdle);
	}
}

void Player::StateDeath(){
	m_model.ChangeAnimation(Death, false);
	if (m_model.isAnimationEnd()); //TODO::GameOver
}

void Player::Collision(Base* b){
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
	case eEnemy:
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
				i->TakeDamage(50);
				m_attackFlag = false;
			}
		}
		break;
	}
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
			//TODO::空撃ち音
			return;
		}
		if (m_fireTime <= 0) {
			m_isMaxAmmo = false;
			g->SetLoadedAmmo(1);
			CVector3D p = g->GetMatrix() * CVector4D(0, 0, -0.4f, 1);
			CVector3D d = g->GetMatrix().GetFront().GetNormalize();
			Base::Add(new Bullet(p, -d));
			m_fireTime = 10;
		}
	}
}
