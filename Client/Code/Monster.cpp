 #include "pch.h"
#include "..\Header\Monster.h"
#include "Export_Utility.h"
#include "Export_System.h" // Jonghan Change

CMonster::CMonster(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CCharacter(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_pAnimatorCom(nullptr)
	, m_pHitBufferCom(nullptr)
	, m_pHeadHit(nullptr)
	, m_pCriticalHit(nullptr)
	, vKnockBackForce({ 0.f,0.f,0.f })
	, m_fHP(0.f)
	, m_bIsExecution(false)
{
}

CMonster::~CMonster()
{
}

_int CMonster::Update_GameObject(const _float& _fTimeDelta)
{
	//Jonghan Monster Change Start

	if (!m_bIsDead)
		Attack(_fTimeDelta);

	KnockBack(_fTimeDelta);
	_matrix		matWorld, matView, matBill, matResult;

	_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	if (!m_bIsExecution)
	{
		m_pTransformCom->Get_WorldMatrix(&matWorld);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixIdentity(&matBill);

		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		D3DXMatrixInverse(&matBill, 0, &matBill);

		matResult = matBill * matWorld;

		m_pTransformCom->Set_WorldMatrix(&(matResult));
	}



	if (m_bIsExecution)
		Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);
	else
		Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	Engine::Add_Collider(m_pColliderCom);

	//Jonghan Monster Change End

	return iExit;
}

void CMonster::LateUpdate_GameObject()
{
	//Jonghan Monster Change Start
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	_float fY = 0.f;
	m_pTransformCom->Set_Pos(vPos.x, fY + 1.f, vPos.z);

	if (!m_bIsExecution)
		CGameObject::Compute_ViewZ(&vPos);

	//Change_State();

	State_Check();

	//Jonghan Monster Change End
	Engine::CGameObject::LateUpdate_GameObject();
}

void CMonster::Damaged(const DAMAGED_STATE& _eDamagedState, const _float& _fAttackDamage)
{
	if (Engine::DAMAGED_STATE::DAMAGED_BODYSHOT == _eDamagedState || Engine::DAMAGED_STATE::DAMAGED_PUSHSHOT == _eDamagedState)
	{
		Damaged_By_Player(_eDamagedState, _fAttackDamage);
	}
	else
	{
		Damaged_By_Player(_eDamagedState, 15.f); //if Monster isn't Boss, it'll be Must Dead
	}
}

void CMonster::AddForce(const _float& _fPower, _vec3 _vLook, const _float& _fDamage)
{
	D3DXVec3Normalize(&_vLook, &_vLook);
	_vLook *= _fPower;
	vKnockBackForce = _vLook;
	Damaged(Engine::DAMAGED_STATE::DAMAGED_PUSHSHOT, _fDamage);
}

void CMonster::KnockBack(const _float& _fTimeDelta)
{
	_float vLength = D3DXVec3Length(&vKnockBackForce);
	vLength -= 1.f;
	if (vLength <= 0)
	{
		vLength = 0;
	}
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);
	vPos += vKnockBackForce * _fTimeDelta;
	m_pTransformCom->Set_Pos(vPos);
	D3DXVec3Normalize(&vKnockBackForce, &vKnockBackForce);
	vKnockBackForce *= vLength;
}

void CMonster::Free()
{
	Engine::CGameObject::Free();
}