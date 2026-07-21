#include "UIPotion.h"
#include "Game/Obj/Chara/Player.h"

TexAnim _potion[] = {
	{0,10},
};
TexAnim _drink[] = {
	{0,10},
	{1,10},
	{2,10},
	{3,10},
};
TexAnimData UIPotion::_anim[] = {
	ANIMDATA(_potion),
	ANIMDATA(_drink),
};

UIPotion::UIPotion() 
	: m_potionCnt(0) {
	m_potionImg = COPY_RESOURCE("UIPotion", CImage);
	m_backImg = COPY_RESOURCE("UIPotionBack", CImage);
	m_potionImg.SetSize(128, 128);
	m_potionImg.SetCenter(64, 64);
	m_potionImg.ChangeAnimation(0);
	m_backImg.SetSize(192, 192);
	m_backImg.SetCenter(86, 86);
}

void UIPotion::Update(){
	m_potionImg.UpdateAnimation();
}

void UIPotion::Draw(){
	m_backImg.SetPos(1814, 750);
	m_backImg.Draw();
	if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
		int getPotion = p->GetPotionCnt();
		if (m_potionCnt > getPotion) {
			m_potionImg.ChangeAnimation(1, false);
			if (m_potionImg.CheckAnimationEnd())
				m_potionCnt = getPotion;
		}
		else {
			m_potionCnt = getPotion;
			m_potionImg.ChangeAnimation(0);
		}

		m_potionImg.SetPos(1814, 750);
		m_potionImg.Draw();
		m_cntImg.SetSize(32, 32);
		m_cntImg.SetRect(32 * getPotion, 0, 32 + 32 * getPotion, 32);
		m_cntImg.SetPos(1858, 780);
		m_cntImg.Draw();
	}
}
