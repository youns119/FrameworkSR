#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform 
	: public CComponent
{
private :
	explicit CTransform();
	explicit CTransform(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CTransform(const CTransform& _rhs);
	virtual ~CTransform();

public :
	const _matrix* Get_WorldMatrix() { return &m_matWorld; }
	void Get_Info(INFO _eType, _vec3* _pInfo)
	{
		memcpy(_pInfo, &m_matWorld.m[(UINT)_eType][0], sizeof(_vec3));
	}

	void Set_Pos(_float _fX, _float _fY, _float _fZ)
	{
		m_vInfo[(UINT)INFO::INFO_POS] = { _fX, _fY, _fZ };
	}

public :
	static CTransform* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public :
	HRESULT	Ready_Transform();
	virtual _int Update_Component(const _float& _fTimeDelta);
	virtual void LateUpdate_Component();

public :
	void Move_Pos(const _vec3* _pDir, const _float& _fTimeDelta, const _float& _fSpeed)
	{
		m_vInfo[(UINT)INFO::INFO_POS] += *_pDir * _fTimeDelta * _fSpeed;
	}

	void Rotation(ROTATION _eType, const _float& _fAngle)
	{
		*(((_float*)&m_vAngle) + (UINT)_eType) += _fAngle;
	}

public :
	virtual CComponent* Clone();

private :
	virtual void Free();

private :
	_vec3 m_vInfo[(UINT)INFO::INFO_END];
	_vec3 m_vScale;
	_vec3 m_vAngle;
	_matrix	m_matWorld;
};

END