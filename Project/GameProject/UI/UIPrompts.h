#pragma once
#include "UIBase.h"

struct Prompts {
	std::string Move, Dash, Punch, Ready, Shot, Safe, Aim, Hipfire, Reload, Interact, Potion;
};

class UIPrompts :public UIBase {
private:
	CFont m_promptText;
	std::vector<std::string> m_prompts;
	int m_lastState;
	bool m_isPad;		//trueÅ@Pad : falseÅ@Keyboard
public:
	UIPrompts();
	void Update() override;
	void Draw() override;
	void DrawPrompts(const Prompts& pro);
};
