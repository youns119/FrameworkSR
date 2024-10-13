#pragma once

#include "UIUnit.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;

END

class CUISkillBase
	: public Engine::CUIUnit
{
public:
	enum class UI_SKILL
	{
		SKILL_BACK,
		SKILL_OUTLINE,
		SKILL_BLACK,
		SKILL_END
	};

private:
	explicit CUISkillBase(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual ~CUISkillBase();

public:
	static CUISkillBase* Create(LPDIRECT3DDEVICE9 _pGraphicDev);

public:
	HRESULT Ready_Unit();
	virtual	_int Update_Unit(const _float& _fTimeDelta);
	virtual	void LateUpdate_Unit();
	virtual	void Render_Unit();

private:
	HRESULT Add_Component();

private:
	void Update_Color(CTexture* _pTexture);

public:
	virtual void Reset();

private:
	virtual void Free();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom[(_uint)UI_SKILL::SKILL_END];
	Engine::CTransform* m_pTransformCom[(_uint)UI_SKILL::SKILL_END];

	_bool m_bCoolDown;
};