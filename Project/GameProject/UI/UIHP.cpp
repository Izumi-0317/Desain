#include "UIHP.h"
#include "Game/Obj/Chara/Player.h"

namespace {
	constexpr int DELAY_TIME = 5;
}

UIHP::UIHP() 
	: m_currentHP(100)
	, m_lowStartHP(0)
	, m_delayTime(0)
	, m_lerpTime(1.0f)
	, m_lowHPSize(1000.0f){
	m_hpImg = COPY_RESOURCE("UIHP", CImage);
	m_lowHpImg = COPY_RESOURCE("UILowHP", CImage);
	m_backImg = COPY_RESOURCE("UIHPBack", CImage);
}

void UIHP::Update(){
	if (m_delayTime <= 0.0f && m_lerpTime < 1.0f) m_lerpTime += 0.01f;
	if (m_delayTime > 0) m_delayTime--;
}

void UIHP::Draw(){
	m_backImg.SetPos(0, 0);
	m_backImg.Draw();
	if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
		//HPが減ったら各種設定
		if (m_currentHP != p->m_hp) {
			m_lowStartHP = m_currentHP;		//赤色のHPバーの減り始める位置
			m_currentHP = p->m_hp;			//減ったHPを現在のHPに設定
			m_lerpTime = 0.0f;				//補間係数を0.0fにする
			m_delayTime = DELAY_TIME;		//5フレーム待ってから赤色のHPバーを減らす
		}
		//赤色のHPバーのサイズは線形補間で設定
		m_lowHPSize = CVector3D::Leap(CVector3D(m_lowStartHP, 0, 0), CVector3D(m_currentHP, 0, 0), m_lerpTime).x;
	}

	m_lowHpImg.SetPos(0, 0);
	m_lowHpImg.SetSize(m_lowHPSize * 10, 50);
	m_lowHpImg.Draw();

	m_hpImg.SetPos(0, 0);
	m_hpImg.SetSize(m_currentHP * 10, 50);
	m_hpImg.Draw();
}
