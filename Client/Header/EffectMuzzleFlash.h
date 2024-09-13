#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CTransform;
class CTexture;
class CRcTex;
class CEffect;

END

class CEffectMuzzleFlash : public CGameObject
{
private:
	explicit CEffectMuzzleFlash(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectMuzzleFlash();

public:
	static CEffectMuzzleFlash* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& _fTimeDelta) override;
	void LateUpdate_GameObject() override;
	void Render_GameObject() override;

private:
	HRESULT	Add_Component();
	HRESULT Setup_Material();

private:
	void Free() override;

private:
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CRcTex* m_pBufferCom;
	Engine::CEffect* m_pEffectCom;

private:
	//_float m_fLifeTime;
	//_bool m_bIsVisible;
	//_float m_fElapse;

	// sprite animation component ? 
	_uint m_iTotalFrame;
	_uint m_iCurFrame;


};

