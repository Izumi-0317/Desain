#include "AmmoUI.h"
#include "Game/Gun.h"

AmmoUI::AmmoUI()
	: Base(eUI)
	, m_ammo(0){
	m_ammoImg = COPY_RESOURCE("AmmoUI", CImage);
	m_rifleImg = COPY_RESOURCE("RifleUI", CImage);
	m_backImg = COPY_RESOURCE("WeaponUIBack", CImage);
	m_rifleImg.SetSize(288, 96);
}

void AmmoUI::Update() {
	if (Gun* g = dynamic_cast<Gun*>(Base::FindObject(eGun))) {
		m_ammo = g->GetLoadedAmmo();
	}
}

void AmmoUI::Draw() {
	m_backImg.SetPos(1220, 880);
	m_backImg.Draw();
	int ammo = 30;
	for (int i = 0;i < 2;i++, ammo /= 10) {
		int a = ammo % 10;
		m_ammoImg.SetSize(48, 48);
		m_ammoImg.SetRect(32 * a, 0, 32 + 32 * a, 32);
		m_ammoImg.SetPos(1800 - 48 * i, 940);
		m_ammoImg.Draw();
	}
	m_ammoImg.SetSize(64, 64);
	m_ammoImg.SetRect(320, 0, 352, 32);
	m_ammoImg.SetPos(1800 - 64 * 2, 940);
	m_ammoImg.Draw();
	ammo = m_ammo;
	for (int i = 3;i < 5;i++, ammo /= 10) {
		int a = ammo % 10;
		m_ammoImg.SetRect(32 * a, 0, 32 + 32 * a, 32);
		m_ammoImg.SetPos(1800 - 64 * i, 940);
		m_ammoImg.Draw();
	}
	m_rifleImg.SetPos(1250, 930);
	m_rifleImg.Draw();
}
