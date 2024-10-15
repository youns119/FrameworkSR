#include "pch.h"
#include "../Header/DrinkMachine.h"
#include "Export_Utility.h"
#include "Export_System.h"

CDrinkMachine::CDrinkMachine(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
	, m_pAnimatorCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_bIsDead(false)
	, m_eCurState(DRINKMACHINESTATE::MACHINE_IDLE)
	, m_ePreState(DRINKMACHINESTATE::MACHINE_IDLE)
	, m_fSpawnDrinkTimer(0.5f)
	, m_iSpawnCount(12)
{
	for (_int i = 0; i < DRINKMACHINESTATE::MACHINE_END; ++i)
		m_pTextureCom[i] = nullptr;
}

CDrinkMachine::CDrinkMachine(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
	: CGameObject(_pGraphicDev)
	, m_pAnimatorCom(nullptr)
	, m_pBufferCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_bIsDead(false)
	, m_vStartPos(_vecPos)
	, m_eCurState(DRINKMACHINESTATE::MACHINE_IDLE)
	, m_ePreState(DRINKMACHINESTATE::MACHINE_IDLE)
	, m_fSpawnDrinkTimer(0.5f)
	, m_iSpawnCount(12)
{
	for (_int i = 0; i < DRINKMACHINESTATE::MACHINE_END; ++i)
		m_pTextureCom[i] = nullptr;
}

CDrinkMachine::~CDrinkMachine()
{
}

CDrinkMachine* CDrinkMachine::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CDrinkMachine* pGameObject = new CDrinkMachine(_pGraphicDev);

	if (FAILED(pGameObject->Ready_GameObject()))
	{
		Safe_Release(pGameObject);
		MSG_BOX("Boss_Robot Create Failed");
		return nullptr;
	}

	return pGameObject;
}

CDrinkMachine* CDrinkMachine::Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
{
	CDrinkMachine* pGameObject = new CDrinkMachine(_pGraphicDev, _vecPos);

	if (FAILED(pGameObject->Ready_GameObject()))
	{
		Safe_Release(pGameObject);
		MSG_BOX("Boss_Robot Create Failed");
		return nullptr;
	}

	return pGameObject;
}


HRESULT CDrinkMachine::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(m_vStartPos.x, m_vStartPos.y, m_vStartPos.z);
	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(1.f);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);

	Set_Animation();

	_vec3 vUp, vLook, vRight, vPos;
	m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);
	vPos = { -0.5f, -0.5f, 0.5f };
	_vec3 vOffset =
	{
		vUp.x + vRight.x + vPos.x,
		vUp.y + vRight.y + vPos.y,
		vUp.z + vRight.z + vPos.z
	};

	m_pColliderCom->SetOffsetPos(vOffset);
	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(1.25f);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);

	return S_OK;
}

_int CDrinkMachine::Update_GameObject(const _float& _fTimeDelta)
{
	//if (!m_bIsDead)

	Spawn_Drink();
	_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);

	if (DRINKMACHINESTATE::MACHINE_BROKEN == m_eCurState)
		m_fSpawnDrinkTimer += _fTimeDelta;

	_matrix		matWorld, matView, matBill, matResult;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	//D3DXMatrixIdentity(&matBill);

	//matBill._11 = matView._11;
	//matBill._13 = matView._13;
	//matBill._31 = matView._31;
	//matBill._33 = matView._33;

	//matBill._11 = matView._11;
	//matBill._12 = matView._12;
	//matBill._21 = matView._21;
	//matBill._22 = matView._22;

	//matBill._22 = matView._22;
	//matBill._23 = matView._23;
	//matBill._32 = matView._32;
	//matBill._33 = matView._33;

	//D3DXMatrixInverse(&matBill, 0, &matBill);
	//
	//matResult = matBill * matWorld;
	//
	//m_pTransformCom->Set_WorldMatrix(&(matResult));

	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Engine::Add_Collider(m_pColliderCom);

	//Jonghan Monster Change End

	return iExit;
}

void CDrinkMachine::LateUpdate_GameObject()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	_float fY(0.f);
	m_pTransformCom->Set_Pos(vPos.x, fY + 1.f, vPos.z);

	CGameObject::Compute_ViewZ(&vPos);

	//Change_State();

	State_Check();

	//Jonghan Monster Change End
	Engine::CGameObject::LateUpdate_GameObject();
}

void CDrinkMachine::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//Jonghan Monster Change Start

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pGraphicDev->SetTexture(0, nullptr);

	
	m_pAnimatorCom->Render_Animator();
	m_pBufferCom->Render_Buffer();
	//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void CDrinkMachine::Break_Machine()
{
	m_eCurState = CDrinkMachine::MACHINE_BROKEN;
}

void CDrinkMachine::OnCollisionEnter(CCollider& _pOther)
{
	//CGameObject* pGameObject = Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player");
	//dynamic_cast<CPlayer*>(pGameObject)->Get_HasItem();
	//m_pColliderCom->SetActive(false);
	//Spawn_Drink();
}

HRESULT CDrinkMachine::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom[DRINKMACHINESTATE::MACHINE_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DropMachineIdleTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_IdleTexture", pComponent });

	pComponent = m_pTextureCom[DRINKMACHINESTATE::MACHINE_ACTIVE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DropMachineActiveTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_ActiveTexture", pComponent });

	pComponent = m_pTextureCom[DRINKMACHINESTATE::MACHINE_BROKEN] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_DropMachineBrokenTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_BrokenTexture", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Calculator", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	pComponent->SetOwner(*this);

	pComponent = m_pAnimatorCom = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Animator", pComponent });

	return S_OK;
}

void CDrinkMachine::Set_Animation()
{
	m_pAnimatorCom->CreateAnimation(L"Idle", m_pTextureCom[DRINKMACHINESTATE::MACHINE_IDLE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Active", m_pTextureCom[DRINKMACHINESTATE::MACHINE_ACTIVE], 13.f);
	m_pAnimatorCom->CreateAnimation(L"Broken", m_pTextureCom[DRINKMACHINESTATE::MACHINE_BROKEN], 13.f);

	m_pAnimatorCom->PlayAnimation(L"Idle", true);
}

void CDrinkMachine::Spawn_Drink()
{
	if (0.45f < m_fSpawnDrinkTimer && 0 < m_iSpawnCount)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
		vPos.y += 0.7f;
		vPos.z -= 1.f;
		Engine::Spawn_DrinkObject(vPos);

		m_fSpawnDrinkTimer = 0.f;
		m_iSpawnCount--;
	}
}

void CDrinkMachine::State_Check()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case CDrinkMachine::MACHINE_IDLE:
			m_pAnimatorCom->PlayAnimation(L"Broken", true);
			break;
		case CDrinkMachine::MACHINE_ACTIVE:
			m_pAnimatorCom->PlayAnimation(L"Broken", true);
			break;
		case CDrinkMachine::MACHINE_BROKEN:
			m_pAnimatorCom->PlayAnimation(L"Broken", true);
			Engine::Play_Sound(L"DrinkMachine_Break.wav", CHANNELID::SOUND_PLAYER_LEG, 0.6f);
			Engine::Play_Sound(L"Soda_Spawn.wav", CHANNELID::SOUND_EFFECT, 0.8f);
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CDrinkMachine::Free()
{
	CGameObject::Free();
}
