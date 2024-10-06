#include "pch.h"
#include "../Header/EffectPool.h"
#include "Export_Utility.h"

void CEffectPool::Operate()
{
	for (auto pObj : m_vecObject)
	{
		CComponent* pComponent = pObj->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Effect");
		CEffect* pEffect = dynamic_cast<CEffect*>(pComponent);
		if (nullptr == pEffect)
			continue;

		if (pEffect->Get_Visibility())
			continue;

		// pool 의 transform 을 이용해서 effect의 방향과 위치를 설정해주는 코드
		pComponent = pObj->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform");
		if (pComponent)
		{
			CTransform* pTransform = static_cast<CTransform*>(pComponent);
			//m_pTransformCom->Set_WorldMatrix(pTransform->Get_WorldMatrix());
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
			pTransform->Set_Pos(vPos);
			pTransform->Set_Angle(*(m_pTransformCom->Get_Angle()));
			pTransform->Set_Scale(*(m_pTransformCom->Get_Scale()));
		}

		pEffect->Set_CallerObject(m_pCallerObject);
		pEffect->Operate_Effect();
		break;
	}
}
//이펙트가 미사일의 트랜스폼을 받아오는 방법 : 오브젝트 풀에서 호출할때 미사일의 주소나, 미사일 트랜스폼의 주소를 넘겨야한다.


CEffectPool::CEffectPool(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
	, m_pCallerObject(nullptr)
{
}

CEffectPool::~CEffectPool()
{
}

HRESULT CEffectPool::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// 게임 오브젝트도 클론을 해야할까?
	// 만드는 함수를 콜백으로 넘길까? 이렇ㄱ ㅔ해볼게
	NULL_CHECK_RETURN(m_fnCreate, E_FAIL);

	for (_uint i = 0; i < 10; ++i)
		m_vecObject.push_back(m_fnCreate(m_pGraphicDev));

	return S_OK;
}

_int CEffectPool::Update_GameObject(const _float& _fTimeDelta)
{
	for (auto pObj : m_vecObject)
		pObj->Update_GameObject(_fTimeDelta);

	return CGameObject::Update_GameObject(_fTimeDelta);
}

void CEffectPool::LateUpdate_GameObject()
{
	for (auto pObj : m_vecObject)
		pObj->LateUpdate_GameObject();

	CGameObject::LateUpdate_GameObject();
}

void CEffectPool::Render_GameObject()
{
	for (auto pObj : m_vecObject)
	{
		if (static_cast<CEffect*>(pObj->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Effect"))->Get_Visibility())
			pObj->Render_GameObject();
	}
}

CEffectPool* CEffectPool::Create(LPDIRECT3DDEVICE9 _pGraphicDev, CGameObject* (*_fnCreate)(LPDIRECT3DDEVICE9))
{
	CEffectPool* pObj = new CEffectPool(_pGraphicDev);
	pObj->m_fnCreate = _fnCreate;
	if (FAILED(pObj->Ready_GameObject()))
	{
		Safe_Release(pObj);
		MSG_BOX("Effect Pool Create Failed!");
		return nullptr;
	}

	return pObj;
}

void CEffectPool::Free()
{
	for_each(m_vecObject.begin(), m_vecObject.end(), CDeleteObj());
	Engine::CGameObject::Free();
}

HRESULT CEffectPool::Add_Component()
{
	m_pTransformCom = static_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_pTransformCom->SetOwner(*this);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", m_pTransformCom });

	return S_OK;
}
