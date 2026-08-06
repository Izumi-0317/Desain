#pragma once
#include "UI/UIBase.h"

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
};
