#pragma once
#include "Monster.h"
class CBoss_Humanoid :
    public CMonster
{
public:
    enum BOSS_HUMANOIDSTATE {BOSS_SPAWN, BOSS_ATTACK, BOSS_END};

private:
    explicit CBoss_Humanoid(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CBoss_Humanoid();

public:
	static CBoss_Humanoid* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	virtual HRESULT	Ready_GameObject();
	//virtual _int Update_GameObject(const _float& _fTimeDelta);
	virtual void LateUpdate_GameObject();
	virtual void Render_GameObject();

public:
	virtual void Change_State() ; //Jonghan Change

private:
	virtual HRESULT	Add_Component();
	virtual void State_Check(); //Jonghan Change
	virtual void Attack();

private:
	Engine::CTexture* m_pTextureCom[CBoss_Humanoid::BOSS_END];

	BOSS_HUMANOIDSTATE m_eCurState;
	BOSS_HUMANOIDSTATE m_ePreState;

private:
	virtual void Free();
};

