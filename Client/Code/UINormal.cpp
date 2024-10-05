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
	m_pCrossHair->Set_Render(true);

	if (!Engine::Get_ListUI(UITYPE::UI_INVENTORY)->empty() ||
		!Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
	{
		m_pCrossHair->Set_Free(true);
		m_pCrossHair->Set_CrossHair(CUICrossHair::UI_CROSSHAIR::CROSSHAIR_PISTOL);
	}
	else
	{
		CPlayer* pPlayer = static_cast<CPlayer*>(m_pGameObject);

		m_pCrossHair->Set_Free(false);

		int iHairNum = pPlayer->Get_WeaponState();
		m_pCrossHair->Set_CrossHair((CUICrossHair::UI_CROSSHAIR)iHairNum);

		if (m_pCrossHair->Get_CrossHair() == CUICrossHair::UI_CROSSHAIR::CROSSHAIR_SNIPER)
		{
			int iCurrState = pPlayer->Get_RightCurrState();

			if (iCurrState == 5 || iCurrState == 6)
				m_pCrossHair->Set_Render(false);
			else m_pCrossHair->Set_Render(true);
		}
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
	m_pCrossHair = CUICrossHair::Create(m_pGraphicDev);
	m_pCrossHair->Set_OwnerUI(this);
	m_vecUIUnit.push_back(m_pCrossHair);

	return S_OK;
}

void CUINormal::Free()
{
	Engine::CUI::Free();
}