#include "..\..\Header\UI.h"

CUI::CUI(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
	, m_pGameObject(nullptr)
	, m_eUIType(UITYPE::UI_END)
	, m_bRender(false)
	, m_fViewZ(0.f)
{
	m_vecUIUnit.clear();

	m_pGraphicDev->AddRef();
}

CUI::CUI(const CUI& _rhs)
	: m_pGraphicDev(_rhs.m_pGraphicDev)
	, m_vecUIUnit(_rhs.m_vecUIUnit)
	, m_pGameObject(_rhs.m_pGameObject)
	, m_eUIType(_rhs.m_eUIType)
	, m_bRender(false)
	, m_fViewZ(0.f)
{
	m_pGraphicDev->AddRef();
}

CUI::~CUI()
{
}

_int CUI::Update_UI(const _float& _fTimeDelta)
{
	for (auto& pUnit : m_vecUIUnit)
	{
		if (!pUnit->Get_Render()) continue;
		else pUnit->Update_Unit(_fTimeDelta);
	}

	return 0;
}

void CUI::LateUpdate_UI()
{
	for (auto& pUnit : m_vecUIUnit)
	{
		if (!pUnit->Get_Render()) continue;
		else pUnit->LateUpdate_Unit();
	}
}

void CUI::Render_UI()
{
	sort(m_vecUIUnit.begin(), m_vecUIUnit.end(),
		[](CUIUnit* pDst, CUIUnit* pSrc)->bool
		{
			return pDst->Get_ViewZ() > pSrc->Get_ViewZ();
		}
	);

	for (auto& pUnit : m_vecUIUnit)
	{
		if (!pUnit->Get_Render()) continue;
		else pUnit->Render_Unit();
	}
}

void CUI::Reset()
{
	for (auto& pUnit : m_vecUIUnit)
		pUnit->Reset();
}

void CUI::Free()
{
	for_each(m_vecUIUnit.begin(), m_vecUIUnit.end(), CDeleteObj());
	m_vecUIUnit.clear();

	Safe_Release(m_pGraphicDev);
}