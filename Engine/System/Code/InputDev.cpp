#include "InputDev.h"

IMPLEMENT_SINGLETON(CInputDev)

CInputDev::CInputDev(void)
{
	ZeroMemory(&m_tMouseState, sizeof(DIMOUSESTATE));

	for (_int i = 0; i < VK_MAX; i++)
	{
		m_tKeyInfo[i].eKeyState = KEYSTATE::KEY_END;
		m_tKeyInfo[i].bPrevPress = false;
	}

	for (_int i = 0; i < MS_MAX; i++)
	{
		m_tMouseInfo[i].eKeyState = KEYSTATE::KEY_END;
		m_tMouseInfo[i].bPrevPress = false;
	}
}

CInputDev::~CInputDev(void)
{
	Free();
}

HRESULT CInputDev::Ready_InputDev(HINSTANCE _hInst, HWND _hWnd)
{
	// DInput 컴객체를 생성하는 함수
	FAILED_CHECK_RETURN(DirectInput8Create
	(
		_hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL
	), E_FAIL);

	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);
	m_pKeyBoard->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pKeyBoard->Acquire();

	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);
	m_pMouse->SetDataFormat(&c_dfDIMouse);
	m_pMouse->SetCooperativeLevel(_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	m_pMouse->Acquire();

	return S_OK;
}

void CInputDev::Update_InputDev(void)
{
	_byte byKeyState[VK_MAX];

	m_pKeyBoard->GetDeviceState(VK_MAX, byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	// Keyboard
	for (int i = 0; i < VK_MAX; i++)
	{
		if (byKeyState[i] & 0x80)
		{
			if (m_tKeyInfo[i].bPrevPress)
				m_tKeyInfo[i].eKeyState = KEYSTATE::KEY_HOLD;
			else
				m_tKeyInfo[i].eKeyState = KEYSTATE::KEY_PRESS;

			m_tKeyInfo[i].bPrevPress = true;
		}
		else
		{
			if (m_tKeyInfo[i].bPrevPress)
				m_tKeyInfo[i].eKeyState = KEYSTATE::KEY_RELEASE;
			else
				m_tKeyInfo[i].eKeyState = KEYSTATE::KEY_END;

			m_tKeyInfo[i].bPrevPress = false;
		}
	}

	// Mouse
	for (_int i = 0; i < MS_MAX; i++)
	{
		if (m_tMouseState.rgbButtons[i] & 0x80)
		{
			if (m_tMouseInfo[i].bPrevPress)
				m_tMouseInfo[i].eKeyState = KEYSTATE::KEY_HOLD;
			else
				m_tMouseInfo[i].eKeyState = KEYSTATE::KEY_PRESS;

			m_tMouseInfo[i].bPrevPress = true;
		}
		else
		{
			if (m_tMouseInfo[i].bPrevPress)
				m_tMouseInfo[i].eKeyState = KEYSTATE::KEY_RELEASE;
			else
				m_tMouseInfo[i].eKeyState = KEYSTATE::KEY_END;

			m_tMouseInfo[i].bPrevPress = false;
		}
	}
}

void CInputDev::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}