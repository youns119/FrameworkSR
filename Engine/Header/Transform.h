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
	void Get_WorldMatrix(_matrix* _pWorld) const { *_pWorld = m_matWorld; }
	// ¿¬¿í
	void Get_Info(INFO _eType, _vec3* _pInfo)
	{
		*_pInfo = m_vInfo[(_uint)_eType];
	}
	const _vec3* Get_Scale() { return &m_vScale; }
	const _vec3* Get_Angle() { return &m_vAngle; }

	void Set_WorldMatrix(const _matrix* _pWorld)
	{
		m_matWorld = *_pWorld;
	}
	// ±Ôºó
	void Set_Pos(const _vec3& _vPos) { m_vInfo[(_uint)INFO::INFO_POS] = _vPos; }
	void Set_Pos(_float _fX, _float _fY, _float _fZ)
	{
		m_vInfo[(_uint)INFO::INFO_POS] = { _fX, _fY, _fZ };
	}
	void Set_Scale(const _vec3& _vScale) { m_vScale = _vScale; }
	void Set_Scale(_float _fX, _float _fY, _float _fZ)
	{
		m_vScale = { _fX, _fY, _fZ };
	}
	void Set_Angle(const _vec3& _vAngle) { m_vAngle = _vAngle; }
	void Set_Angle(_float _fX, _float _fY, _float _fZ)
	{
		m_vAngle = { _fX, _fY, _fZ };
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
		m_vInfo[(_uint)INFO::INFO_POS] += *_pDir * _fTimeDelta * _fSpeed;
	}
	void Rotation(ROTATION _eType, const _float& _fAngle)
	{
		*(((_float*)&m_vAngle) + (_uint)_eType) += _fAngle;
	}
	void Chase_Target(const _vec3* _pTargetPos, const _float& _fSpeed);
	const _matrix* Compute_LookAtTarget(const _vec3* _pTargetPos);

public :
	virtual CComponent* Clone();

private :
	virtual void Free();

private :
	_vec3 m_vInfo[(_uint)INFO::INFO_END];
	_vec3 m_vScale;
	_vec3 m_vAngle;
	_matrix	m_matWorld;
};

END