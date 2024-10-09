#pragma once

#include "Base.h"
#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement
	: public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	CScene* Get_CurrScene() { return m_pScene; }
	CONTROLLERID Get_ControllerID() { return m_eController; }
	CComponent* Get_Component(COMPONENTID _eID, const _tchar* _pLayerTag, const _tchar* _pObjTag, const _tchar* _pComponentTag);

	HRESULT	Set_Scene(CScene* _pScene);
	void Set_ControllerID(CONTROLLERID _eController) { m_eController = _eController; }
	_int Get_CountScene() { return m_iCountScene; }

public:
	_int Update_Scene(const _float& _fTimeDelta);
	void LateUpdate_Scene();
	void Render_Scene(LPDIRECT3DDEVICE9 _pGraphicDev);
	void Set_PlayerSkillTimer(const _float& _fTimer) { m_fPlayerSkillTimer = _fTimer; }

public:
	virtual void Free();

private:
	CScene* m_pScene;

	CONTROLLERID m_eController;
	_float m_fPlayerSkillTimer;
	_int m_iCountScene;
};

END