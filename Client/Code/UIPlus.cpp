#include "pch.h"
#include "..\Header\UIPlus.h"
#include "..\Header\UIPlusCenter.h"
#include "..\Header\UIPlusLeft.h"
#include "Export_Utility.h"

CUIPlus::CUIPlus(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pUIPlusCenter(nullptr)
	, m_pUIPlusLeft(nullptr)
	, m_ePlusType(UI_PLUS::PLUS_END)
	, m_fLifeTime(0.f)
	, m_fElapsed(0.f)
	, m_iBlink(0)
{
	m_eUIType = UITYPE::UI_PLUS;
}

CUIPlus::~CUIPlus()
{
}

CUIPlus* CUIPlus::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIPlus* pUIPlus = new CUIPlus(_pGraphicDev);

	if (FAILED(pUIPlus->Ready_UI()))
	{
		Safe_Release(pUIPlus);
		MSG_BOX("UIPlus create Failed");
		return nullptr;
	}

	return pUIPlus;
}

HRESULT CUIPlus::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	m_fLifeTime = 5.f;

	return S_OK;
}

_int CUIPlus::Update_UI(const _float& _fTimeDelta)
{
	m_fElapsed += _fTimeDelta;

	if (m_fLifeTime < m_fElapsed)
	{
		m_fElapsed = 0.f;
		m_bRender = false;
	}
	else if (m_fLifeTime - m_fElapsed <= 1.f && _fTimeDelta != 0.f)
		m_iBlink++;

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUIPlus::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUIPlus::Render_UI()
{
	if (m_iBlink == 5)
	{
		m_iBlink = 0;
		return;
	}

	Engine::CUI::Render_UI();
}

HRESULT CUIPlus::Add_Unit()
{
	CUIUnit* pUIUnit = nullptr;

	pUIUnit = m_pUIPlusCenter = CUIPlusCenter::Create(m_pGraphicDev);
	m_vecUIUnit.push_back(pUIUnit);

	pUIUnit = m_pUIPlusLeft = CUIPlusLeft::Create(m_pGraphicDev);
	m_vecUIUnit.push_back(pUIUnit);

	return S_OK;
}

void CUIPlus::Init(_vec3 _vPos, UI_PLUS _ePlusType)
{
	CUIPlusCenter* pUIPlusCenter = static_cast<CUIPlusCenter*>(m_pUIPlusCenter);
	CUIPlusLeft* pUIPlusLeft = static_cast<CUIPlusLeft*>(m_pUIPlusLeft);

	pUIPlusCenter->Set_Pos(_vPos);
	pUIPlusCenter->Set_Sec((_int)_ePlusType);
	pUIPlusLeft->Set_Index(Engine::Get_ListUI(UITYPE::UI_PLUS)->size());
	pUIPlusLeft->Set_Text((_int)_ePlusType);
}

void CUIPlus::Free()
{
	Engine::CUI::Free();
}