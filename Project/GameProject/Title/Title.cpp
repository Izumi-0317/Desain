#include "Title.h"
#include "Game/Game.h"

Title::Title()
	: Base(eScene)
	, m_cnt(0)
	, m_alpha(0.0f){
	m_titleImg = COPY_RESOURCE("Title", CImage);
	m_startImg = COPY_RESOURCE("Start", CImage);
	m_backImg = COPY_RESOURCE("TitleBack", CImage);
	m_backImg.SetSize(1920, 1080);
}

void Title::Update(){
	if (m_cnt++ >= 2 && PUSH(CInput::eButton5) && !CInput::GetPadData(0) ||
		m_cnt++ >= 2 && PUSH(CInput::eButton3) && CInput::GetPadData(0)) {
		SetKill();
		Base::Add(new Game());
	}
}

void Title::Draw(){
	m_backImg.Draw();
	m_titleImg.SetSize(800, 350);
	m_titleImg.SetPos(560, 240);
	m_titleImg.Draw();
	m_startImg.SetSize(600, 100);
	m_startImg.SetPos(660, 740);
	m_startImg.SetColor(1, 1, 1, 0.3f);
	m_startImg.Draw();
	m_alpha += 0.04f;
	if (m_alpha > DtoR(180)) m_alpha = 0;
	m_startImg.SetColor(1, 1, 1, sin(m_alpha));
	m_startImg.Draw();
}
