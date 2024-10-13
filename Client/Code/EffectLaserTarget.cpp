#include "pch.h"
#include "../Header/EffectLaserTarget.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "../Header/Laser.h"

CEffectLaserTarget::CEffectLaserTarget(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
}

CEffectLaserTarget::~CEffectLaserTarget()
{
}

HRESULT CEffectLaserTarget::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pEffectCom->Set_LifeTime(1000.f);
	m_pEffectCom->Set_CallBack(OnOperate);
	m_pEffectCom->Set_CallBackParam(this);

	Set_ParticleCohesionPointParameter();
	Set_ParticleSparkParameter();

	return S_OK;
}

_int CEffectLaserTarget::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_pEffectCom->Get_Visibility())
		return 0;

	if (m_bIsMissing)
		m_fGraceTime += _fTimeDelta;

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectLaserTarget::LateUpdate_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;

	//CMissile* pTarget = static_cast<CMissile*>(m_pEffectCom->Get_CallerObject());
	//CLaser* pTarget = static_cast<CLaser*>(m_pEffectCom->Get_CallerObject());
	CGameObject* pTarget = m_pEffectCom->Get_CallerObject();

	//	m_bIsMissing = TRUE;

	//if (m_fGraceTime >= 1.f)
	if (!pTarget->Get_IsRender())
		m_pEffectCom->Stop_Effect();
	else
	{
		_vec3 vTargetPos;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vTargetPos);
		// 옥상 사각형 바닥안에 있는지 판단
		if (vTargetPos.x > 8.f && vTargetPos.x < 31.f &&
			vTargetPos.z > 34.f && vTargetPos.z < 53.f)
		{
			m_bIsInGround = TRUE;
		}
		else
		{
			m_bIsInGround = FALSE;
		}

		Engine::CParticleSystem::PARAM tParticleCohesionPointParam = m_pParticleSystemComCohesionPoint->Get_Parameter();
		tParticleCohesionPointParam.tInit.tSphere.vStartPos = vTargetPos;
		m_pParticleSystemComCohesionPoint->Set_Parameter(tParticleCohesionPointParam);

		Engine::CParticleSystem::PARAM tParticleSparkParam = m_pParticleSystemComSpark->Get_Parameter();
		tParticleSparkParam.tInit.tSphere.vStartPos = vTargetPos;
		m_pParticleSystemComSpark->Set_Parameter(tParticleSparkParam);

		Compute_ViewZ(&vTargetPos);
		pTarget->Compute_ViewZ(&vTargetPos);
		m_fViewZ -= 10.f;
	}


	Engine::CGameObject::LateUpdate_GameObject();

}

void CEffectLaserTarget::Render_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;


	m_pTextureCom->Set_Texture();

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	if (m_bIsInGround)
	{
		m_pParticleSystemComCohesionPoint->Render_Parcitle();
	}
	m_pParticleSystemComSpark->Render_Parcitle();
}

CEffectLaserTarget* CEffectLaserTarget::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CEffectLaserTarget* pEffect = new CEffectLaserTarget(_pGraphicDev);

	if (FAILED(pEffect->Ready_GameObject()))
	{
		Safe_Release(pEffect);
		MSG_BOX("CEffectSmokeTrail create failed");
		return nullptr;
	}

	return pEffect;
}

void CEffectLaserTarget::Free()
{
	Engine::CGameObject::Free();
}

HRESULT CEffectLaserTarget::Add_Component()
{
	Engine::CComponent* pComponent(nullptr);

	pComponent = m_pTextureCom = static_cast<Engine::CTexture*>(Engine::Clone_Proto(L"Proto_Glow_White"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = static_cast<Engine::CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });

	pComponent = m_pParticleSystemComSpark = static_cast<Engine::CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem_Spark", pComponent });

	pComponent = m_pParticleSystemComCohesionPoint = static_cast<Engine::CParticleSystem*>(Engine::Clone_Proto(L"Proto_ParticleSystem"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ParticleSystem_CohesionPoint", pComponent });

}

void CEffectLaserTarget::Set_ParticleCohesionPointParameter()
{
	CParticleSystem::PARAM tParam;
	tParam.eShape = CParticleSystem::SHAPE::SPHERE;
	tParam.tInit.tSphere.fRadius = 3.0f;
	tParam.tInit.tSphere.fTheta = D3DX_PI / 6.f;
	//tParam.vVelocityNoise = _vec3(1.f, 1.f, 1.f);
	tParam.vColor = _vec4(1.0f, 0.f, 0.0f, 1.f);
	tParam.vColorFade = _vec4(0.995f, 0.454f, 0.454f, 0.f);
	tParam.iTotalCnt = 200;

	tParam.fSize = 7.5f;
	tParam.fLifeTime = 0.15f;

	tParam.fEmitRate = 80.f;
	tParam.iEmitCnt = 10.f;

	m_pParticleSystemComCohesionPoint->Set_Parameter(tParam);
	m_pParticleSystemComCohesionPoint->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystemComCohesionPoint->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystemComCohesionPoint->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystemComCohesionPoint->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	m_pParticleSystemComCohesionPoint->Set_Option(CParticleSystem::OPTION::ALPHAOP_ADD, TRUE);
	m_pParticleSystemComCohesionPoint->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);

	m_pParticleSystemComCohesionPoint->SetUp_Particle();

}

void CEffectLaserTarget::Set_ParticleSparkParameter()
{
	CParticleSystem::PARAM tParam;
	tParam.eShape = CParticleSystem::SHAPE::SPHERE;
	tParam.tInit.tSphere.fRadius = 10.6f;
	tParam.tInit.tSphere.fTheta = D3DX_PI / 6.f;
	tParam.vColor = _vec4(1.0f, 0.461f, 0.461f, 1.f);
	tParam.vColorFade = _vec4(0.917f, 0.35f, 0.0f, 0.f);
	tParam.iTotalCnt = 500;

	tParam.fSize = 0.5f;
	//tParam.fLifeTime = 0.45f;
	tParam.fLifeTime = 1.f;

	tParam.fEmitRate = 100.;
	tParam.iEmitCnt = 10.f;

	tParam.fGravity = 9.5;

	m_pParticleSystemComSpark->Set_Parameter(tParam);
	m_pParticleSystemComSpark->Set_Option(CParticleSystem::OPTION::REPEAT, TRUE);
	m_pParticleSystemComSpark->Set_Option(CParticleSystem::OPTION::DEATH_OVER_TIME, TRUE);
	m_pParticleSystemComSpark->Set_Option(CParticleSystem::OPTION::ZWRITE_DISABLE, TRUE);
	m_pParticleSystemComSpark->Set_Option(CParticleSystem::OPTION::COLOR_FADE, TRUE);
	m_pParticleSystemComSpark->Set_Option(CParticleSystem::OPTION::ALPHAOP_ADD, TRUE);
	m_pParticleSystemComSpark->Set_Option(CParticleSystem::OPTION::EMISSION_CONTROL, TRUE);
	m_pParticleSystemComSpark->Set_Option(CParticleSystem::OPTION::GRAVITY, TRUE);

	m_pParticleSystemComSpark->SetUp_Particle();
}

void CEffectLaserTarget::OnOperate(void* _pParam)
{
	CEffectLaserTarget* pThis = (CEffectLaserTarget*)_pParam;

	pThis->m_pParticleSystemComCohesionPoint->Reset();
	pThis->m_pParticleSystemComSpark->Reset();

	pThis->m_bIsMissing = FALSE;
	pThis->m_fGraceTime = 0.f;

	Engine::Play_Sound(L"LaserBeam.wav", CHANNELID::SOUND_EFFECT, 1.f);

}
