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

    virtual void Damaged(const _int& _iEnumNumber = 2, const _float& _fAttackDamage = 0.f) {}

protected:
    virtual void Free();
};

END