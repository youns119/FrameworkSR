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

void Set_TimeDelta(const _tchar* _pTimerTag)
{
	CTimerManager::GetInstance()->Set_TimeDelta(_pTimerTag);
}

HRESULT	Ready_Timer(const _tchar* _pTimerTag)
{
	return CTimerManager::GetInstance()->Ready_Timer(_pTimerTag);
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

void Release_System()
{
	CFrameManager::GetInstance()->DestroyInstance();
	CTimerManager::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}