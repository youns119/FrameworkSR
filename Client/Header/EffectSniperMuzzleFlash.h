#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CEffect;
class CParticleSystem;

END

class CEffectSniperMuzzleFlash : public CGameObject
{
private:
	explicit CEffectSniperMuzzleFlash(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectSniperMuzzleFlash();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectSniperMuzzleFlash* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	virtual void Free();

private:
	HRESULT Add_Component();
	void Animate_UV();
	void Set_ParticleParam();


public:
	static void OnOperate(void* _pParam);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CEffect* m_pEffectCom;
	Engine::CParticleSystem* m_pParticleSystemCom;
	Engine::CTexture* m_pParticleTextureCom;

	Engine::CTransform* m_pRightTransformCom;
	Engine::CTransform* m_pLeftTransformCom;

	_uint m_iTotalFrame;
	_uint m_iCurFrame;
};

