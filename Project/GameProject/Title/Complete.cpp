#include "Complete.h"
#include "Title.h"

Complete::Complete()
	: Base(eScene)
	, m_cnt(0){
	m_backImg = COPY_RESOURCE("TitleBack", CImage);//TODO::‰¼
	m_backImg.SetSize(1920, 1080);
}

void Complete::Update(){
	if (m_cnt++ >= 2 && PUSH(CInput::eButton5)) {
		SetKill();
		Base::Add(new Title());
	}
}

void Complete::Draw(){
	m_backImg.SetFlipV(true);
	m_backImg.Draw();
}
