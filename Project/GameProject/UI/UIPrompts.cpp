#include "UIPrompts.h"
#include "Game/Obj/Chara/Player.h"

UIPrompts::UIPrompts()
	: m_promptText("C:\\Windows\\Fonts\\msgothic.ttc", 30)
    , m_lastState(-1){
	m_backImg = COPY_RESOURCE("UIPromptsBack", CImage);
	m_backImg.SetSize(360, 360);
	m_backImg.SetPos(0, 670);
	m_isPad = CInput::GetPadData(0);
}

void UIPrompts::Update()
{
}

void UIPrompts::Draw(){
	m_backImg.Draw();
	if (m_isPad) {
		//pad
		DrawPrompts({
			.Move = "[Lｽﾃｨｯｸ]　　　移動",
			.Dash = "[Lｽﾃｨｯｸ↑] 　ダッシュ",
			.Punch = "[RT]　　　　　殴打",
			.Ready = "[X] 　　　　　挙銃",
			.Shot = "[RT]　　　　　発砲",
			.Safe = "[X] 　　　　　脱銃",
			.Aim = "[LT]　　　　　エイム",
			.Hipfire = "[LT]　　　　　エイム解除",
			.Reload = "[Y] 　　　リロード",
			.Interact = "[A] 　　インタラクト",
			.Potion = "[A] 　　　ポーション"
			});
	}
	else {
		//keyboard
		DrawPrompts({
			.Move = "[WASD]　　　　移動",
			.Dash = "[SHIFT+W]　　ダッシュ",
			.Punch = "[左クリック]　殴打",
			.Ready = "[F] 　　　　　挙銃",
			.Shot = "[左クリック]　発砲",
			.Safe = "[F] 　　　　　脱銃",
			.Aim = "[右クリック]　エイム",
			.Hipfire = "[右クリック]　エイム解除",
			.Reload = "[R] 　　　リロード",
			.Interact = "[E] 　　インタラクト",
			.Potion = "[E] 　　　ポーション"
			});
	}
}

void UIPrompts::DrawPrompts(const Prompts& pro){
    if (Player* p = ObjectManager::FindObject<Player>(ObjectType::ePlayer)) {
        int currentState = p->GetState();

        if (currentState == p->SIdle || currentState == p->SHave || currentState == p->SAiming) {
            m_prompts.clear();
            m_prompts.push_back(pro.Move);

            switch (currentState) {
            case p->SIdle:
                m_prompts.push_back(pro.Dash);
                m_prompts.push_back(pro.Punch);
                m_prompts.push_back(pro.Ready);
                if (!p->GetIsMaxAmmo())   m_prompts.push_back(pro.Reload);
				if (p->GetIntaractable()) m_prompts.push_back(pro.Interact);
				else if (p->GetPotionCnt() > 0) m_prompts.push_back(pro.Potion);
                break;
            case p->SHave:
                m_prompts.push_back(pro.Shot);
                m_prompts.push_back(pro.Safe);
                m_prompts.push_back(pro.Aim);
                if (!p->GetIsMaxAmmo())   m_prompts.push_back(pro.Reload);
                if (p->GetIntaractable()) m_prompts.push_back(pro.Interact);
                break;
            case p->SAiming:
                m_prompts.push_back(pro.Shot);
                m_prompts.push_back(pro.Hipfire);
                break;
            }
            m_lastState = currentState;
        }
    }
    for (int i = 0; i < m_prompts.size(); i++) {
        m_promptText.Draw(30, 970 - i * 45, 1, 1, 0, m_prompts[i].c_str());
    }
}
