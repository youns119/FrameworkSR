#pragma once
#include "Humanoid.h"


class CBlackMan : public CHumanoid
{
public:
    enum SHIELDSTATE { SHIELDSTATE_IDLE, SHIELDSTATE_ATTACK, SHIELDSTATE_ATTACK_DELAY, SHIELDSTATE_END };
private:
    explicit CBlackMan(LPDIRECT3DDEVICE9 _pGraphicDev);
    explicit CBlackMan(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos);
    virtual ~CBlackMan();

public:
    static CBlackMan* Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pShield);
    static CBlackMan* Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pShield, _vec3 _vecPos);

public:
    virtual HRESULT	Ready_GameObject();
    virtual _bool Get_Execution(_vec3 _vLook, const _bool& _bIsDo);

private:
    virtual HRESULT	Add_Component();
    virtual void State_Check(); //Jonghan Change
    virtual void Attack(const _float& _fTimeDelta);
    void Set_Shield(CGameObject* _pShield) { m_pShield = _pShield; }
    virtual void Set_Animation();

public:
    virtual void Damaged_By_Player(const DAMAGED_STATE& _eDamagedState = DAMAGED_STATE::DAMAGED_BODYSHOT, const _float& _fAttackDamage = 0.f);

private:
    Engine::CTexture* m_pShieldTextureCom[SHIELDSTATE::SHIELDSTATE_END];

    CGameObject* m_pShield;
    _bool m_bIsShield;

private:
    virtual void Free();
};

