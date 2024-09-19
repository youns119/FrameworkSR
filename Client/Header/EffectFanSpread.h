#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CEffect;

END

class CEffectFanSpread : public CGameObject
{
private:
	explicit CEffectFanSpread(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectFanSpread();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectFanSpread* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	virtual void Free();

private:
	HRESULT Add_Component();

private:
	void Animate_UV();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CEffect* m_pEffectCom;

	_uint m_iTotalFrame;
	_uint m_iCurFrame;
};

