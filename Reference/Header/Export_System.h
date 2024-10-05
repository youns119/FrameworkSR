#pragma once

#include "GraphicDev.h"
#include "TimerManager.h"
#include "FrameManager.h"
#include "FontManager.h"
#include "InputDev.h"
#include "SoundManager.h"

BEGIN(Engine)

// GraphicDev
inline HRESULT Ready_GraphicDev(HWND _hWnd, WINMODE _eMode, const _uint& _iSizeX, const _uint& _iSizeY, CGraphicDev** _ppGraphicDev);
inline void	Render_Begin(D3DXCOLOR _tColor);
inline void	Render_End();

// TimerManager
inline _float Get_TimeDelta(const _tchar* _pTimerTag);
inline _float Get_Elapsed();
inline _float Get_Elapsed_Fixed();
inline _bool Get_Stop();
inline void Set_TimeDelta(const _tchar* _pTimerTag);
inline HRESULT Ready_Timer(const _tchar* _pTimerTag);
inline void Stop_Timer(_bool _bStop);

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
inline _bool Key_Press(_ubyte _byKeyID);
inline _bool Key_Hold(_ubyte _byKeyID);
inline _bool Key_Release(_ubyte _byKeyID);
inline _bool Mouse_Press(MOUSEKEYSTATE _eMouse);
inline _bool Mouse_Hold(MOUSEKEYSTATE _eMouse);
inline _bool Mouse_Release(MOUSEKEYSTATE _eMouse);
inline _long Get_DIMouseMove(MOUSEMOVESTATE _eMouseState);
inline HRESULT Ready_InputDev(HINSTANCE _hInst, HWND _hWnd);
inline void	Update_InputDev(void);

// SoundManager
inline void Ready_SoundManager();
inline void Play_Sound(const _tchar* _pSoundKey, CHANNELID _eID, _float _fVolume);
inline void Play_BGM(const _tchar* _pSoundKey, _float _fVolume);
inline void Play_BGMOnce(const _tchar* _pSoundKey, CHANNELID _eID, _float _fVolume);
inline void Stop_Sound(CHANNELID _eID);
inline void Stop_All();
inline void Set_ChannelVolume(CHANNELID _eID, float _fVolume);

inline void	Release_System();

#include "Export_System.inl"

END