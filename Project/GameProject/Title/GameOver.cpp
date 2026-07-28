#include "GameOver.h"
#include "Title.h"

GameOver::GameOver()
	: Base(eScene)
	, m_cnt(0){
	m_backImg = COPY_RESOURCE("TitleBack", CImage);	//TODO::‰¼
	m_backImg.SetSize(1920, 1080);
}

void GameOver::Update(){
	if (m_cnt++ >= 2 && PUSH(CInput::eButton5)) {
		SetKill();
		Base::Add(new Title());
	}
}

void GameOver::Draw(){
	m_backImg.SetFlipH(true);
	m_backImg.Draw();
}
