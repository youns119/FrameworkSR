#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "Define.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Logo.h"

class CMainApp 
	: public CBase
{
private :
	explicit CMainApp();
	virtual ~CMainApp();

public :
	static CMainApp* Create();

public :
	HRESULT	Ready_MainApp();
	int	Update_MainApp(const float& fTimeDelta);
	void LateUpdate_MainApp();
	void Render_MainApp();

private :
	HRESULT	SetUp_DefaultSetting(LPDIRECT3DDEVICE9* ppGraphicDev);
	HRESULT	Ready_Scene(LPDIRECT3DDEVICE9 pGraphicDev, Engine::CManagement** ppManagement);

private :
	virtual void Free();

private :
	bool SetUp();

private :
	CGraphicDev* m_pDeviceClass;
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	CManagement* m_pManagementClass;
};