// GraphicDev
HRESULT	Ready_GraphicDev(HWND _hWnd, WINMODE _eMode, const _uint& _iSizeX, const _uint& _iSizeY, CGraphicDev** _ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(_hWnd, _eMode, _iSizeX, _iSizeY, _ppGraphicDev);
}

void Render_Begin(D3DXCOLOR _tColor)
{
	CGraphicDev::GetInstance()->Render_Begin(_tColor);
}
void Render_End()
{
	CGraphicDev::GetInstance()->Render_End();
}

// TimerManager
_float Get_TimeDelta(const _tchar* _pTimerTag)
{
	return CTimerManager::GetInstance()->Get_TimeDelta(_pTimerTag);
}

inline _float Get_Elapsed()
{
	return CTimerManager::GetInstance()->Get_Elapsed();
}

inline _float Get_Elapsed_Fixed()
{
	return CTimerManager::GetInstance()->Get_Elapsed_Fixed();
}

_bool Get_Stop()
{
	return CTimerManager::GetInstance()->Get_Stop();
}

void Set_TimeDelta(const _tchar* _pTimerTag)
{
	CTimerManager::GetInstance()->Set_TimeDelta(_pTimerTag);
}

HRESULT	Ready_Timer(const _tchar* _pTimerTag)
{
	return CTimerManager::GetInstance()->Ready_Timer(_pTimerTag);
}

inline void Stop_Timer(_bool _bStop)
{
	CTimerManager::GetInstance()->Stop_Timer(_bStop);
}

// FrameManager
HRESULT	Ready_Frame(const _tchar* _pFrameTag, const _float& _fCallLimit)
{
	return CFrameManager::GetInstance()->Ready_Frame(_pFrameTag, _fCallLimit);
}

_bool IsPermit_Call(const _tchar* _pFrameTag, const _float& _fTimeDelta)
{
	return CFrameManager::GetInstance()->IsPermit_Call(_pFrameTag, _fTimeDelta);
}

// FontManager
inline HRESULT Ready_Font
(
	LPDIRECT3DDEVICE9 _pGraphicDev,
	const _tchar* _pFontTag,
	const _tchar* _pFontType,
	const _uint& _iWidth,
	const _uint& _iHeight,
	const _uint& _iWeight
)
{
	return CFontManager::GetInstance()->Ready_Font(_pGraphicDev, _pFontTag, _pFontType, _iWidth, _iHeight, _iWeight);
}

inline void	Render_Font
(
	const _tchar* _pFontTag,
	const _tchar* _pString,
	const _vec2* _pPos,
	D3DXCOLOR _tColor
)
{
	CFontManager::GetInstance()->Render_Font(_pFontTag, _pString, _pPos, _tColor);
}

// InputDev
_bool Key_Press(_ubyte _byKeyID)
{
	return CInputDev::GetInstance()->Key_Press(_byKeyID);
}

_bool Key_Hold(_ubyte _byKeyID)
{
	return CInputDev::GetInstance()->Key_Hold(_byKeyID);
}

_bool Key_Release(_ubyte _byKeyID)
{
	return CInputDev::GetInstance()->Key_Release(_byKeyID);
}

_bool Mouse_Press(MOUSEKEYSTATE _eMouse)
{
	return CInputDev::GetInstance()->Mouse_Press(_eMouse);
}

_bool Mouse_Hold(MOUSEKEYSTATE _eMouse)
{
	return CInputDev::GetInstance()->Mouse_Hold(_eMouse);
}

_bool Mouse_Release(MOUSEKEYSTATE _eMouse)
{
	return CInputDev::GetInstance()->Mouse_Release(_eMouse);
}

_long Get_DIMouseMove(MOUSEMOVESTATE _eMouseState)
{
	return CInputDev::GetInstance()->Get_DIMouseMove(_eMouseState);
}

HRESULT Ready_InputDev(HINSTANCE _hInst, HWND _hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(_hInst, _hWnd);
}

void Update_InputDev(void)
{
	CInputDev::GetInstance()->Update_InputDev();
}

// SoundManager
inline void Ready_SoundManager()
{
	CSoundManager::GetInstance()->Ready_SoundManager();
}

inline void Play_Sound(const _tchar* _pSoundKey, CHANNELID _eID, _float _fVolume)
{
	CSoundManager::GetInstance()->Play_Sound(_pSoundKey, _eID, _fVolume);
}

inline void Play_BGM(const _tchar* _pSoundKey, _float _fVolume)
{
	CSoundManager::GetInstance()->Play_BGM(_pSoundKey, _fVolume);
}

inline void Play_BGMOnce(const _tchar* _pSoundKey, CHANNELID _eID, _float _fVolume)
{
	CSoundManager::GetInstance()->Play_BGMOnce(_pSoundKey, _eID, _fVolume);
}

inline void Stop_Sound(CHANNELID _eID)
{
	CSoundManager::GetInstance()->Stop_Sound(_eID);
}

inline void Stop_All()
{
	CSoundManager::GetInstance()->Stop_All();
}

inline void Set_ChannelVolume(CHANNELID _eID, float _fVolume)
{
	CSoundManager::GetInstance()->Set_ChannelVolume(_eID, _fVolume);
}

void Release_System()
{
	CSoundManager::DestroyInstance();
	CInputDev::GetInstance()->DestroyInstance();
	CFontManager::GetInstance()->DestroyInstance();
	CFrameManager::GetInstance()->DestroyInstance();
	CTimerManager::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}