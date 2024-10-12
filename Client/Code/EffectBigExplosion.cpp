#include "pch.h"
#include "../Header/EffectBigExplosion.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "../Header/EffectShockWave.h"
#include "../Header/EffectPool.h"

CEffectBigExplosion::CEffectBigExplosion(LPDIRECT3DDEVICE9 _pGrphicDev)
	: CGameObject(_pGrphicDev)
{
}

CEffectBigExplosion::~CEffectBigExplosion()
{
}

HRESULT CEffectBigExplosion::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(1.f, 1.f, 1.f);
	m_pTransformCom->Set_Scale(1.5f, 1.5f, 1.5f);
	//m_pTransformCom->Set_Scale(0.3f, 0.3f, 0.3f);

	m_pEffectCom->Set_LifeTime(2.1f);
	m_pEffectCom->Set_Visibility(FALSE);
	m_pEffectCom->Set_Repeatable(FALSE);
	m_pEffectCom->Set_CallBack(OnOperate);
	m_pEffectCom->Set_CallBackParam(this);

	Set_FlareParticle();
	Set_SmokeParticle();
	Set_FragmentParticle();

	return S_OK;
}

_int CEffectBigExplosion::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_pEffectCom->Get_Visibility())
		return 0;

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	return iExit;
}

void CEffectBigExplosion::LateUpdate_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;

	_vec3	vTemp;
	m_pTransformCom->Get_Info(Engine::INFO::INFO_POS, &vTemp);
	CGameObject::Compute_ViewZ(&vTemp);

	Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectBigExplosion::Render_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();
	m_pParticleSystemSmoke->Render_Parcitle();

	m_pFragmentTextureCom->Set_Texture();
	m_pParticleSystemFragment->Render_Parcitle();

	//m_pTextureCom->Set_Texture();
	m_pParticleSystemFlare->Render_Parcitle();
}

HRESULT CEffectBigExplosion::Add_Component()
{
	CComponent* pComponent(nullptr);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pFragmentTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Glow_White"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_FragmentTexture", pComponent });

	pComponent = m_pTextureCom = static_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Glow_2Texture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pParticleSystemFlare = static_cast<CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem_Flare", pComponent });

	pComponent = m_pParticleSystemSmoke = static_cast<CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem_Smoke", pComponent });

	pComponent = m_pParticleSystemFragment = static_cast<CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem_Fragment", pComponent });

	pComponent = m_pEffectCom = static_cast<CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

}

CEffectBigExplosion* CEffectBigExplosion::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffectBigExplosion* pExplosion = new CEffectBigExplosion(pGraphicDev);

	if (FAILED(pExplosion->Ready_GameObject()))
	{
		Safe_Release(pExplosion);
		MSG_BOX("pExplosion Create Failed");
		return nullptr;
	}

	return pExplosion;
}

void CEffectBigExplosion::Free()
{
	Engine::CGameObject::Free();
}

void CEffectBigExplosion::OnOperate(void* _pParam)
{
	CEffectBigExplosion* pThis = (CEffectBigExplosion*)_pParam;

	pThis->m_pParticleSystemSmoke->Reset();
	pThis->m_pParticleSystemFlare->Reset();
	pThis->m_pParticleSystemFragment->Reset();

	_vec3 vPos;
	pThis->m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_ShockWave", L"Com_Transform");
	CGameObject* pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
	static_cast<CTransform*>(pComponent)->Set_Pos(vPos);
	//static_cast<CEffectPool*>(pGameObject)->Set_CallerObject(pThis);
	static_cast<CEffectPool*>(pGameObject)->Operate();

	std::wstring strSoundKey = L"explosions-00";
	_uint iIdx = rand() % 4 + 1;
	strSoundKey += std::to_wstring(iIdx) + L".wav";
	Engine::Play_Sound(strSoundKey.c_str(), CHANNELID::SOUND_EFFECT, 1.0f);


	pThis->m_pEffectCom->Stop_Effect();
}

void CEffectBigExplosion::Set_FlareParticle()
{
	CParticleSystem::PARAM tParam;
	tParam.tInit.tHexahedron.tStartBoundary.vMin = { -0.3f, -0.3f, -0.3f };
	tParam.tInit.tHexahedron.tStartBoundary.vMax = { 0.3f, 0.3f, 0.3f };
	tParam.tInit.tHexahedron.vInitVelocity = { 0.f, 0.0f, 0.f };
	tParam.vVelocityNoise = { 2.5f, 2.5f, 2.5f };
	tParam.vColor = _vec4(1.0f, 50.f / 255.f, 0.f, 1.f);
	tParam.vColorFade = _vec4(0.1f, 0.1f, 0.1f, 0.0f);
	tParam.iTotalCnt = 200;

	tParam.fSize = 2.0f;
	tParam.fSizeFade = 0.f;
	tParam.fLifeTime = 0.5f;

	tParam.fEmitRate = 25.;
	tParam.iEmitCnt = 60.f;

	m_pParticleSystemFlare->Set_Parameter(tParam);
	//m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::ALPHAOP_ADD, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
	m_pParticleSystemFlare->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);

	m_pParticleSystemFlare->SetUp_Particle();
}

void CEffectBigExplosion::Set_SmokeParticle()
{
	CParticleSystem::PARAM tParam;
	tParam.tInit.tHexahedron.tStartBoundary.vMin = { -0.75f, -0.75f, -0.75f };
	tParam.tInit.tHexahedron.tStartBoundary.vMax = { 0.75f, 0.75f, 0.75f };
	tParam.tInit.tHexahedron.vInitVelocity = { 0.f, 0.f, 0.f };
	tParam.vVelocityNoise = { 2.0f, 2.0f, 2.0f };
	tParam.vColor = _vec4(0.8f, 0.8f, 0.8f, 1.f);
	tParam.vColorFade = _vec4(0.0f, 0.f, 0.f, 0.0f);
	tParam.iTotalCnt = 500;

	tParam.fSize = 1.2f;
	tParam.fSizeFade = 5.f;
	tParam.fLifeTime = 1.5f;

	tParam.fEmitRate = 50.f;
	tParam.iEmitCnt = 50;

	m_pParticleSystemSmoke->Set_Parameter(tParam);
	//m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
	m_pParticleSystemSmoke->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);

	m_pParticleSystemSmoke->SetUp_Particle();
}

void CEffectBigExplosion::Set_FragmentParticle()
{
	CParticleSystem::PARAM tParam;
	tParam.eShape = CParticleSystem::SHAPE::SPHERE;

	tParam.tInit.tSphere.fRadius = 10.f;
	tParam.tInit.tSphere.fTheta = D3DX_PI;
	tParam.vVelocityNoise = { 0.f, 0.0f, 0.f };
	tParam.vColor = _vec4(1.f, 225.f / 255.f, 134.f / 255.f, 1.f);
	tParam.vColorFade = _vec4(234.f / 255.f, 89.f / 255.f, 0.f, 1.0f);
	tParam.iTotalCnt = 50;

	tParam.fSize = 50.f;
	tParam.fSizeFade = 0.0f;
	tParam.fLifeTime = 0.4f;

	//tParam.fGravity = 8.5f;

	m_pParticleSystemFragment->Set_Parameter(tParam);
	//m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	//m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::SIZE_OVER_TIME, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::ALPHAOP_ADD, TRUE);
	m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::POINT_SCALE_DISABLE, TRUE);
	//m_pParticleSystemFragment->Set_Option(CParticleSystem::OPTION::GRAVITY, TRUE);

	m_pParticleSystemFragment->SetUp_Particle();
}


