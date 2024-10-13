#include "pch.h"
#include "../Header/EffectBossRobotDie.h"
#include "Export_Utility.h"
#include "../Header/EffectPool.h"

CEffectBossRobotDie::CEffectBossRobotDie(LPDIRECT3DDEVICE9 _pGraphicDev)
    : CGameObject(_pGraphicDev)
{
}

CEffectBossRobotDie::~CEffectBossRobotDie()
{
}

HRESULT CEffectBossRobotDie::Ready_GameObject()
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
    m_pEffectCom->Set_LifeTime(5.f);
	m_pEffectCom->Set_CallBack(OnOperate);
	m_pEffectCom->Set_CallBackParam(this);

    return S_OK;
}

_int CEffectBossRobotDie::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_pEffectCom->Get_Visibility())
		return 0;

	m_fElapsedTime += _fTimeDelta;
	if (m_fElapsedTime >= 0.2f)
	{
		m_fElapsedTime -= 0.2f;

		_vec3 vPos;
		CComponent* pComponent(nullptr);
		CGameObject* pGameObject(nullptr);

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Monster", L"Boss_Robot", L"Com_Transform");
		static_cast<CTransform*>(pComponent)->Get_Info(INFO::INFO_POS, &vPos);

		_vec3 vRandomOffset;
		vRandomOffset.x = rand() % 10000 * 0.0001 * 10.f - 5.0f;
		vRandomOffset.y = rand() % 10000 * 0.0001 * 10.f - 5.0f;
		vRandomOffset.z = rand() % 10000 * 0.0001 * 10.f - 5.0f;

		if (rand() % 3 > 0)
			pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_BigExplosion", L"Com_Transform");
		else
			pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_Explosion", L"Com_Transform");

		pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
		static_cast<CTransform*>(pComponent)->Set_Pos(vPos + vRandomOffset);
		static_cast<CTransform*>(pComponent)->Set_Scale(1.5f, 1.5f, 1.5f);
		//static_cast<CEffectPool*>(pGameObject)->Set_CallerObject(pThis);
		static_cast<CEffectPool*>(pGameObject)->Operate();
		//Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	}

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectBossRobotDie::LateUpdate_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;
	Engine::CGameObject::LateUpdate_GameObject();
}

void CEffectBossRobotDie::Render_GameObject()
{
	if (!m_pEffectCom->Get_Visibility())
		return;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


}

CEffectBossRobotDie* CEffectBossRobotDie::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CEffectBossRobotDie* pEffect = new CEffectBossRobotDie(_pGraphicDev);

	if (FAILED(pEffect->Ready_GameObject()))
	{
		Safe_Release(pEffect);
		MSG_BOX("CEffectSmokeTrail create failed");
		return nullptr;
	}

	return pEffect;
}

void CEffectBossRobotDie::Free()
{
	Engine::CGameObject::Free();
}

HRESULT CEffectBossRobotDie::Add_Component()
{
	Engine::CComponent* pComponent(nullptr);

	pComponent = m_pEffectCom = static_cast<Engine::CEffect*>(Engine::Clone_Proto(L"Proto_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Effect", pComponent });
}

void CEffectBossRobotDie::OnOperate(void* _pParam)
{
	CEffectBossRobotDie* pThis = (CEffectBossRobotDie*)_pParam;

	pThis->m_fElapsedTime = 0.f;
}
