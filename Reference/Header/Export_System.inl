// GraphicDev
HRESULT	Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDev**	ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}

void Render_Begin(D3DXCOLOR Color)
{
	CGraphicDev::GetInstance()->Render_Begin(Color);
}
void Render_End()
{
	CGraphicDev::GetInstance()->Render_End();
}

// TimerManager
_float Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerManager::GetInstance()->Get_TimeDelta(pTimerTag);
}

void Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimerManager::GetInstance()->Set_TimeDelta(pTimerTag);
}

HRESULT	Ready_Timer(const _tchar* pTimerTag)
{
	return CTimerManager::GetInstance()->Ready_Timer(pTimerTag);
}

// FrameManager
HRESULT	Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrameManager::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}

_bool IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameManager::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}

void Release_System()
{
	CFrameManager::GetInstance()->DestroyInstance();
	CTimerManager::GetInstance()->DestroyInstance();
	CGraphicDev::GetInstance()->DestroyInstance();
}