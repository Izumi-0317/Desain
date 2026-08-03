#include "Complete.h"
#include "Title.h"

Complete::Complete(const CTexture& texture)
	: Base(eScene)
	, m_cnt(0)
	, m_alpha(0.0f)
	, m_lerpTime(0.0f){
	m_completeImg = COPY_RESOURCE("Complete", CImage);
	m_returnImg = COPY_RESOURCE("Return", CImage);
	m_backImg = COPY_RESOURCE("CompleteBack", CImage);
	/**m_backImg.mp_texture = texture;
	m_backImg.SetRect(0, 0, 1920.0f, 1080.0f);
	m_backImg.SetPos(0, 0);
	m_backImg.SetColor(1.0f, 1.0f, 1.0f, 1.0f);*/
	m_backImg.SetSize(1920, 1080);
}
//TODO::フェードインしてからkillallを実行
Complete::~Complete(){
	
}

void Complete::Update(){
	if (m_lerpTime >= 1.0f) SOUND("Complete")->Play();
	if (m_cnt++ >= 2 && PUSH(CInput::eButton5)) {
		SetKill();
		Base::Add(new Title());
	}
}

void Complete::Draw() {
	/*m_backImg.Draw();
	m_completeImg.SetSize(800, 350);
	m_completeImg.SetPos(560, 240);
	m_completeImg.Draw();
	m_returnImg.SetSize(600, 100);
	m_returnImg.SetPos(660, 740);
	m_returnImg.SetColor(1, 1, 1, 0.3f);
	m_returnImg.Draw();
	m_alpha += 0.04f;
	if (m_alpha > DtoR(180)) m_alpha = 0;
	m_returnImg.SetColor(1, 1, 1, sin(m_alpha));
	m_returnImg.Draw();*/

	m_backImg.Draw();
	m_completeImg.SetSize(800, 350);
	m_completeImg.SetPos(560, 240);
	m_completeImg.Draw();
	m_returnImg.SetSize(600, 100);
	m_returnImg.SetPos(660, 740);
	m_returnImg.SetColor(1, 1, 1, 0.3f);
	m_returnImg.Draw();
	m_alpha += 0.04f;
	if (m_alpha > DtoR(180)) m_alpha = 0;
	m_returnImg.SetColor(1, 1, 1, sin(m_alpha));
	m_returnImg.Draw();
}
