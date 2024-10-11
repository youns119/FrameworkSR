#include "pch.h"
#include "..\Header\UIPhoneBase.h"
#include "..\Header\Player.h"
#include "Export_System.h"
#include "Export_Utility.h"

CUIPhoneBase::CUIPhoneBase(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CUIUnit(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformTexture(nullptr)
	, m_pTexture(nullptr)
	, m_pTextureSurface(nullptr)
	, m_pPreSurface(nullptr)
	, m_fTick(0.f)
	, m_fMaxHP(0.f)
	, m_fTimerHP(0.f)
	, m_tColor{}
	, m_eModeratorFace(MODERATOR_FACE::MODERATOR_END)
{
	for (_uint i = 0; i < (_uint)UI_PHONE::PHONE_END; ++i)
	{
		m_pTextureCom[i] = nullptr;
		m_pTransformCom[i] = nullptr;
	}

	for (_int i = 0; i < 2; i++)
		m_pAnimatorCom[i] = nullptr;

	for (_int i = 0; i < 2; i++)
		m_iDigit[i] = 0;
}

CUIPhoneBase::~CUIPhoneBase()
{
}

CUIPhoneBase* CUIPhoneBase::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CUIPhoneBase* pUIPhoneBase = new CUIPhoneBase(_pGraphicDev);

	if (FAILED(pUIPhoneBase->Ready_Unit()))
	{
		Safe_Release(pUIPhoneBase);
		MSG_BOX("UIPhoneBase create Failed");
		return nullptr;
	}

	return pUIPhoneBase;
}

HRESULT CUIPhoneBase::Ready_Unit()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_fMaxHP = 20.f;
	m_tColor = D3DCOLOR_XRGB(0, 255, 0);

	m_pTransformCom[(_uint)UI_PHONE::PHONE_MODERATOR_NORMAL]->Set_Pos(210.f, 200.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_UPPER]->Set_Pos(210.f, 255.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK1]->Set_Pos(210.f, 253.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_BACK]->Set_Pos(210.f, 100.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK2]->Set_Pos(210.f, 100.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK3]->Set_Pos(210.f, 100.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_CARD]->Set_Pos(210.f, 100.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_DIGIT1]->Set_Pos(185.f, 100.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_DIGIT2]->Set_Pos(235.f, 100.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_INT]->Set_Pos(210.f, 150.f, 0.f);
	m_pTransformTexture->Set_Pos(0.f, 0.f, 0.f);

	m_pTransformCom[(_uint)UI_PHONE::PHONE_MODERATOR_NORMAL]->Set_Scale(70.f, 70.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_UPPER]->Set_Scale(74.f, 74.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK1]->Set_Scale(65.f, 10.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_BACK]->Set_Scale(65.f, 70.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK2]->Set_Scale(65.f, 70.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK3]->Set_Scale(65.f, 70.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_CARD]->Set_Scale(70.f, 70.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_DIGIT1]->Set_Scale(45.f, 45.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_DIGIT2]->Set_Scale(45.f, 45.f, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_INT]->Set_Scale(70.f, 250.f, 0.f);
	m_pTransformTexture->Set_Scale(WINCX / 2.f, WINCY / 2.f, 0.f);

	m_pGraphicDev->CreateTexture
	(
		WINCX, WINCY,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pTexture,
		NULL
	);

	m_pTexture->GetSurfaceLevel(0, &m_pTextureSurface);
	m_pGraphicDev->GetRenderTarget(0, &m_pPreSurface);

	m_bRender = true;

	Set_Animation();

	return S_OK;
}

_int CUIPhoneBase::Update_Unit(const _float& _fTimeDelta)
{
	CTransform* pTransform = static_cast<CTransform*>(Engine::Get_CurrScene()->Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Player", L"Player", L"Com_Left_Transform"));

	_vec3 vPos, vAngle;

	pTransform->Get_Info(INFO::INFO_POS, &vPos);
	vAngle = *pTransform->Get_Angle();

	vPos.z = 0.f;

	m_pTransformTexture->Set_Pos(vPos);
	m_pTransformTexture->Set_Angle(vAngle);
	m_pTransformTexture->Rotation(ROTATION::ROT_Z, 0.62f);

	m_fTick += _fTimeDelta;

	Update_Time();

	return Engine::CUIUnit::Update_Unit(_fTimeDelta);
}

void CUIPhoneBase::LateUpdate_Unit()
{
	Engine::CUIUnit::LateUpdate_Unit();
}

void CUIPhoneBase::Render_Unit()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));

	if ((pPlayer->Get_LeftCurrState() >= 1 && pPlayer->Get_LeftCurrState() <= 5) ||
		!pPlayer->Get_LeftUse() ||
		!Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
		return;

	{
		m_pGraphicDev->GetRenderTarget(0, &m_pPreSurface);
		m_pGraphicDev->SetRenderTarget(0, m_pTextureSurface);
		m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PHONE::PHONE_MODERATOR_NORMAL]->Get_WorldMatrix());

		m_pAnimatorCom[0]->Render_Animator();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK1]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_PHONE::PHONE_RGBBACK1]->Set_Texture();
		Update_Color();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PHONE::PHONE_UPPER]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_PHONE::PHONE_UPPER]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PHONE::PHONE_BACK]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_PHONE::PHONE_BACK]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK2]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_PHONE::PHONE_RGBBACK2]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		if (m_fTick >= 0.3f)
		{
			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK3]->Get_WorldMatrix());

			m_pTextureCom[(_uint)UI_PHONE::PHONE_RGBBACK3]->Set_Texture();
			m_pBufferCom->Render_Buffer();

			if (m_fTick >= 0.6f) m_fTick = 0.f;
		}

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PHONE::PHONE_CARD]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_PHONE::PHONE_CARD]->Set_Texture();
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PHONE::PHONE_DIGIT1]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_PHONE::PHONE_DIGIT1]->Set_Texture(m_iDigit[0]);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PHONE::PHONE_DIGIT2]->Get_WorldMatrix());

		m_pTextureCom[(_uint)UI_PHONE::PHONE_DIGIT2]->Set_Texture(m_iDigit[1]);
		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom[(_uint)UI_PHONE::PHONE_INT]->Get_WorldMatrix());

		m_pAnimatorCom[1]->Render_Animator();
		m_pBufferCom->Render_Buffer();

		{
			m_pGraphicDev->SetRenderTarget(0, m_pPreSurface);

			m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformTexture->Get_WorldMatrix());

			m_pGraphicDev->SetTexture(0, m_pTexture);
			m_pBufferCom->Render_Buffer();
		}
	}
}

HRESULT CUIPhoneBase::Add_Component()
{
	CComponent* pComponent = NULL;

	{
		pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_RGBBACK1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPhone_RGBBack"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_RGBBack1", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_UPPER] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPhone_Upper"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Upper", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_MODERATOR_NORMAL] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPhone_Moderator_Normal"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Moderator_Normal", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_MODERATOR_HURT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPhone_Moderator_Hurt"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Moderator_Hurt", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_MODERATOR_HURTBAD] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPhone_Moderator_HurtBad"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Moderator_HurtBad", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_BACK] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPhone_Back"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Back", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_RGBBACK2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPhone_RGBBack"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_RGBBack2", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_RGBBACK3] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPhone_RGBBack"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_RGBBack3", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_CARD] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIPhone_Card"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Card", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_DIGIT1] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_DBlack"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit1", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_DIGIT2] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIText_DBlack"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Digit2", pComponent });

		pComponent = m_pTextureCom[(_uint)UI_PHONE::PHONE_INT] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_UIShop_Buzz"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture_Int", pComponent });
	}

	{
		pComponent = m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_RGBBack1", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_PHONE::PHONE_UPPER] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Upper", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_PHONE::PHONE_MODERATOR_NORMAL] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Moderator", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_PHONE::PHONE_BACK] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Back", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_RGBBack2", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK3] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_RGBBack3", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_PHONE::PHONE_CARD] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Card", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_PHONE::PHONE_DIGIT1] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit1", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_PHONE::PHONE_DIGIT2] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Digit2", pComponent });

		pComponent = m_pTransformCom[(_uint)UI_PHONE::PHONE_INT] = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Int", pComponent });

		pComponent = m_pTransformTexture = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform_Texture", pComponent });
	}

	pComponent = m_pAnimatorCom[0] = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator_Moderator", pComponent });

	pComponent = m_pAnimatorCom[1] = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator_Int", pComponent });

	return S_OK;
}

void CUIPhoneBase::Set_Animation()
{
	m_pAnimatorCom[0]->CreateAnimation(L"Moderator_Normal", m_pTextureCom[(_uint)UI_PHONE::PHONE_MODERATOR_NORMAL], 1.f);
	m_pAnimatorCom[0]->CreateAnimation(L"Moderator_Hurt", m_pTextureCom[(_uint)UI_PHONE::PHONE_MODERATOR_HURT], 1.f);
	m_pAnimatorCom[0]->CreateAnimation(L"Moderator_HurtBad", m_pTextureCom[(_uint)UI_PHONE::PHONE_MODERATOR_HURTBAD], 1.f);
	m_pAnimatorCom[0]->PlayAnimation(L"Moderator_Normal", true);

	m_pAnimatorCom[1]->CreateAnimation(L"Int", m_pTextureCom[(_uint)UI_PHONE::PHONE_INT], 10.f);
	m_pAnimatorCom[1]->PlayAnimation(L"Int", true);
}

void CUIPhoneBase::Update_Time()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));

	m_fTimerHP = pPlayer->Get_TimerHP();

	m_iDigit[0] = (int)m_fTimerHP / 10;
	m_iDigit[1] = (int)m_fTimerHP % 10;

	_float fRatio = m_fTimerHP / m_fMaxHP;

	if (fRatio > 2.f / 3.f && m_eModeratorFace != MODERATOR_FACE::MODERATOR_NORMAL)
	{
		m_pAnimatorCom[0]->PlayAnimation(L"Moderator_Normal", true);
		m_eModeratorFace = MODERATOR_FACE::MODERATOR_NORMAL;
	}
	else if ((fRatio > 1.f / 3.f && fRatio <= 2.f / 3.f) && m_eModeratorFace != MODERATOR_FACE::MODERATOR_HURT)
	{
		m_pAnimatorCom[0]->PlayAnimation(L"Moderator_Hurt", true);
		m_eModeratorFace = MODERATOR_FACE::MODERATOR_HURT;
	}
	else if (fRatio <= 1.f / 3.f && m_eModeratorFace != MODERATOR_FACE::MODERATOR_HURTBAD)
	{
		m_pAnimatorCom[0]->PlayAnimation(L"Moderator_HurtBad", true);
		m_eModeratorFace = MODERATOR_FACE::MODERATOR_HURTBAD;
	}

	m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK2]->Set_Scale(65.f, 70.f * fRatio, 0.f);
	m_pTransformCom[(_uint)UI_PHONE::PHONE_RGBBACK2]->Set_Pos(210.f, 30.f + 70.f * fRatio, 0.f);
}

void CUIPhoneBase::Update_Color()
{
	_float fRatio = 255.f * m_fTimerHP / m_fMaxHP;

	m_tColor = D3DCOLOR_XRGB((int)(255 - fRatio), (int)fRatio, 0);

	m_pTextureCom[(_uint)UI_PHONE::PHONE_RGBBACK1]->Change_TextureColor(m_tColor);
}

void CUIPhoneBase::Free()
{
	m_pTextureSurface->Release();
	m_pPreSurface->Release();
	m_pTexture->Release();

	Engine::CUIUnit::Free();
}