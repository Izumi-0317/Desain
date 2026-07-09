#include "PotionUI.h"
#include "Game/Obj/Chara/Player.h"

PotionUI::PotionUI()
	: Base(eUI){
	m_cntImg = COPY_RESOURCE("CntUI", CImage);
	m_potionImg = COPY_RESOURCE("PotionUI", CImage);
	m_backImg = COPY_RESOURCE("PotionUIBack", CImage);
	m_potionImg.SetSize(128, 128);
	m_potionImg.SetCenter(64, 64);
	m_backImg.SetSize(192, 192);
	m_backImg.SetCenter(86, 86);
}

void PotionUI::Update()
{
}

void PotionUI::Draw(){
	m_backImg.SetPos(1814, 750);
	m_backImg.Draw();
	m_potionImg.SetPos(1814, 750);
	m_potionImg.Draw();
	if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
		int potion = p->GetPotionCnt();
		m_cntImg.SetSize(32, 32);
		m_cntImg.SetRect(32 * potion, 0, 32 + 32 * potion, 32);
		m_cntImg.SetPos(1858, 780);
		m_cntImg.Draw();
	}
}
