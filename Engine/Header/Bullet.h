#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CBullet
	: public CGameObject
{
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 _pGraphicDev);
	explicit CBullet(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vDir, const _float& _fAttackDamage);
	virtual ~CBullet();

public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

public:
	virtual void Fire_Bullet(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vDir, const _float& _fAttackDamage, const _bool& _bIsBoss) {}
	virtual void Fire_Missile(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage, const _vec3& vCurvePos) {}
	virtual void Fire_Laser(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage) {}
	virtual void Fire_MiniGun(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage) {}
	virtual void Fire_Head(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage, const _vec3& vCurvePos) {}
	virtual void Boss_Sniper_Laser(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos) {}
	virtual _float Get_Linear()const { return m_fLinear; }
protected:
	virtual void Free();

protected:
	_vec3 m_vDir;
	_float m_fAttackDamage;

	_float m_fLinear;
};

END