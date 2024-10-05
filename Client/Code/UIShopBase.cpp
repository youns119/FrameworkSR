#include "pch.h"
#include "..\Header\UIShopBase.h"
#include "..\Header\UIShop.h"
#include "Export_Utility.h"

CUIShopBase::CUIShopBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pAnimatorCom(nullptr)
	, m_bStart(false)
	, m_fScaleRatio(0.f)
	, m_fStartTime(0.f)
{
	for (_uint i = 0; i < (_uint)UI_SHOPBASE::SHOPBASE_END; ++i)
		m_pTextureCom[i] = nullptr;

	for (int i = 0; i < 2; i++)
		m_pTransformCom[i] = nullptr;
}

CUIShopBase::~CUIShopBase()
{
}

CUIShopBase* CUIShopBase::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIShopBase* pUIShopBase = new CUIShopBase(_pGraphicDev);

	if (FAILED(pUIShopBase->Ready_Unit()))
	{
		Safe_Release(pUIShopBase);
		MSG_BOX("UIShopBase Create Failed");
		return nullptr;
	}

	return pUIShopBase;
}

HRESULT CUIShopBase::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_bStart = false;
	m_fScaleRatio = 1.f;
	m_fStartTime = 0.f;

	m_pTransformCom[0]->Set_Pos(50.f, -WINCY / 1.5f, 0.f);
	m_pTransformCom[1]->Set_Pos(90.f, 140.f, 0.f);

	m_pTransformCom[0]->Set_Scale(850.f, 850.f, 0.f);
	m_pTransformCom[1]->Set_Scale(310.f, 310.f, 0.f);

	Set_Animation();

	m_bRender = true;

	return S_OK;
}

_int CUIShopBase::Update_Unit(const _float& _fTimeDelta)
{
	if (static_cast<CUIShop*>(m_pOwnerUI)->Get_FinishTime() >= 1.f)
	{
		_vec3 vDir = { 0.f, -1.f, 0.f };

		for (int i = 0; i < 2; i++)
			m_pTransformCom[i]->Move_Pos(&vDir, _fTimeDelta, static_cast<CUIShop*>(m_pOwnerUI)->Get_DownSpeed());

		return Engine::CUIUnit::Update_Unit(_fTimeDelta);
	}

	if (m_fStartTime >= 1.f && m_bStart == false)
	{
		m_bStart = true;
		return 0;
	}

	_vec3 vPos;
	_vec3 vDir = { 0.f, 1.f, 0.f };

	m_pTransformCom[0]->Get_Info(INFO::INFO_POS, &vPos);

	if (vPos.y < -85.f)
		m_pTransformCom[0]->Move_Pos(&vDir, _fTimeDelta, 1400.f);
	else
	{
		m_pTransformCom[0]->Set_Scale(950.f * m_fScaleRatio, 950.f * m_fScaleRatio, 0.f);

		m_fScaleRatio += _fTimeDelta * 1.2f;
		if (m_fScaleRatio >= 1.45f) m_fScaleRatio = 1.45f;
	}

	m_pTransformCom[0]->Get_Info(INFO::INFO_POS, &vPos);

	if (vPos.y > -85.f)
		m_pTransformCom[0]->Set_Pos(50.f, -85.f, 0.f);

	if (m_fStartTime < 2.f) m_fStartTime += _fTimeDelta;

	if (m_pAnimatorCom->GetCurrAnim()->GetFinish())
		m_pAnimatorCom->PlayAnimation(L"Load", true);

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIShopBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIShopBase::Render_Unit()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[0]->Get_WorldMatrix());

	if (m_bStart)
	{
		m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_BASE]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[1]->Get_WorldMatrix());
		m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_TEXT]->Set_Texture();
	}
	else
		m_pAnimatorCom->Render_Animator();

	m_pBufferCom->Render_Buffer();
}

HRESULT CUIShopBase::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_START] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Hand_Start"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Start", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_LOAD] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Hand_Load"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Load", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_BASE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Hand_Base"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Base", pComponent });

	pComponent = m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_TEXT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_Upgrade"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Text_Upgrade", pComponent });

	pComponent = m_pTransformCom[0] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Shop", pComponent });

	pComponent = m_pTransformCom[1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Text", pComponent });

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator_Shop", pComponent });

	return S_OK;
}

void CUIShopBase::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Start", m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_START], 10.f);
	m_pAnimatorCom->CreateAnimation(L"Load", m_pTextureCom[(_uint)UI_SHOPBASE::SHOPBASE_LOAD], 15.f);

	m_pAnimatorCom->PlayAnimation(L"Start", false);
}

void CUIShopBase::Reset()
{
	m_bStart = false;
	m_fScaleRatio = 1.f;
	m_fStartTime = 0.f;

	m_pTransformCom[0]->Set_Pos(50.f, -WINCY / 1.5f, 0.f);
	m_pTransformCom[1]->Set_Pos(90.f, 140.f, 0.f);

	m_pTransformCom[0]->Set_Scale(850.f, 850.f, 0.f);
	m_pTransformCom[1]->Set_Scale(310.f, 310.f, 0.f);

	m_pAnimatorCom->GetCurrAnim()->ResetAnimFrame(0.f);

	m_pAnimatorCom->PlayAnimation(L"Start", false);
	m_pAnimatorCom->GetCurrAnim()->ResetAnimFrame(0.f);
}

void CUIShopBase::Free()
{
	Engine::CUIUnit::Free();
}