#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CEffect;

END

class CEffectBloodJet : public CGameObject
{
private:
	enum class BLOOD
	{
		BULLKY,
		BULLKY2,
		FAN_SPREAD,
		THIN_SPLATTER,
		WHIP_LIKE,

		BLOOD_END
	};

private:
	explicit CEffectBloodJet(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CEffectBloodJet();

public:
	virtual HRESULT	Ready_GameObject();
	virtual	_int	Update_GameObject(const _float& _fTimeDelta);
	virtual	void	LateUpdate_GameObject();
	virtual	void	Render_GameObject();

public:
	static CEffectBloodJet* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

private:
	virtual void Free();

private:
	HRESULT Add_Component();

private:
	void Set_UVGrid();

public:
	static void OnOperate(void* _pParam);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CEffect* m_pEffectCom;

	_uint m_iTotalFrame[(_uint)BLOOD::BLOOD_END];
	_uint m_iCurFrame;

	_vec2 m_vGrid[(_uint)BLOOD::BLOOD_END];
	_vec3 m_vDerivedPosition[(_uint)BLOOD::BLOOD_END];
	//_vec3 m_vRenderPositoin[3];
	std::array<pair<BLOOD, _vec3>, 3> m_vRender;
	std::array<CTransform*, 3> m_vRenderTransform;
};

