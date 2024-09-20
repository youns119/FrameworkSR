#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev
	: public CBase
{
	DECLARE_SINGLETON(CInputDev)

public:
	struct KeyInfo
	{
		KEYSTATE eKeyState;
		bool bPrevPress;
	};

private:
	explicit CInputDev(void);
	virtual ~CInputDev(void);

public:
	_bool Key_Press(_ubyte _byKeyID)
	{
		if (m_tKeyInfo[_byKeyID].eKeyState == KEYSTATE::KEY_HOLD)
		{
			int i = 0;
		}

		return m_tKeyInfo[_byKeyID].eKeyState == KEYSTATE::KEY_PRESS;
	}
	_bool Key_Hold(_ubyte _byKeyID) { return m_tKeyInfo[_byKeyID].eKeyState == KEYSTATE::KEY_HOLD; }
	_bool Key_Release(_ubyte _byKeyID) { return m_tKeyInfo[_byKeyID].eKeyState == KEYSTATE::KEY_RELEASE; }

	_bool Mouse_Press(MOUSEKEYSTATE _eMouse) { return m_tMouseInfo[(_uint)_eMouse].eKeyState == KEYSTATE::KEY_PRESS; }
	_bool Mouse_Hold(MOUSEKEYSTATE _eMouse) { return m_tMouseInfo[(_uint)_eMouse].eKeyState == KEYSTATE::KEY_HOLD; }
	_bool Mouse_Release(MOUSEKEYSTATE _eMouse) { return m_tMouseInfo[(_uint)_eMouse].eKeyState == KEYSTATE::KEY_RELEASE; }

	_long Get_DIMouseMove(MOUSEMOVESTATE _eMouseState) { return *(((_long*)&m_tMouseState) + (_uint)_eMouseState); }

public:
	HRESULT Ready_InputDev(HINSTANCE _hInst, HWND _hWnd);
	void Update_InputDev(void);

public:
	virtual void Free(void);

private:
	LPDIRECTINPUT8 m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8 m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8 m_pMouse = nullptr;

private:
	KeyInfo m_tKeyInfo[VK_MAX];
	KeyInfo m_tMouseInfo[MS_MAX];

	DIMOUSESTATE m_tMouseState;
};

END

#endif // InputDev_h__