#pragma once
#include "Base.h"
#include "../Reference/Header/Export_Utility.h"
#include "../Reference/Header/Export_System.h"

class CMainSetting : public CBase
{

private:
	explicit CMainSetting();
	virtual ~CMainSetting();

public:
	static CMainSetting* Create(HWND _hWnd);

public:
	HRESULT	Ready_MainSetting(HWND _hWnd);
	int		Update_MainSetting(const float& _fTimeDelta);
	void	LateUpdate_MainSetting();
	void	Render_MainSetting();

private:
	HRESULT	SetUp_DefaultSetting(LPDIRECT3DDEVICE9* _ppGraphicDev);
	HRESULT	Ready_Scene(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::CManagement** _ppManagement);

private:
	virtual void Free();

private:
	CGraphicDev* m_pDeviceClass;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
	HWND m_hWnd;
	CManagement* m_pManagementClass;
};

