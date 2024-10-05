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
	void Operate_Effect();
	void Stop_Effect();

public:
	void Set_Repeatable(_bool _bRepeatable) { m_bRepeatable = _bRepeatable; }
	void Set_Visibility(_bool _bIsVisible) { m_bIsVisible = _bIsVisible; }
	void Set_Billboard(_bool _bBillboard) { m_bBillboard = _bBillboard; }
	void Set_LifeTime(_float _fLifeTime) { m_fLifeTime = _fLifeTime; }
	void Set_CallerObject(CGameObject* _pCallerObject) { m_pCallerObject = _pCallerObject; }

	_bool Get_Visibility() { return m_bIsVisible; }
	_float Get_ElapsedPersentage() { return m_fElapsed / m_fLifeTime; }
	_float Get_LifeTime() const { return m_fLifeTime; }
	CGameObject* Get_CallerObject() { return m_pCallerObject; }


public:
	void Reset();

private:
	_float m_fLifeTime;
	_float m_fElapsed;
	_bool m_bIsVisible;
	_bool m_bRepeatable;
	_bool m_bBillboard;

public:
	void Set_CallBack(void (*_pCallBack)(void*)) { m_pCallBack = _pCallBack; }
	void Set_CallBackParam(void* _pParam) { m_pParam = _pParam; }

private:
	void (*m_pCallBack)(void*);
	void* m_pParam;


private:
	Engine::CGameObject* m_pCallerObject;
};

END
