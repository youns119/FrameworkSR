#include "pch.h"
#include "..\Header\UISkillBase.h"
#include "..\Header\Player.h"
#include "Export_System.h"
#include "Export_Utility.h"

CUISkillBase::CUISkillBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_bCoolDown(false)
{
	for (_uint i = 0; i < (_uint)UI_SKILL::SKILL_END; i++)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}
}

CUISkillBase::~CUISkillBase()
{
}

CUISkillBase* CUISkillBase::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUISkillBase* pUISkillBase = new CUISkillBase(_pGraphicDev);

	if (FAILED(pUISkillBase->Ready_Unit()))
	{
		Safe_Release(pUISkillBase);
		MSG_BOX("UISkillBase create Failed");
		return nullptr;
	}

	return pUISkillBase;
}

HRESULT CUISkillBase::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom[(_uint)UI_SKILL::SKILL_BACK]->Set_Pos(-WINCX / 2.f + 100.f, -WINCY / 2.f + 100.f, 0.f);
	m_pTransformCom[(_uint)UI_SKILL::SKILL_OUTLINE]->Set_Pos(-WINCX / 2.f + 100.f, -WINCY / 2.f + 100.f, 0.f);
	m_pTransformCom[(_uint)UI_SKILL::SKILL_BLACK]->Set_Pos(-WINCX / 2.f + 100.f, -WINCY / 2.f + 100.f, 0.f);

	m_pTransformCom[(_uint)UI_SKILL::SKILL_BACK]->Set_Scale(70.f, 70.f, 0.f);
	m_pTransformCom[(_uint)UI_SKILL::SKILL_OUTLINE]->Set_Scale(70.f, 70.f, 0.f);
	m_pTransformCom[(_uint)UI_SKILL::SKILL_BLACK]->Set_Scale(70.f, 70.f, 0.f);

	m_bRender = true;

	return S_OK;
}

_int CUISkillBase::Update_Unit(const _float& _fTimeDelta)
{
	m_bCoolDown = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"))->Get_Cool();

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUISkillBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUISkillBase::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SKILL::SKILL_BACK]->Get_WorldMatrix());

	m_pTextureCom[(_uint)UI_SKILL::SKILL_BACK]->Set_Texture();
	m_pBufferCom->Render_Buffer();

	if (!m_bCoolDown)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SKILL::SKILL_OUTLINE]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SKILL::SKILL_OUTLINE]->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
	else
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SKILL::SKILL_OUTLINE]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SKILL::SKILL_OUTLINE]->Set_Texture();
		Update_Color(m_pTextureCom[(_uint)UI_SKILL::SKILL_OUTLINE]);
		m_pBufferCom->Render_Buffer();
	}

	if (!m_bCoolDown)
	{
		DWORD tPreFactor;

		m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &tPreFactor);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_SKILL::SKILL_BLACK]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_SKILL::SKILL_BLACK]->Change_Alpha(0.5f);
		m_pTextureCom[(_uint)UI_SKILL::SKILL_BLACK]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tPreFactor);
	}
}

HRESULT CUISkillBase::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	{
		pComponent = m_pTextureCom[(_uint)UI_SKILL::SKILL_BACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UISkill_Back"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Back", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SKILL::SKILL_OUTLINE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UISkill_Outline"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Outline", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_SKILL::SKILL_BLACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UISkill_Black"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Black", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_SKILL::SKILL_BACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Back", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SKILL::SKILL_OUTLINE] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Outline", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_SKILL::SKILL_BLACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Black", pComponent });
	}

	return S_OK;
}

void CUISkillBase::Update_Color(CTexture* _pTexture)
{
	_float fHue = fmod(Engine::Get_Elapsed() * 40.f, 360.0f);
	_float fSaturation = 1.0f;
	_float fValue = 1.0f;

	_pTexture->Change_TextureColor(fHue, fSaturation, fValue);
}

void CUISkillBase::Reset()
{
}

void CUISkillBase::Free()
{
	Engine::CUIUnit::Free();
}