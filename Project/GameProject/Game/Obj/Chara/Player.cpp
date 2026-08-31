#include "Player.h"
#include "Base/ObjectManager.h"
#include "Game/Camera.h"
#include "Game/Obj/Gimmick/GimmickBase.h"
#include "Game/Game.h"
#include "Game/Obj/Weapon/Bullet.h"
#include "Game/Obj/Weapon/Gun.h"

namespace {
	constexpr int PLAYER_HP = 100;
	constexpr int POTION_HEAL_AMOUNT = 30;
	constexpr float PLAYER_HEIGHT = 2.8f;
	constexpr float DEFAULT_MOVE_SPEED = 0.06f;
	constexpr float RUN_SPEED = 2.0f;
	constexpr float HAVE_SPEED = 1.0f;
	constexpr float AIMING_SPEED = 0.5f;
}

using enum CInput::E_BUTTON;

Player::Player(const CVector3D& pos)
	: CharaBase(ObjectType::ePlayer)
	, m_fireTime(0)
	, m_potionCnt(0)
	, m_canAct(true)
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
	m_hp = m_maxHp = PLAYER_HP;
	m_rad = 0.2f;
	m_model.ChangeAnimation(Idle);
	m_upper_body = 4;
	int m_body_end = 56;
	for (int i = m_upper_body; i <= m_body_end; i++)
		m_model.GetNode(i)->SetAnimationLayer(1);

	new Gun("AK47");
	if (Camera* c = ObjectManager::FindObject<Camera>(ObjectType::eCamera)) {
		c->m_rot = CVector3D(0, DtoR(180), 0);
	}
}

void Player::Update(){
	CharaBase::Update();
	if (Camera* c = ObjectManager::FindObject<Camera>(ObjectType::eCamera)) {
		m_rot.y = c->m_rot.y;
	}

	if (m_canAct) {
		switch (m_state) {
		case SIdle:	  StateIdle();		 break;
		case SDamage: StateDamage();	 break;
		case SDeath:  StateDeath();		 break;
		case SPunch:  StatePunch();		 break;
		case SHave:	  StateHave();		 break;
		case SAiming: StateAiming();	 break;
		case SReloaded: StateReloaded(); break;
		}
	}
	else {
		if(m_state != SIdle) ChangeState(SHave);
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
	TransitionAnim(m_DownToAimFlag, DownToAim, SHave);
	//脱銃のアニメーション遷移
	TransitionAnim(m_AimToDownFlag, AimToDown, SIdle);

	//リロード
	if ((m_state == SIdle || m_state == SHave) &&
		!m_isMaxAmmo) {
		if (IsInput(eButton2, eButton1))
			ChangeState(SReloaded);
	}

	mp_intaractable = nullptr;
	//デバッグ
	//if (PUSH(CInput::eButton5)) m_vec.y = 5;
	//m_pos += m_vec;
}

void Player::Render(){
	m_model.SetPos(m_pos);
	m_model.SetRot(m_rot);
	m_model.SetScale(0.02f, 0.02f, 0.02f);
	m_model.UpdateMatrix();
	if (Camera* c = ObjectManager::FindObject<Camera>(ObjectType::eCamera)) {
		c->UpdateCam();
		if (m_state == SPunch) {
			m_model.BindFrameMatrix(m_upper_body, CMatrix::MRotation(c->m_rot), CA3MNode::eBind_Relative);
		}
		else {
			m_model.BindFrameMatrix(m_upper_body,
				CMatrix::MRotation(c->m_rot) * CMatrix::MRotationY(DtoR(-40)), CA3MNode::eBind_Absolute);
		}
	}
	m_model.Render();
	//Utility::DrawCapsule(m_capusle, CVector4D(1, 0, 0, 0.5f));
	//プレイヤーの更新・描画後に銃を更新
	if (Gun* g = ObjectManager::FindObject<Gun>(ObjectType::eGun)) {
		g->UpdateGun();
	}
}

void Player::Collision(ObjectBase* b) {
	switch (b->GetType()) {
	case ObjectType::eRoom: {
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
		break;
	}
	case ObjectType::eBoss:
	case ObjectType::eEnemy: {
		float dist;
		CVector3D cross, dir;
		if (CCollision::CollisionCapsule(m_capusle, b->m_capusle, &dist, &cross, &dir)) {
			float s = (m_capusle.GetRadius() + b->m_capusle.GetRadius()) - dist;
			m_pos -= dir * s;
		}
		//近接攻撃
		if (Camera* c = ObjectManager::FindObject<Camera>(ObjectType::eCamera)) {
			CVector3D center = m_pos + CVector3D(0, 1, 0) + c->m_dir.GetNormalize() * 1.5f;
			if (m_attackFlag && CCollision::CollisionCapsuleShpere(*b->GetCapsule(),
				center, 1.0f,
				&dist, &cross, &dir)) {
				if (Interface* i = dynamic_cast<Interface*>(b)) {
					SOUND("Punch")->Play();
					i->TakeDamage(10);
					m_attackFlag = false;
				}
			}
		}
		break;
	}
	case ObjectType::eChest:
	case ObjectType::eDoor:
	case ObjectType::ePotion: {
		float length;
		CVector3D axis;
		if (CCollision::CollisionOBBCapsule(b->m_obb, m_capusle, &axis, &length)) {
			m_pos += axis * (m_rad - length);
			if (GimmickBase* g = dynamic_cast<GimmickBase*>(b)) {
				//インタラクトする対象に設定
				mp_intaractable = g;
			}
		}
		break;
	}
	}
}

void Player::StateIdle(){
	Move(RUN_SPEED);
	UsePotion();
	Interact();
	//攻撃
	if (IsInput(eMouseL, eButton6)) ChangeState(SPunch);
	if (IsInput(eButton6, eButton4)) m_DownToAimFlag = true;
}

void Player::StatePunch(){
	m_model.SetAnimationSpeed(1, 2.5f);
	m_model.SetAnimationSpeed(0, 2.5f);
	m_model.ChangeAnimation(1, Punch, false);
	m_model.ChangeAnimation(0, Punch, false);
	switch (m_stateStep){
	case 0:
		if (m_model.GetAnimationFrame() >= 30) {
			m_attackFlag = true;
			/*if (Camera* c = dynamic_cast<Camera*>(Base::FindObject(eCamera)))
				Utility::DrawSphere(m_pos + CVector3D(0, 1, 0) + c->m_dir.GetNormalize() * 1.5f,
					1.0f, CVector4D(1, 0, 0, 1));*/
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
		m_model.SetAnimationSpeed(1, 1.0f);
		m_model.SetAnimationSpeed(0, 1.0f);
		ChangeState(SIdle);
	}
}

void Player::StateHave(){
	if (IsInput(eMouseL, eButton6)) Fire();
	Move(HAVE_SPEED);
	Interact();

	if (IsInput(eMouseR, eButton5, CInput::eHold)) ChangeState(SAiming);
	if (IsInput(eButton6, eButton4)) m_AimToDownFlag = true;
}

void Player::StateAiming(){
	if (IsInput(eMouseL, eButton6, CInput::eHold)) Fire();
	Move(AIMING_SPEED);

	if (IsInput(eMouseR, eButton5, CInput::ePull)) ChangeState(SHave);
}

void Player::StateReloaded(){
	if (Gun* g = ObjectManager::FindObject<Gun>(ObjectType::eGun)) {
		if (m_undoState != SDamage) {
			m_model.ChangeAnimation(1, Reloading, false);
			if (m_model.GetAnimationFrame(1) >= 50) {
				//リロード
				if (!m_isMaxAmmo) {
					g->Reloaded();
					m_isMaxAmmo = true;
				}
			}
			if (m_model.isAnimationEnd(1)) ChangeState(m_undoState);
		}
		else if (m_model.isAnimationEnd(1)) ChangeState(SHave);
	}
}

void Player::StateDamage(){
	m_model.SetAnimationSpeed(2.0f);
	m_model.ChangeAnimation(1, Damage, false);
	m_model.ChangeAnimation(0, Damage, false);
	if (m_model.isAnimationEnd()) {
		m_model.SetAnimationSpeed(1.0f);
		ChangeState(m_undoState);
	}
}

void Player::StateDeath(){
	m_model.ChangeAnimation(1, Death, false);
	m_model.ChangeAnimation(0, Death, false);
	if (m_model.isAnimationEnd()) {
		if (Game* g = ObjectManager::FindObject<Game>(ObjectType::eScene)) {
			g->markAsGameOver();
		}
	}
}

void Player::Move(float speed){
	CVector3D keyDir(0, 0, 0);
	//移動
	if (HOLD(CInput::eUp)) keyDir.z = 1;
	if (HOLD(CInput::eDown)) keyDir.z = -1;
	if (HOLD(CInput::eLeft)) keyDir.x = 1;
	if (HOLD(CInput::eRight)) keyDir.x = -1;
	if (CInput::GetPadData(0)) {
		CVector2D axis = CInput::GetLStick(0);
		keyDir = CVector3D(-axis.x, 0, axis.y);
	}
	if (Camera* f = ObjectManager::FindObject<Camera>(ObjectType::eCamera)) {
		if (keyDir.LengthSq() > 0.05f) {
			float keyForce = min(1.0f, keyDir.Length());
			if (SOUND("FootstepsWalk")->CheckEnd()) {
				float pitch = min(1.0f, speed * 1.5f);
				SOUND("FootstepsWalk")->Pitch(pitch);
				SOUND("FootstepsWalk")->Play3D(m_pos, m_pos);
			}

			m_dir = (CMatrix::MRotationY(m_rot.y) * keyDir).GetNormalize();
			switch (m_state){
			case SIdle:
				m_model.ChangeAnimation(1, Walk);
				m_model.ChangeAnimation(0, Walk);
				if (keyDir.z > 0) {
					//走っているなら
					if (HOLD(CInput::eButton4) ||
						keyForce > 0.7f && CInput::GetPadData(0)) {
						SOUND("FootstepsWalk")->Stop();
						if (SOUND("FootstepsRun")->CheckEnd()) {
							SOUND("FootstepsRun")->Volume(0.4f);
							SOUND("FootstepsRun")->Play3D(m_pos, m_pos);
						}

						m_pos += m_dir * (DEFAULT_MOVE_SPEED * speed);
					}
					else {
						SOUND("FootstepsRun")->Stop();
						m_pos += m_dir * (DEFAULT_MOVE_SPEED * keyForce);
					}
				}
				else {
					SOUND("FootstepsRun")->Stop();
					m_pos += m_dir * (DEFAULT_MOVE_SPEED * keyForce);
				}
				/*if (keyDir.z > 0) {
				if (keyDir.LengthSq() < 0.3f || !HOLD(CInput::eButton4)) m_model.ChangeAnimation(Walk);
				else {
					m_pos += m_dir * (MOVE_SPEED * 2.0f);
					m_model.ChangeAnimation(Run);
				}
			}
			else if (keyDir.z < 0) m_model.ChangeAnimation(BackWardWalk);
			else if (keyDir.x > 0) m_model.ChangeAnimation(SideStepL);
			else if (keyDir.x < 0) m_model.ChangeAnimation(SideStepR);*/
				break;
			case SHave:
			case SAiming:
				m_model.ChangeAnimation(1, Aiming);
				m_pos += m_dir * (DEFAULT_MOVE_SPEED * speed);
				if (keyDir.z > 0) m_model.ChangeAnimation(0, StrafeF);
				else if (keyDir.z < 0) m_model.ChangeAnimation(0, StrafeB);
				else if (keyDir.x > 0) m_model.ChangeAnimation(0, StrafeL);
				else if (keyDir.x < 0) m_model.ChangeAnimation(0, StrafeR);
				break;
			}
		}
		else {
			SOUND("FootstepsRun")->Stop();
			SOUND("FootstepsWalk")->Stop();
			switch (m_state){
			case SIdle:
				m_model.ChangeAnimation(1, Idle);
				m_model.ChangeAnimation(0, Idle);
				break;
			case SHave:
			case SAiming:
				m_model.ChangeAnimation(1, Aiming);
				m_model.ChangeAnimation(0, Aiming);
				break;
			}
		}
	}
}

void Player::Fire(){
	if (Gun* g = ObjectManager::FindObject<Gun>(ObjectType::eGun)) {
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
			new Bullet(pos, -dir);
			m_fireTime = 15;
		}
	}
}

void Player::UsePotion(){
	//ポーションを持っているかつ待機状態かつHPが100未満かつ対応するキーを押したら
	if (m_potionCnt > 0 && m_state == SIdle &&
		m_hp < 100 && 
		IsInput(eButton1, eButton3)) {
		SOUND("Potion")->Play3D(m_pos, m_pos);
		m_hp = min(100, m_hp + POTION_HEAL_AMOUNT);
		m_potionCnt--;
		mp_intaractable = nullptr;
	}
}

void Player::Interact(){
	if (mp_intaractable == nullptr) return;
	//インタラクト可能なオブジェクトに近づいているかつ
	//対応するキーを押したらインタラクトする
	if (mp_intaractable && 
		IsInput(eButton1, eButton3))
		mp_intaractable->Interact();
}

void Player::AddForce(const CVector3D& vec, float force){
	m_pos += vec.GetNormalize() * force;
}

void Player::TransitionAnim(bool& flag, int anim, int nextState){
	if (flag && (int)m_model.GetAnimation() != anim) {
		m_state = -1;
		m_model.SetAnimationSpeed(1, 2.0f);
		m_model.ChangeAnimation(1, anim, false);
		if (m_model.isAnimationEnd(1)) {
			flag = false;
			m_model.SetAnimationSpeed(1, 1.0f);
			ChangeState(nextState);
		}
	}
}
