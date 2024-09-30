#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCharacter :
    public CGameObject
{
protected:
    explicit CCharacter(LPDIRECT3DDEVICE9 _pGraphicDev);
    explicit CCharacter(const CCharacter& _rhs);
    virtual ~CCharacter();
public:
    virtual	HRESULT	Ready_GameObject();
    virtual	_int Update_GameObject(const _float& _fTimeDelta);
    virtual	void LateUpdate_GameObject();

    virtual void Damaged(const DAMAGED_STATE& _eDamagedState = DAMAGED_STATE::DAMAGED_BODYSHOT, const _float& _fAttackDamage = 0.f) {}
    _bool Get_IsDead() { return m_bIsDead; }

protected:
    _bool m_bIsDead;

protected:
    virtual void Free();
};

END