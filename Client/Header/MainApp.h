#pragma once

#include "Base.h"
#include "Engine_Define.h"
#include "Define.h"
#include "Export_System.h"
#include "Export_Utility.h"
#include "Logo.h"
#include <fstream>

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
	int	Update_MainApp(const float& _fTimeDelta);
	void LateUpdate_MainApp();
	void Render_MainApp();

private :
	HRESULT	SetUp_DefaultSetting(LPDIRECT3DDEVICE9* _ppGraphicDev);
	HRESULT SetUp_ImGuiSetting(LPDIRECT3DDEVICE9* _ppGraphicDev); // imgui 기본 세팅 
	HRESULT	Ready_Scene(LPDIRECT3DDEVICE9 _pGraphicDev, Engine::CManagement** _ppManagement);

	void LoadFont();//imgui 폰트적용(한글)

private :
	virtual void Free();

private :
	CGraphicDev* m_pDeviceClass;
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	CManagement* m_pManagementClass;
};