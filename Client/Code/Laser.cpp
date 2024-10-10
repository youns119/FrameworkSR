#include "pch.h"
#include "../Header/Laser.h"
#include "Export_Utility.h"

CLaser::CLaser(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CBullet(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pSecondTransformCom(nullptr)
	, m_pTransformPiVot(nullptr)
	, m_fTimer(0.f)
	, m_fFrame(0.f)
	, m_fMaxFrame(4.f)
	, m_fLinear(0.f)
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
	m_pTransformCom->Set_Scale(0.5f, 200.f, 0.5f);
	m_pSecondTransformCom->Set_Scale(0.5f, 200.f, 0.5f);
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
			return 0;
		}
		Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
		Engine::Add_Collider(m_pColliderCom);
		_vec3 vMonsterPos, vDir, vUp, vRight;
		_int iExit = Engine::CBullet::Update_GameObject(_fTimeDelta);

		//매 프레임마다 보스쪽에 위치 잡아주기
		CTransform* m_pBossRoBotTransformCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Monster", L"Boss_Robot", L"Com_Transform"));
		m_pBossRoBotTransformCom->Get_Info(INFO::INFO_POS, &vMonsterPos);
		m_pTransformPiVot->Set_Pos(vMonsterPos);
		_vec3 vTarget;
		_vec3 vPos;
		//가상의 물체로 점점 이동
		D3DXVec3Lerp(&vPos, &m_vLaserObj, &m_vEnd, m_fLinear);
		vTarget = vPos;
		//가상물체 바라보기.
		m_pTransformPiVot->LookAtTarget(&vTarget);
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
	//가상의 물체는 로봇보스 밑에서 시작!
	m_vLaserObj = { _vStartPos.x,_vStartPos.y - 50.f,_vStartPos.z };
	//플레이어 머리꼭대기로 끝점 잡기
	m_vEnd = { _vEndPos.x,_vEndPos.y + 200.f,_vEndPos.z };

	m_pTransformPiVot->Set_Pos(_vStartPos);
	//레이저 1의 위치
	m_pTransformCom->Set_Pos(-3.f, -199.f, 0.f);
	//레이저 2의 위치
	m_pSecondTransformCom->Set_Pos(3.f, -199.f, 0.f);
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
