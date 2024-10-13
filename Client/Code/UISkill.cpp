#include "pch.h"
#include "..\Header\UISkill.h"
#include "..\Header\UISkillBase.h"

CUISkill::CUISkill(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUISkillBase(nullptr)
{
	m_eUIType = UITYPE::UI_SKILL;
}

CUISkill::~CUISkill()
{
}

CUISkill* CUISkill::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUISkill* pUISkill = new CUISkill(_pGraphicDev);

	if (FAILED(pUISkill->Ready_UI()))
	{
		Safe_Release(pUISkill);
		MSG_BOX("UISkill create Failed");
		return nullptr;
	}

	return pUISkill;
}

HRESULT CUISkill::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUISkill::Update_UI(const _float& _fTimeDelta)
{
	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUISkill::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUISkill::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUISkill::Add_Unit()
{
	m_pUISkillBase = CUISkillBase::Create(m_pGraphicDev);
	m_pUISkillBase->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pUISkillBase);

	return S_OK;
}

void CUISkill::Free()
{
	Engine::CUI::Free();
}