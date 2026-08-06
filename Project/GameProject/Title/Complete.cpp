#include "Complete.h"
#include "Game/Obj/Chara/Player.h"
#include "Title.h"

Complete::Complete()
	: Base(eScene)
	, m_alpha(0.0f)
	, m_lerpTime(0.0f)
	, m_isSound(true){
	m_completeImg = COPY_RESOURCE("Complete", CImage);
	m_returnImg = COPY_RESOURCE("Return", CImage);
	m_backImg = COPY_RESOURCE("CompleteBack", CImage);
	m_backImg.SetSize(1920, 1080);
	m_backImg.SetColor(1.0f, 1.0f, 1.0f, m_lerpTime);
	Kill(1 << eUI);
	if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
		p->SetCanAct(false);
	}
}

void Complete::Update(){
	//フェードイン
	m_lerpTime = min(1.0f, m_lerpTime + 0.02f);

	if (m_lerpTime >= 1.0f) {
		Kill(~(1 << eScene));
		if (m_isSound) {
			SOUND("Complete")->Play();
			m_isSound = false;
		}
		if (PUSH(CInput::eButton5) && !CInput::GetPadData(0) ||
			PUSH(CInput::eButton3) && CInput::GetPadData(0)) {
			SetKill();
			Base::Add(new Title());
		}
	}
}

void Complete::Draw() {
	m_backImg.SetColor(1.0f, 1.0f, 1.0f, m_lerpTime);
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
