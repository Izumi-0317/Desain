#include "UIAmmo.h"
#include "Game/Obj/Weapon/Gun.h"

UIAmmo::UIAmmo()
	: m_ammo(0){
	m_rifleImg = COPY_RESOURCE("UIRifle", CImage);
	m_backImg = COPY_RESOURCE("UIWeaponBack", CImage);
	m_rifleImg.SetSize(288, 96);
}

void UIAmmo::Update() {
	if (Gun* g = dynamic_cast<Gun*>(Base::FindObject(eGun))) {
		m_ammo = g->GetLoadedAmmo();
	}
}

void UIAmmo::Draw() {
	m_backImg.SetPos(1220, 880);
	m_backImg.Draw();
	int ammo = 30;
	for (int i = 0;i < 2;i++, ammo /= 10) {
		int a = ammo % 10;
		m_cntImg.SetSize(48, 48);
		m_cntImg.SetRect(32 * a, 0, 32 + 32 * a, 32);
		m_cntImg.SetPos(1800 - 48 * i, 940);
		m_cntImg.Draw();
	}
	m_cntImg.SetSize(64, 64);
	m_cntImg.SetRect(320, 0, 352, 32);
	m_cntImg.SetPos(1800 - 64 * 2, 940);
	m_cntImg.Draw();
	ammo = m_ammo;
	for (int i = 3;i < 5;i++, ammo /= 10) {
		int a = ammo % 10;
		m_cntImg.SetRect(32 * a, 0, 32 + 32 * a, 32);
		m_cntImg.SetPos(1800 - 64 * i, 940);
		m_cntImg.Draw();
	}
	m_rifleImg.SetPos(1250, 930);
	m_rifleImg.Draw();
}
