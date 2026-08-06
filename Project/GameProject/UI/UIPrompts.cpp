#include "UIPrompts.h"
#include "Game/Obj/Chara/Player.h"

UIPrompts::UIPrompts()
	: m_promptText("C:\\Windows\\Fonts\\msgothic.ttc", 40)
    , m_lastState(-1){
	m_backImg = COPY_RESOURCE("UIPromptsBack", CImage);
	m_backImg.SetSize(480, 480);
	m_backImg.SetPos(0, 550);
	m_isPad = CInput::GetPadData(0);
}

void UIPrompts::Update()
{
}

void UIPrompts::Draw(){
	m_backImg.Draw();
	if (m_isPad) {
		//pad
		if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
			int currentState = p->GetState();

			if (currentState == p->SIdle || currentState == p->SHave || currentState == p->SAiming) {
				m_prompts.clear();
				m_prompts.push_back("[Lｽﾃｨｯｸ]　　　移動");

				switch (currentState) {
				case p->SIdle:
					m_prompts.push_back("[Lｽﾃｨｯｸ↑] 　ダッシュ");
					m_prompts.push_back("[RT]　　　　　殴打");
					m_prompts.push_back("[X] 　　　　　挙銃");
					if (!p->GetIsMaxAmmo())   m_prompts.push_back("[Y] 　　　リロード");
					if (p->GetIntaractable()) m_prompts.push_back("[A] 　　インタラクト");
					break;
				case p->SHave:
					m_prompts.push_back("[RT]　　　　　発砲");
					m_prompts.push_back("[X] 　　　　　脱銃");
					m_prompts.push_back("[LT]　　　　　エイム");
					if (!p->GetIsMaxAmmo())   m_prompts.push_back("[Y] 　　　リロード");
					if (p->GetIntaractable()) m_prompts.push_back("[A] 　　インタラクト");
					break;
				case p->SAiming:
					m_prompts.push_back("[RT]　　　　　発砲");
					m_prompts.push_back("[LT]　　　　　エイム解除");
					break;
				}
				m_lastState = currentState;
			}
		}
		for (int i = 0; i < m_prompts.size(); i++) {
			m_promptText.Draw(30, 970 - i * 80, 1, 1, 0, m_prompts[i].c_str());
		}
	}
	else {
		//keyboard
        if (Player* p = dynamic_cast<Player*>(Base::FindObject(ePlayer))) {
            int currentState = p->GetState();

            if (currentState == p->SIdle || currentState == p->SHave || currentState == p->SAiming) {
                m_prompts.clear();
                m_prompts.push_back("[WASD]　　　　移動");

                switch (currentState) {
                case p->SIdle:
                    m_prompts.push_back("[SHIFT+W]　　ダッシュ");
                    m_prompts.push_back("[左クリック]　殴打");
                    m_prompts.push_back("[F] 　　　　　挙銃");
                    if (!p->GetIsMaxAmmo())   m_prompts.push_back("[R] 　　　リロード");
                    if (p->GetIntaractable()) m_prompts.push_back("[E] 　　インタラクト");
                    break;
                case p->SHave:
                    m_prompts.push_back("[左クリック]　発砲");
                    m_prompts.push_back("[F] 　　　　　脱銃");
                    m_prompts.push_back("[右クリック]　エイム");
                    if (!p->GetIsMaxAmmo())   m_prompts.push_back("[R] 　　　リロード");
                    if (p->GetIntaractable()) m_prompts.push_back("[E] 　　インタラクト");
                    break;
                case p->SAiming:
                    m_prompts.push_back("[左クリック]　発砲");
                    m_prompts.push_back("[右クリック]　エイム解除");
                    break;
                }
                m_lastState = currentState;
            }
        }
		for (int i = 0; i < m_prompts.size(); i++) {
            m_promptText.Draw(30, 970 - i * 80, 1, 1, 0, m_prompts[i].c_str());
		}
		//m_keyboardImg.SetSize(74, 72);
		//for (int i = 0;i < 4;i++) {
		//	m_keyboardImg.SetRect(74 * i, 0, 74 + 74 * i, 72);
		//	m_keyboardImg.SetPos(30 + 74 * i, 920);
		//	//m_keyboardImg.Draw();
		//}

		//m_keyboardImg.SetRect(296, 0, 370, 72);
		//m_keyboardImg.SetPos(30, 830);
		//m_keyboardImg.Draw();
	}
}
