#include "HPUI.h"

HPUI::HPUI()
	: Base(eUI){
	m_hpImg = COPY_RESOURCE("HPUI", CImage);
	m_lowHpImg = COPY_RESOURCE("LowHPUI", CImage);
	m_backImg = COPY_RESOURCE("HPUIBack", CImage);
}

void HPUI::Update()
{
}

void HPUI::Draw(){
	m_backImg.SetPos(0, 0);
	m_backImg.Draw();
	m_lowHpImg.SetPos(0, 0);
	m_lowHpImg.Draw();
	m_hpImg.SetPos(0, 0);
	m_hpImg.Draw();
}
