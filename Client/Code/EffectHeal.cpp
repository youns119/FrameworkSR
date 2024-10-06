#include "pch.h"
#include "../Header/EffectHeal.h"
#include "Export_Utility.h"

CEffectHeal::CEffectHeal(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
}

CEffectHeal::~CEffectHeal()
{
}

HRESULT CEffectHeal::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fViewZ = 0.1f;
	Set_ParticleParam();

	m_pTransformCom->Set_Pos(0.f, 0.f, m_fViewZ);

	m_pEffectCom->Set_LifeTime(2.f);
	m_pEffectCom->Set_Repeatable(FALSE);
	m_pEffectCom->Set_Visibility(FALSE);
	m_pEffectCom->Set_CallBack(OnOperate);
	m_pEffectCom->Set_CallBackParam(this);

	return S_OK;
}

_int CEffectHeal::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_pEffectCom->Get_Visibility())
		return 0;
	Engine::Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectHeal::LateUpdate_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;


	Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectHeal::Render_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture();
	m_pParticleSystem->Render_Parcitle();
}

CEffectHeal* CEffectHeal::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CEffectHeal* pObj = new CEffectHeal(_pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		Safe_Release(pObj);
		MSG_BOX("HealEffect Create Failed");
		return nullptr;
	}
	return pObj;
}

void CEffectHeal::Free()
{
	Engine::CGameObject::Free();
}

HRESULT CEffectHeal::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LIKES"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pParticleSystem = static_cast<CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem", pComponent });

	pComponent = m_pEffectCom = static_cast<CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });


	return S_OK;
}

void CEffectHeal::Set_ParticleParam()
{
	CParticleSystem::PARAM tParticleParam;
	ZeroMemory(&tParticleParam, sizeof(CParticleSystem::PARAM));
	tParticleParam.tInit.tHexahedron.tStartBoundary.vMin = { -WINCX * 0.5f, -WINCY * 0.45f, 0.f };
	tParticleParam.tInit.tHexahedron.tStartBoundary.vMax = { WINCX * 0.5f, -WINCY * 0.1f, 0.f };
	//tParticleParam.tBoundary.vMin = { -WINCX * 0.5f, -20.f, 0.f };
	//tParticleParam.tBoundary.vMax = { WINCX * 0.5f, -10.f, 0.f };
	tParticleParam.tInit.tHexahedron.vInitVelocity = { 0.f, 150.f, 0.f };
	tParticleParam.vVelocityNoise = { 0.f, 0.f, 0.f };
	tParticleParam.vColor = _vec4(1.0f, 1.f, 1.f, 1.f);
	tParticleParam.vColorFade = _vec4(-0.5f, 1.f, -0.5f, 0.0f); // 255 대신 500 넣어서 투명해지기 전에 더 빨리 초록색이 되도록
	tParticleParam.iTotalCnt = 10;

	tParticleParam.fSize = 50.f;
	tParticleParam.fLifeTime = 0.75f;

	tParticleParam.fEmitRate = 4.f;
	tParticleParam.iEmitCnt = 2;

	m_pParticleSystem->Set_Parameter(tParticleParam);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	//m_pParticleSystem->Set_Option(CParticleSystem::OPTION::DEATH_OVER_BOUNDARY, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::POINT_SCALE_DISABLE, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);

	//for (int i = 0; i < 10; ++i)
	//{
	//	m_pParticleSystem->Add_Particle();
	//}
	m_pParticleSystem->SetUp_Particle();
}

void CEffectHeal::OnOperate(void* _pParam)
{
	CEffectHeal* pThis = (CEffectHeal*)_pParam;

	pThis->m_pParticleSystem->Reset();
	pThis->m_pEffectCom->Stop_Effect();
}
