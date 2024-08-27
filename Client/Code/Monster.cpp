#include "pch.h"
#include "..\Header\Monster.h"
#include "Export_Utility.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CGameObject(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CMonster::~CMonster()
{
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CMonster* pMonster = new CMonster(_pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("pMonster Create Failed");
		return nullptr;
	}

	return pMonster;
}

HRESULT CMonster::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CMonster::Update_GameObject(const _float& _fTimeDelta)
{
	_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);

	Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Environment", L"Player", L"Com_Transform"));
	NULL_CHECK_RETURN(pPlayerTransform, -1);

	_vec3		vPlayerPos;
	pPlayerTransform->Get_Info(INFO::INFO_POS, &vPlayerPos);

	m_pTransformCom->Chase_Target(&vPlayerPos, 5.f * _fTimeDelta);

	return iExit;
}

void CMonster::LateUpdate_GameObject()
{
}

void CMonster::Render_GameObject()
{
}

HRESULT CMonster::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(Engine::Clone_Proto(L"Proto_TriCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CMonster::Free()
{
}
