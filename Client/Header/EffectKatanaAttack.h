#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CEffect;
class test;

END

class CEffectKatanaAttack : public CGameObject
{
private:
	enum class TYPE
	{
		DIAGONAL,
		VERTICAL,

		TYPE_END
	};
private:
	explicit CEffectKatanaAttack(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectKatanaAttack();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectKatanaAttack* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	virtual void Free();

private:
	HRESULT Add_Component();

private:
	void Animate_UV(_uint i);

public:
	static void OnOperateFirst(void* _pParam);
	static void OnOperateSecond(void* _pParam);
	static void OnOperateThird(void* _pParam);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pAdditionalTransformCom;
	Engine::CTexture* m_pAdditionalTextureCom;
	Engine::CEffect* m_pEffectCom[3];

	_uint m_iTotalFrame;
	_uint m_iCurFrame;

	_uint m_iAdditionalTotalFrame;
	_uint m_iAdditionalCurFrame;
};

