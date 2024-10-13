#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CEffect;
class CTransform;

END

class CEffectBossRobotDie : public CGameObject
{
private:
	explicit CEffectBossRobotDie(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectBossRobotDie();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectBossRobotDie* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	virtual void Free();

private:
	HRESULT Add_Component();

public:
	static void OnOperate(void* _pParam);

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CEffect* m_pEffectCom;


	_float m_fElapsedTime;
};


