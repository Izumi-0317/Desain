#include "GameOver.h"
#include "Title.h"

GameOver::GameOver()
	: Base(eScene)
	, m_cnt(0)
	, m_alpha(0.0f){
	m_gameOverImg = COPY_RESOURCE("GameOver", CImage);
	m_returnImg = COPY_RESOURCE("Return", CImage);
	m_backImg = COPY_RESOURCE("GameOverBack", CImage);
	m_backImg.SetSize(1920, 1080);
	SOUND("GameOver")->Play();
}

void GameOver::Update(){
	if (m_cnt++ >= 2 && PUSH(CInput::eButton5)) {
		SetKill();
		Base::Add(new Title());
	}
}

void GameOver::Draw(){
	m_backImg.Draw();
	m_gameOverImg.SetSize(800, 350);
	m_gameOverImg.SetPos(560, 240);
	m_gameOverImg.Draw();
	m_returnImg.SetSize(600, 100);
	m_returnImg.SetPos(660, 740);
	m_returnImg.SetColor(1, 1, 1, 0.3f);
	m_returnImg.Draw();
	m_alpha += 0.04f;
	if (m_alpha > DtoR(180)) m_alpha = 0;
	m_returnImg.SetColor(1, 1, 1, sin(m_alpha));
	m_returnImg.Draw();
}
