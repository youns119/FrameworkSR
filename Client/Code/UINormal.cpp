#include "pch.h"
#include "..\Header\UINormal.h"
#include "..\Header\UICrossHair.h"
#include "..\Header\Player.h"
#include "Export_Utility.h"

CUINormal::CUINormal(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUI(_pGraphicDev)
	, m_pCrossHair(nullptr)
{
	m_eUIType = UITYPE::UI_NORMAL;
}

CUINormal::~CUINormal()
{
}

CUINormal* CUINormal::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUINormal* pUINormal = new CUINormal(_pGraphicDev);

	if (FAILED(pUINormal->Ready_UI()))
	{
		Safe_Release(pUINormal);
		MSG_BOX("UINormal create Failed");
		return nullptr;
	}

	return pUINormal;
}

HRESULT CUINormal::Ready_UI()
{
	FAILED_CHECK_RETURN(Add_Unit(), E_FAIL);

	return S_OK;
}

_int CUINormal::Update_UI(const _float& _fTimeDelta)
{
	CUICrossHair* pCrossHair = static_cast<CUICrossHair*>(m_pCrossHair);

	if (!Engine::Get_ListUI(UITYPE::UI_INVENTORY)->empty() ||
		!Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
	{
		pCrossHair->Set_Free(true);
		pCrossHair->Set_CrossHair(CUICrossHair::UI_CROSSHAIR::CROSSHAIR_PISTOL);
	}
	else
	{
		pCrossHair->Set_Free(false);

		int iHairNum = static_cast<CPlayer*>(m_pGameObject)->Get_WeaponState();
		pCrossHair->Set_CrossHair((CUICrossHair::UI_CROSSHAIR)iHairNum);
	}

	return Engine::CUI::Update_UI(_fTimeDelta);
}

void CUINormal::LateUpdate_UI()
{
	Engine::CUI::LateUpdate_UI();
}

void CUINormal::Render_UI()
{
	Engine::CUI::Render_UI();
}

HRESULT CUINormal::Add_Unit()
{
	CUIUnit* pUIUnit = nullptr;

	pUIUnit = m_pCrossHair = CUICrossHair::Create(m_pGraphicDev);
	m_vecUIUnit.push_back(pUIUnit);

	return S_OK;
}

void CUINormal::Free()
{
	Engine::CUI::Free();
}