#include "UIHP.h"
#include "Game/Obj/Chara/CharaBase.h"

namespace {
	constexpr int DELAY_TIME = 20;
	constexpr float PLAYER_HP_SIZE = 1000.0f;
	constexpr float BOSS_HP_SIZE = 1500.0f;
}

UIHP::UIHP(int type) 
	: m_type(type)
	, m_maxHp(0)
	, m_currentHp(100)
	, m_lowStartHp(0)
	, m_delayTime(0)
	, m_lerpTime(1.0f)
	, m_lowHpSize(0.0f){
	m_hpImg = COPY_RESOURCE("UIHP", CImage);
	m_lowHpImg = COPY_RESOURCE("UILowHP", CImage);
	m_backImg = COPY_RESOURCE("UIHPBack", CImage);
	m_nameImg = COPY_RESOURCE("BossName", CImage);
	if (CharaBase* c = ObjectManager::FindObject<CharaBase>(type)) {
		switch (type) {
		case ObjectType::ePlayer:
		case ObjectType::eBoss:
			m_maxHp = m_currentHp = c->m_maxHp;
			break;
		}
	}
}

void UIHP::Update(){
	if (m_delayTime <= 0.0f && m_lerpTime < 1.0f) m_lerpTime += 0.01f;
	if (m_delayTime > 0) m_delayTime--;
}

void UIHP::Draw(){
	if (CharaBase* c = ObjectManager::FindObject<CharaBase>(m_type)) {
		//HPが減ったら各種設定
		if (m_currentHp != c->m_hp) {
			//連続で攻撃を受けていなければ
			if (m_delayTime <= 0.0f)
				m_lowStartHp = m_currentHp;	//赤色のHPバーの減り始める位置
			//減り始める位置が現在のHPより低ければ
			if (m_lowStartHp <= c->m_hp)
				m_lowStartHp = c->m_hp;		//減り始める位置を現在のHPに設定

			m_currentHp = c->m_hp;			//減ったHPを現在のHPに設定
			m_lerpTime = 0.0f;				//補間係数を0.0fにする
			m_delayTime = DELAY_TIME;		//5フレーム待ってから赤色のHPバーを減らす
		}

		//これ以上減らせないなら赤色のHPバーを透明にする
		if (c->m_hp <= 0 && m_lerpTime >= 1.0f)
			m_lowHpImg.SetColor(0, 0, 0, 0);

		//赤色のHPバーのサイズは線形補間で設定
		m_lowHpSize = CVector3D::Leap(CVector3D(m_lowStartHp, 0, 0), CVector3D(m_currentHp, 0, 0), m_lerpTime).x;
	}

	switch (m_type){
	case ObjectType::ePlayer:
		m_backImg.SetPos(0, 1030);

		m_lowHpImg.SetPos(0, 1030);
		m_lowHpImg.SetSize(m_lowHpSize * 10, PLAYER_HP_SIZE / 20);

		m_hpImg.SetPos(0, 1030);
		m_hpImg.SetSize(m_currentHp * 10, PLAYER_HP_SIZE / 20);
		m_hpImg.SetColor(0, 1, 0, 1);
		break;
	case ObjectType::eBoss:
		m_backImg.SetPos(210, 125);
		m_backImg.SetSize(BOSS_HP_SIZE, BOSS_HP_SIZE / 20);

		m_lowHpImg.SetPos(210, 125);
		m_lowHpImg.SetSize(m_lowHpSize * 3, BOSS_HP_SIZE / 20);

		m_hpImg.SetPos(210, 125);
		m_hpImg.SetSize(m_currentHp * 3, BOSS_HP_SIZE / 20);
		m_hpImg.SetColor(0.7f, 0, 0.4f, 1);

		m_nameImg.SetPos(810, 25);
		m_nameImg.SetSize(300, 100);
		m_nameImg.Draw();
		break;
	}

	m_backImg.Draw();
	m_lowHpImg.Draw();
	m_hpImg.Draw();
}
