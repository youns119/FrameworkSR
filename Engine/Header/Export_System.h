#pragma once

#include "GraphicDev.h"
#include "TimerManager.h"
#include "FrameManager.h"
#include "FontManager.h"
#include "InputDev.h"

BEGIN(Engine)

// GraphicDev
inline HRESULT Ready_GraphicDev(HWND _hWnd, WINMODE _eMode, const _uint& _iSizeX, const _uint& _iSizeY, CGraphicDev** _ppGraphicDev);
inline void	Render_Begin(D3DXCOLOR _tColor);
inline void	Render_End();

// TimerManager
inline _float Get_TimeDelta(const _tchar* _pTimerTag);
inline void Set_TimeDelta(const _tchar* _pTimerTag);
inline HRESULT Ready_Timer(const _tchar* _pTimerTag);

// FrameManager
inline HRESULT Ready_Frame(const _tchar* _pFrameTag, const _float& _fCallLimit);
inline _bool IsPermit_Call(const _tchar* _pFrameTag, const _float& _fTimeDelta);

// FontManager
inline HRESULT Ready_Font
(
	LPDIRECT3DDEVICE9 _pGraphicDev,
	const _tchar* _pFontTag,
	const _tchar* _pFontType,
	const _uint& _iWidth,
	const _uint& _iHeight,
	const _uint& _iWeight
);
inline void Render_Font
(
	const _tchar* _pFontTag,
	const _tchar* _pString,
	const _vec2* _pPos,
	D3DXCOLOR _tColor
);

// InputDev
inline _byte Get_DIKeyState(_ubyte _byKeyID);
inline _byte Get_DIMouseState(MOUSEKEYSTATE _eMouse);
inline _long Get_DIMouseMove(MOUSEMOVESTATE _eMouseState);
inline HRESULT Ready_InputDev(HINSTANCE _hInst, HWND _hWnd);
inline void	Update_InputDev(void);

inline void	Release_System();

#include "Export_System.inl"

END