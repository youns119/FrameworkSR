#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;

END

class CEffectPool : public CGameObject
{
private:
	explicit CEffectPool(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectPool();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectPool* Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* (*_fnCreate)(LPDIRECT3DDEVICE9));

private:
	virtual void Free();

	HRESULT Add_Component();

public:
	void Operate();

	void Set_CreateFunction(CGameObject* (*_fnCreate)(LPDIRECT3DDEVICE9)) { m_fnCreate = _fnCreate; }

public:
	void Set_CallerObject(CGameObject* _pCaller) { m_pCallerObject = _pCaller; }

private:
	Engine::CTransform* m_pTransformCom;

	vector<CGameObject*> m_vecObject;

	CGameObject* (*m_fnCreate)(LPDIRECT3DDEVICE9);

	Engine::CGameObject* m_pCallerObject;
};

