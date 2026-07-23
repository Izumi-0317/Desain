#include "GL.h"
#include "CInput.h"
#include "Utility.h"

unsigned short		CInput::m_key_code[PAD_MAX][eKeyMax] = { 0 };
unsigned short		CInput::m_pad_code[PAD_MAX][10] = {
	{ XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_Y, XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER, XINPUT_GAMEPAD_BACK, XINPUT_GAMEPAD_START, XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB },
	{ XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_Y, XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER, XINPUT_GAMEPAD_BACK, XINPUT_GAMEPAD_START, XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB },
	{ XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_Y, XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER, XINPUT_GAMEPAD_BACK, XINPUT_GAMEPAD_START, XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB },
	{ XINPUT_GAMEPAD_A, XINPUT_GAMEPAD_B, XINPUT_GAMEPAD_X, XINPUT_GAMEPAD_Y, XINPUT_GAMEPAD_LEFT_SHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER, XINPUT_GAMEPAD_BACK, XINPUT_GAMEPAD_START, XINPUT_GAMEPAD_LEFT_THUMB, XINPUT_GAMEPAD_RIGHT_THUMB },
};
unsigned long CInput::m_key_old[PAD_MAX]={0};
unsigned long CInput::m_key_state[PAD_MAX][CInput::eStateMax]={0};
CVector2D CInput::m_mouse_vec;
int CInput::m_mouse_wheel= 0;
int CInput::m_mouse_wheelOld= 0;
bool CInput::m_mouse_inside = false;
LPDIRECTINPUT8 CInput::m_pDinput=NULL;
LPDIRECTINPUTDEVICE8 CInput::m_pMouse=NULL;
CInput::SPadDevice CInput::m_device[PAD_MAX] = { { 0, }, };
XINPUT_STATE CInput::m_xstate[PAD_MAX];

static inline float toFloat( short i) {
	return (float)i / 0x8000;
};
bool CInput::Init(){

	// 「DirectInput」オブジェクトの作成
	if (FAILED(DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pDinput, NULL)))
		return false;
	
	if (FAILED(m_pDinput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
		return false;
	//利用可能なゲームコントローラーの列挙関数を実行
	if (FAILED(m_pDinput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)CInput::_padCallback, &m_device, DIEDFL_ATTACHEDONLY)))
		return false;


	// データ・フォーマットを設定
	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}
	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_pMouse->SetCooperativeLevel(GL::hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return false;
	}

	ZeroMemory(m_xstate, sizeof(m_xstate));
	return true;


}

//利用可能なジョイスティックを列挙するコールバック関数
bool CALLBACK CInput::_padCallback(const LPDIDEVICEINSTANCE lpddi, LPVOID pvRe)
{

	//	HRESULT ret;
	static int d_num = 0;
	SPadDevice* pd = &((SPadDevice*)pvRe)[d_num];
	LPDIRECTINPUTDEVICE8* ppDevice = &pd->m_pPadDevice;
	//ジョイスティック用にデバイスオブジェクトを作成
	if (m_pDinput->CreateDevice(lpddi->guidInstance, ppDevice, NULL) != DI_OK)
		return DIENUM_CONTINUE;

	(*ppDevice)->QueryInterface(IID_IDirectInputDevice2, (LPVOID*)&ppDevice);

	pd->no = d_num;
	if (!pd->m_pPadDevice) return DIENUM_CONTINUE;
	// デバイスをジョイスティックに設定
	if (FAILED(pd->m_pPadDevice->SetDataFormat(&c_dfDIJoystick2))) 	return DIENUM_CONTINUE;
	// 協調レベルの設定
	if (FAILED(pd->m_pPadDevice->SetCooperativeLevel(GL::hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))  return DIENUM_CONTINUE;
	// デバイスを「取得」する
	pd->m_pPadDevice->Acquire();


	d_num++;
	return (d_num >= PAD_MAX) ? DIENUM_STOP : DIENUM_CONTINUE;
}
void CInput::ClearInstance() {

	if (m_pMouse) m_pMouse->Release();
	if(m_pDinput) m_pDinput->Release();
	m_pDinput = nullptr;
}
void CInput::SetMouseInside(bool f) {
	m_mouse_inside = f;
	UpdateClipCursor(true);
}
void CInput::ShowCursor(bool f)
{
	if (f) {
		while (::ShowCursor(true) < 0);
	} else {
		while (::ShowCursor(false) >= 0);
	}
}
static void convert_dinput_to_xinput(XINPUT_STATE* xiState,DIJOYSTATE2* diState) {
	// 左スティック (DirectInputのX/Y軸)
	xiState->Gamepad.sThumbLX = diState->lX- 32767.5f;
	// DirectInputは一般的に「下方向がプラス」のため、XInput（上方向がプラス）に合わせる場合は反転
	xiState->Gamepad.sThumbLY = -(diState->lY - 32767.5f);

	// 右スティック (DirectInputのZ/Rz軸、あるいはRx/Ry軸の場合もあります)
	xiState->Gamepad.sThumbRX =diState->lZ- 32767.5f;
	xiState->Gamepad.sThumbRY = -(diState->lRz - 32767.5f); // 同様にY軸反転

	// =========================================================================
	// 2. トリガーの変換 (例として特定のボタン、または別軸を割り当てる)
	// DirectInput: ボタン、もしくはスライダー軸に割り当てられている
	// XInput:     0 ? 255 (BYTE)
	// =========================================================================
	// ここでは例として、DirectInputのボタン6と7を左右のトリガー（ON/OFF）として扱う例
	xiState->Gamepad.bLeftTrigger = (diState->rgbButtons[6] & 0x80) ? 255 : 0;
	xiState->Gamepad.bRightTrigger = (diState->rgbButtons[7] & 0x80) ? 255 : 0;

	// =========================================================================
	// 3. 方向キー (POVハット) の変換
	// DirectInput: 北が0、右回りに100倍された度数 (36000で1周)。中心（未入力）は 0xFFFFFFFF
	// XInput:     wButtons のビットフラグ (XINPUT_GAMEPAD_DPAD_xxx)
	// =========================================================================
	if (diState->rgdwPOV[0] != 0xFFFFFFFF)
	{
		DWORD pov = diState->rgdwPOV[0];
		// 上 (315度?45度)
		if (pov >= 31500 || pov <= 4500) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_UP;
		// 右 (45度?135度)
		if (pov >= 4500 && pov <= 13500) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT;
		// 下 (135度?225度)
		if (pov >= 13500 && pov <= 22500) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_DOWN;
		// 左 (225度?315度)
		if (pov >= 22500 && pov <= 31500) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_DPAD_LEFT;
	}

	// =========================================================================
	// 4. 通常ボタンの変換
	// DirectInput: rgbButtons[i] の最上位ビットが1なら押されている
	// XInput:     wButtons の各対応ビットフラグを立てる
	// =========================================================================
	// 以下は一般的な「DirectInputのボタン番号」から「Xboxコントローラー配置」へのマッピング例です
	// ※使用するデバイス（DualShock等）やデバイスドライバによって番号がズレるため調整が必要です
	if (diState->rgbButtons[0] & 0x80) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_A;
	if (diState->rgbButtons[1] & 0x80) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_B;
	if (diState->rgbButtons[2] & 0x80) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_X;
	if (diState->rgbButtons[3] & 0x80) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_Y;

	if (diState->rgbButtons[4] & 0x80) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_LEFT_SHOULDER;
	if (diState->rgbButtons[5] & 0x80) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_RIGHT_SHOULDER;

	if (diState->rgbButtons[6] & 0x80) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_LEFT_THUMB;
	if (diState->rgbButtons[7] & 0x80) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_RIGHT_THUMB;

	if (diState->rgbButtons[8] & 0x80) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_BACK;
	if (diState->rgbButtons[9] & 0x80) xiState->Gamepad.wButtons |= XINPUT_GAMEPAD_START;

}

XINPUT_STATE* CInput::GetPadData(int no)
{

	if (no >= PAD_MAX) return nullptr;
	if (XInputGetState(no, &m_xstate[no]) == ERROR_SUCCESS) {
		return &m_xstate[no];
	} else if(m_device[no].m_pPadDevice){
		auto pd = &m_device[no].js;
		HRESULT hr = m_device[no].m_pPadDevice->Acquire();
		if ((hr == DI_OK) || (hr == S_FALSE)) {
			m_device[no].m_pPadDevice->GetDeviceState(sizeof(DIJOYSTATE2), pd);
			convert_dinput_to_xinput(&m_xstate[no], pd);
			return &m_xstate[no];
		}
	}
	return nullptr;
}
CVector2D CInput::GetRStick(int no)
{
	if (no >= PAD_MAX) return CVector2D::zero;


	// コントローラーが接続されているか確認
	if (auto state = GetPadData(no)) {
		// XInputのスティック値は -32768 ? 32767 の範囲（Y軸は上がプラス）
		float x = toFloat(state->Gamepad.sThumbRX);
		float y = toFloat(state->Gamepad.sThumbRY);

		// 必要に応じてデッドゾーン（XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE）の処理をここに入れます
		return CVector2D(x, y);
	}
	return CVector2D::zero;
}
CVector2D CInput::GetLStick(int no)
{

	if (no >= PAD_MAX) return CVector2D::zero;

	if (auto state = GetPadData(no)) {
		float x = toFloat(state->Gamepad.sThumbLX);
		float y = toFloat(state->Gamepad.sThumbLY);

		// 必要に応じてデッドゾーン（XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE）の処理をここに入れます
		return CVector2D(x, y);
	}
	return CVector2D::zero;
}
float CInput::GetLeftTrigger(int no)
{
	if (auto state = GetPadData(no))
		return state->Gamepad.bLeftTrigger / 255.0f;
	return 0.0f;
		
}
float CInput::GetRightTrigger(int no)
{
	if (auto state = GetPadData(no))
		return state->Gamepad.bRightTrigger / 255.0f;
	return 0.0f;
}
void CInput::Update() {
	for (int i = 0; i < PAD_MAX; i++) {
		unsigned long key = 0;
		unsigned long state = 0;
		for (int j = 0; j < eKeyMax; j++) {
			if (::GetKeyState(m_key_code[i][j]) & 0x80) {
				key |= 1 << j;
			}
		}

		// XInputによるコントローラー入力のチェック
		XINPUT_STATE *xstate;

		// 最大4台までサポート（XInputの仕様上、PAD_MAXが4より大きい場合は制限されます）
		if (i < 4 && (xstate = GetPadData(i))!=nullptr) {

			// 1. 十字キー（DPAD）の処理
			if (xstate->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)    key |= 1 << eUp;
			if (xstate->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)  key |= 1 << eDown;
			if (xstate->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)  key |= 1 << eLeft;
			if (xstate->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) key |= 1 << eRight;

			// 2. 通常ボタンの処理（eButton10までのボタンをチェック）
			for (int j = eButton1; j <= eButton10; j++) {
				// m_pad_codeに設定したXINPUT_GAMEPAD_?ビットマスクで判定
				if (xstate->Gamepad.wButtons & m_pad_code[i][j]) {
					key |= 1 << j;
				}
			}
		}
		m_key_state[i][ePush] = (~m_key_old[i]) & key;
		m_key_state[i][eHold] = m_key_old[i] & key;
		m_key_state[i][ePull] = m_key_old[i] & (~key);
		m_key_state[i][eFree] = (~m_key_old[i]) & (~key);
		m_key_old[i] = key;
	}
	
	DIMOUSESTATE state;
	HRESULT hr = m_pMouse->Poll();
	if (FAILED(hr))
	{
		hr = m_pMouse->Acquire();
		while (hr == DIERR_INPUTLOST)
			hr = m_pMouse->Acquire();
	}else
	if (!FAILED(m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &state))) {
		m_mouse_vec = CVector2D((float)state.lX, (float)state.lY);
	}


	m_mouse_wheelOld = m_mouse_wheel;
	m_mouse_wheel = 0;
	
}

 void CInput::UpdateClipCursor(int f) {
	 if (f && m_mouse_inside) {
		 static RECT rect;
		 rect = GL::window_rect;
		 rect.bottom -= 32;
		 ClipCursor(&rect);
	 } else {
		 ClipCursor(NULL);
	 }

}