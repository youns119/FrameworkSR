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
	, m_vStartPos({ 0.f,0.f,0.f })
	, m_pPlayerTransformCom(nullptr)
	, m_iTriggerCount(0)
{
}

CMonster::~CMonster()
{
}

_int CMonster::Update_GameObject(const _float& _fTimeDelta)
{
	//Jonghan Monster Change Start

	if (!m_bIsRender) //Trigger
		return 0;

	Picking_Terrain();

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
	if (!m_bIsRender) //Trigger
		return;

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	if (!m_bIsExecution)
		CGameObject::Compute_ViewZ(&vPos);

	//Change_State();

	State_Check();

	//Jonghan Monster Change End
	Engine::CGameObject::LateUpdate_GameObject();
}

void CMonster::Set_IsRender(const _bool& _bool)
{
	m_bIsRender = _bool;
	m_pColliderCom->SetActive(_bool);
	m_pColliderCom->SetShow(_bool);
}

void CMonster::Damaged(const DAMAGED_STATE& _eDamagedState, const _float& _fAttackDamage)
{
	if (Engine::DAMAGED_STATE::DAMAGED_BODYSHOT == _eDamagedState || Engine::DAMAGED_STATE::DAMAGED_PUSHSHOT == _eDamagedState)
	{
		//Damaged_By_Player(_eDamagedState, _fAttackDamage);
		Damaged_By_Player(_eDamagedState, 0.f);
	}
	else
	{
		Damaged_By_Player(_eDamagedState, 15.f); //if Monster isn't Boss, it'll be Must Dead
	}
}

void CMonster::AddForce(const _float& _fPower, _vec3 _vLook, const _float& _fDamage)
{
	D3DXVec3Normalize(&_vLook, &_vLook);
	_vLook.y = 0.f;
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

void CMonster::Picking_Terrain()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	_float fTilePos = Engine::FloorRayCast(vPos);

	if (fTilePos > 0.f)
		m_pTransformCom->Set_Pos(vPos.x, vPos.y - fTilePos + 1.f, vPos.z);
}

void CMonster::Collide_Wall(CCollider& _pOther)
{
	// 벽 충돌 밀어내기
	CGameObject* pGameObject = _pOther.GetOwner();

	if (Engine::Get_CurrScene()->Get_Layer(pGameObject) == L"Layer_Wall")
	{
		CCollider::AABB* vBoxThis = m_pColliderCom->GetAABB();
		CCollider::AABB* vBoxOther = _pOther.GetAABB();

		_vec3 vCenterThis = (vBoxThis->vMin + vBoxThis->vMax) / 2.f;
		_vec3 vCenterOther = (vBoxOther->vMin + vBoxOther->vMax) / 2.f;

		_vec3 vOverlap = vCenterThis - vCenterOther;
		_float fOverlapX = (vBoxThis->vMax.x - vBoxThis->vMin.x) / 2.0f + (vBoxOther->vMax.x - vBoxOther->vMin.x) / 2.0f - fabs(vOverlap.x);
		_float fOverlapZ = (vBoxThis->vMax.z - vBoxThis->vMin.z) / 2.0f + (vBoxOther->vMax.z - vBoxOther->vMin.z) / 2.0f - fabs(vOverlap.z);

		if (!(fOverlapX < 0 || fOverlapZ < 0))
		{
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

			if (fOverlapX < fOverlapZ)
			{
				if (vOverlap.x > 0)
					m_pTransformCom->Set_Pos(vPos.x + fOverlapX, vPos.y, vPos.z);
				else
					m_pTransformCom->Set_Pos(vPos.x - fOverlapX, vPos.y, vPos.z);
			}
			else
			{
				if (vOverlap.z > 0)
					m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z + fOverlapZ);
				else
					m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z - fOverlapZ);
			}

			m_pTransformCom->Update_Component(0.f);
			m_pColliderCom->LateUpdate_Component();
		}
	}
}

void CMonster::OnCollision(CCollider& _pOther)
{
	Collide_Wall(_pOther);
}

void CMonster::OnCollisionEnter(CCollider& _pOther)
{
	Collide_Wall(_pOther);
}

void CMonster::OnCollisionExit(CCollider& _pOther)
{
}

void CMonster::Free()
{
	Engine::CGameObject::Free();
}