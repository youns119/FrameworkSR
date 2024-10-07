#include "pch.h"
#include "../Header/EffectSpark.h"
#include "Export_Utility.h"

CEffectSpark::CEffectSpark(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
}

CEffectSpark::~CEffectSpark()
{
}

HRESULT CEffectSpark::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(2.f, 2.f, 2.f);
	//m_pTransformCom->Rotation(ROT::ROT_Z, D3DX_PI * 0.5f);
	m_pEffectCom->Set_LifeTime(0.5f);
	m_pEffectCom->Set_Visibility(FALSE);
	m_pEffectCom->Set_Repeatable(FALSE);
	m_pEffectCom->Set_CallBack(OnOperate);
	m_pEffectCom->Set_CallBackParam(this);

	Set_ParticleParam();
	Set_ParticleBloomParam();

	return S_OK;
}

_int CEffectSpark::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_pEffectCom->Get_Visibility())
		return 0;

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectSpark::LateUpdate_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;

	_vec3	vTemp;
	m_pTransformCom->Get_Info(Engine::INFO::INFO_POS, &vTemp);
	CGameObject::Compute_ViewZ(&vTemp);


	Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectSpark::Render_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Set_Texture();

	m_pParticleSystemBloomCom->Render_Parcitle();
	m_pParticleSystemCom->Render_Parcitle();
}

CEffectSpark* CEffectSpark::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CEffectSpark* pEffect = new CEffectSpark(_pGraphicDev);

	if (FAILED(pEffect->Ready_GameObject()))
	{
		Safe_Release(pEffect);
		MSG_BOX("spark create failed");
		return nullptr;
	}

	return pEffect;
}

void CEffectSpark::Free()
{
	Engine::CGameObject::Free();
}

HRESULT CEffectSpark::Add_Component()
{
	CComponent* pComponent(nullptr);

	pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_Glow_White"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pParticleSystemCom = static_cast<Engine::CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem", pComponent });

	pComponent = m_pParticleSystemBloomCom = static_cast<Engine::CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystemBloom", pComponent });

	pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

}

void CEffectSpark::Set_ParticleParam()
{
	CParticleSystem::PARAM tParam;

	tParam.eShape = CParticleSystem::SHAPE::CIRCLE;

	tParam.tInit.tCircle.fHeight = 1.f;
	tParam.tInit.tCircle.fRadius = 4.f;

	tParam.vVelocityNoise = { 0.f, 0.5f, 0.f };
	tParam.vColor = _vec4(1.0f, 0.884f, 0.525f, 1.f);
	tParam.vColorFade = _vec4(0.917f, 0.35f, 0.f, 1.f);
	tParam.iTotalCnt = 75;

	tParam.fSize = 0.075f;
	tParam.fSizeFade = 0.00f;
	tParam.fLifeTime = 0.4f;

	tParam.fEmitRate = 20.f;
	tParam.iEmitCnt = 5;

	tParam.fGravity = 7.5f;

	m_pParticleSystemCom->Set_Parameter(tParam);
	//m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	//m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::POINT_SCALE_DISABLE, TRUE);
	//m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
	m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::GRAVITY, TRUE);
	m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::ALPHAOP_ADD, TRUE);
	m_pParticleSystemCom->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);

	m_pParticleSystemCom->SetUp_Particle();
}

void CEffectSpark::Set_ParticleBloomParam()
{
	CParticleSystem::PARAM tParam;

	tParam.eShape = CParticleSystem::SHAPE::CIRCLE;

	tParam.tInit.tCircle.fHeight = 1.f;
	tParam.tInit.tCircle.fRadius = 4.f;

	tParam.vVelocityNoise = { 0.f, 0.5f, 0.f };
	tParam.vColor = _vec4(1.0f, 0.884f, 0.525f, 1.f);
	tParam.vColorFade = _vec4(0.917f, 0.35f, 0.f, 1.f);
	tParam.iTotalCnt = 25;

	tParam.fSize = 0.3f;
	tParam.fSizeFade = 0.00f;
	tParam.fLifeTime = 0.2f;

	tParam.fEmitRate = 20.f;
	tParam.iEmitCnt = 5;

	tParam.fGravity = 7.5f;

	m_pParticleSystemBloomCom->Set_Parameter(tParam);
	//m_pParticleSystemBloomCom->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystemBloomCom->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystemBloomCom->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystemBloomCom->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	//m_pParticleSystemBloomCom->Set_Option(CParticleSystem::OPTION::POINT_SCALE_DISABLE, TRUE);
	//m_pParticleSystemBloomCom->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
	m_pParticleSystemBloomCom->Set_Option(CParticleSystem::OPTION::GRAVITY, TRUE);
	m_pParticleSystemBloomCom->Set_Option(CParticleSystem::OPTION::ALPHAOP_ADD, TRUE);
	m_pParticleSystemBloomCom->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);

	m_pParticleSystemBloomCom->SetUp_Particle();
}

void CEffectSpark::OnOperate(void* _pParam)
{
	CEffectSpark* pThis = (CEffectSpark*)_pParam;

	pThis->m_pParticleSystemCom->Reset();
	pThis->m_pParticleSystemBloomCom->Reset();

	pThis->m_pEffectCom->Stop_Effect();
}
