#pragma once
//#include "Engine_Define.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CEffect 
	: public CComponent
{
private:
	explicit CEffect();
	explicit CEffect(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CEffect(const CEffect& _rhs);
	virtual ~CEffect();

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 _pGrphicDev);

public:
	HRESULT Ready_Effect();
	virtual _int Update_Component(const _float& _fTimeDelta) override;
	virtual void LateUpdate_Component();

public:
	virtual CEffect* Clone() override;

private:
	virtual void Free() override;


public:
	void Set_Visibility(_bool _bIsVisible) { m_bIsVisible = _bIsVisible; }
	void Set_LifeTime(_float _fLifeTime) { m_fLifeTime = _fLifeTime; }

	_bool Get_Visibility() { return m_bIsVisible; }

private:
	_float m_fLifeTime;
	_float m_fElapsed;
	_bool m_bIsVisible;
};

END
