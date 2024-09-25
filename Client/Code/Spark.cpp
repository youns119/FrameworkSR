#include "pch.h"
#include "../Header/Spark.h"
#include "Export_Utility.h"

CSpark::CSpark(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CSpark::~CSpark()
{
}

HRESULT CSpark::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_ParticleParam();

	m_pTransformCom->Set_Pos(10.f, 2.f, 10.f);

	m_pEffectCom->Set_LifeTime(0.5f);
	m_pEffectCom->Set_Visibility(FALSE);
	m_pEffectCom->Set_CallBack(OnOperate);
	m_pEffectCom->Set_CallBackParam(this);

	return S_OK;
}

_int CSpark::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_pEffectCom->Get_Visibility())
		return 0;
    Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

    return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CSpark::LateUpdate_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;

	_vec3	vTemp;
	m_pTransformCom->Get_Info(Engine::INFO::INFO_POS, &vTemp);
	CGameObject::Compute_ViewZ(&vTemp);

    Engine::CGameObject::LateUpdate_GameObject();
}

void CSpark::Render_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

    m_pTextureCom->Set_Texture();
    m_pParticleSystem->Render_Parcitle();
}

CSpark* CSpark::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
    CSpark* pObj = new CSpark(_pGraphicDev);

    if (FAILED(pObj->Ready_GameObject()))
    {
        Safe_Release(pObj);
        MSG_BOX("Spark Particle Create Failed");
        return nullptr;
    }
    return pObj;
}

void CSpark::Free()
{
    Engine::CGameObject::Free();
}

HRESULT CSpark::Add_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Glow_White"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = static_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pParticleSystem = static_cast<CParticleSpark*>(Engine::Clone_Proto(L"Proto_ParticleSpark"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem", pComponent });

	pComponent = m_pEffectCom = static_cast<CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });


	return S_OK;
}

void CSpark::Set_ParticleParam()
{
	CParticleSystem::PARAM tParticleParam;
	ZeroMemory(&tParticleParam, sizeof(CParticleSystem::PARAM));
	tParticleParam.tStartBoundary.vMin = { 0.f, 0.f, 0.f };
	tParticleParam.tStartBoundary.vMax = { 0.f, 0.f, 0.f };
	tParticleParam.vInitVelocity = { 0.f, 2.f, 0.f };
	//tParticleParam.vVelocityNoise = { 2.f, 2.f, 2.f };
	tParticleParam.tColor = D3DCOLOR_ARGB(255, 255, 255, 230);
	tParticleParam.fSize = 0.05f;
	tParticleParam.fLifeTime = 0.5f;
	tParticleParam.fGravity = 9.8f;

	m_pParticleSystem->Set_Parameter(tParticleParam);
	//m_pParticleSystem->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::GRAVITY, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystem->Set_Option(CParticleSystem::OPTION::ZWRITE, TRUE);

	m_pParticleSystem->Set_Diraction({ 1.f, 1.0f, -1.f });
	m_pParticleSystem->Set_Speed({ 2.f, 5.f });

	for (int i = 0; i < 300; ++i)
	{
		m_pParticleSystem->Add_Particle();
	}

	
}

void CSpark::OnOperate(void* _pParam)
{
	CSpark* pThis = (CSpark*)_pParam;

	pThis->m_pParticleSystem->Reset();
}
