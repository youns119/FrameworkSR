#include "pch.h"
#include "../Header/Laser.h"
#include "Export_Utility.h"

CLaser::CLaser(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CBullet(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_fTimer(0.f)
	, m_fFrame(0.f)
	, m_fMaxFrame(4.f)
	, m_fLinear(0.f)
	, m_fAngle(0.f)
{
}

CLaser::~CLaser()
{
}

HRESULT CLaser::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(0.05f);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(false);
	m_pTransformCom->Set_Scale(0.5f, 100.f, 0.5f);
	m_pSecondTransformCom->Set_Scale(0.5f, 100.f, 0.5f);
	m_pTransformPiVot->Set_Scale(1.f, 1.f, 1.f);


	return S_OK;
}

_int CLaser::Update_GameObject(const _float& _fTimeDelta)
{
	if (m_bIsRender) {

		m_fLinear += 0.005f;
		if (m_fLinear >= 1.f) {
			m_fLinear = 0.f;
			m_bIsRender = false;
			m_pColliderCom->SetActive(false);
		}
		Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
		Engine::Add_Collider(m_pColliderCom);
		CTransform* m_pBossRoBotTransformCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Monster", L"Boss_Robot", L"Com_Transform"));
		_vec3 vMonsterPos, vDir, vUp, vRight;
		m_pBossRoBotTransformCom->Get_Info(INFO::INFO_POS, &vMonsterPos);
		m_pTransformPiVot->Set_Pos(vMonsterPos);
		vDir = m_vLaserObj - vMonsterPos;
		D3DXVec3Normalize(&vDir, &vDir);
		vUp = { 0.f, 1.f, 0.f };
		D3DXVec3Cross(&vRight, &vUp, &vDir);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Cross(&vUp, &vDir, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);
		_matrix mWorld;
		*vDir *= -1;
		D3DXMatrixIdentity(&mWorld);
		memcpy(&mWorld.m[0][0], &vRight, sizeof(_vec3));
		memcpy(&mWorld.m[1][0], &vDir, sizeof(_vec3));
		memcpy(&mWorld.m[2][0], &vUp, sizeof(_vec3));
		memcpy(&mWorld.m[3][0], &vMonsterPos, sizeof(_vec3));
		m_fAngle = 45 * _fTimeDelta;
		m_pTransformPiVot->Rotation(ROTATION::ROT_X, D3DXToRadian(m_fAngle));
		_int iExit = Engine::CBullet::Update_GameObject(_fTimeDelta);

		_matrix		matWorld, matView, matBill, matResult;

		_matrix mRotationX;
		_matrix mRotationXInverse;
		D3DXMatrixRotationX(&mRotationX, (*m_pTransformPiVot->Get_Angle()).x);
		D3DXMatrixInverse(&mRotationXInverse, 0, &mRotationX);
		m_pTransformPiVot->Get_WorldMatrix(&matWorld);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixIdentity(&matBill);

		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		D3DXMatrixInverse(&matBill, 0, &matBill);

		matResult = mRotationX * matBill * mRotationXInverse * matWorld;

		m_pTransformPiVot->Set_WorldMatrix(&matResult);
		return iExit;
	}
	return 0;
}

void CLaser::LateUpdate_GameObject()
{
	if (m_bIsRender)
		m_pColliderCom->SetActive(true);
	else
		m_pColliderCom->SetActive(false);
	Engine::CBullet::LateUpdate_GameObject();
}

void CLaser::Render_GameObject()
{
	if (m_bIsRender)
	{
	
		_matrix m_matWolrd;
		m_matWolrd = (*m_pTransformCom->Get_WorldMatrix()) * (*m_pTransformPiVot->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWolrd);
		m_pTextureCom->Set_Texture((_uint)m_fFrame);
		m_pBufferCom->Render_Buffer();

		m_matWolrd = (*m_pSecondTransformCom->Get_WorldMatrix()) * (*m_pTransformPiVot->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWolrd);
		m_pTextureCom->Set_Texture((_uint)m_fFrame);
		m_pBufferCom->Render_Buffer();

	}
}

void CLaser::Fire_Laser(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage)
{
	m_bIsRender = true;
	m_pColliderCom->SetActive(true);
	m_vLaserObj = { _vEndPos.x,_vEndPos.y - 50.f,_vEndPos.z };
	m_pTransformPiVot->Set_Pos(_vStartPos);
	m_pTransformCom->Set_Pos(-3.f, -99.f, 0.f);
	m_pSecondTransformCom->Set_Pos(3.f, -99.f, 0.f);
	m_pTransformPiVot->Set_Angle(0.f, 0.f, 0.f);
}

CLaser* CLaser::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CLaser* pLaser = new CLaser(_pGraphicDev);

	if (FAILED(pLaser->Ready_GameObject()))
	{
		Safe_Release(pLaser);
		MSG_BOX("Laser Create Failed");
		return nullptr;
	}

	return pLaser;
}

HRESULT CLaser::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTransformPiVot = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform2", pComponent });

	pComponent = m_pSecondTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform3", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Laser"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_LaserTexture", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	pComponent->SetOwner(*this);


	return S_OK;
}

void CLaser::Free()
{
	CBullet::Free();
}
