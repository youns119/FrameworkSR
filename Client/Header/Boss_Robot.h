#pragma once
#include "Monster.h"
class CBoss_Robot :
    public CMonster
{
public:
    enum BOSS_ROBOTSTATE { BOSS_SPAWN, BOSS_ATTACK, BOSS_END };

private:
    explicit CBoss_Robot(LPDIRECT3DDEVICE9 _pGraphicDev);
    virtual ~CBoss_Robot();

public:
	static CBoss_Robot* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

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
	virtual void Attack(const _float& _fTimeDelta);
	virtual void Set_Animation();
	void Changing_State(BOSS_ROBOTSTATE _eState) { m_eCurState = _eState; }

private:
	Engine::CTexture* m_pTextureCom[CBoss_Robot::BOSS_END];

	BOSS_ROBOTSTATE m_eCurState;
	BOSS_ROBOTSTATE m_ePreState;

private:
	virtual void Free();
};

