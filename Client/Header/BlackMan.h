#pragma once
#include "Humanoid.h"


class CBlackMan : public CHumanoid
{
public:
    enum SHIELDSTATE { SHIELDSTATE_IDLE, SHIELDSTATE_ATTACK, SHIELDSTATE_ATTACK_DELAY, SHIELDSTATE_END };
private:
    explicit CBlackMan(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CBlackMan();

public:
    static CBlackMan* Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* _pShield);

public:
    virtual HRESULT	Ready_GameObject();

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

