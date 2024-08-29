#ifndef InputDev_h__
#define InputDev_h__

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInputDev 
	: public CBase
{
	DECLARE_SINGLETON(CInputDev)

private :
	explicit CInputDev(void);
	virtual ~CInputDev(void);

public :
	_byte Get_DIKeyState(_ubyte _byKeyID)
	{
		return m_byKeyState[_byKeyID];
	}
	_byte Get_DIMouseState(MOUSEKEYSTATE _eMouse)
	{
		return m_tMouseState.rgbButtons[(_uint)_eMouse];
	}
	// 현재 마우스의 특정 축 좌표를 반환
	_long Get_DIMouseMove(MOUSEMOVESTATE _eMouseState)
	{
		return *(((_long*)&m_tMouseState) + (_uint)_eMouseState);
	}

public :
	HRESULT Ready_InputDev(HINSTANCE _hInst, HWND _hWnd);
	void Update_InputDev(void);

public :
	virtual void Free(void);

private :
	LPDIRECTINPUT8 m_pInputSDK = nullptr;

private :
	LPDIRECTINPUTDEVICE8 m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8 m_pMouse = nullptr;

private :
	_byte m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수
	DIMOUSESTATE m_tMouseState;
};

END

#endif // InputDev_h__