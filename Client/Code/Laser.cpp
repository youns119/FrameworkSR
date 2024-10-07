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
	m_pTransformCom->Set_Scale(1.f, 100.f, 1.f);
	m_pTransformPiVot->Set_Scale(1.f, 1.f, 1.f);
	//m_pTransformPiVot->Rotation(ROTATION::ROT_X, D3DXToRadian(45.f));
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
		//로봇보스 팔에서 시작위치를 잡는다
		//레이저가 임의에 물체를 바라보게 만든다.
		//임의에 물체는 점점 플레이어를 향해 나아간다
		Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
		Engine::Add_Collider(m_pColliderCom);
		CTransform* m_pBossRoBotTransformCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Monster", L"Boss_Robot", L"Com_Transform"));
		_vec3 vMonsterPos, vDir, vUp, vRight;
		m_pBossRoBotTransformCom->Get_Info(INFO::INFO_POS, &vMonsterPos);
		m_pTransformPiVot->Set_Angle(*m_pBossRoBotTransformCom->Get_Angle());

		vDir = m_vLaserObj - vMonsterPos;
		D3DXVec3Normalize(&vDir, &vDir);
		vUp = { 0.f, 1.f, 0.f };

		D3DXVec3Cross(&vRight, &vUp, &vDir);
		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Cross(&vUp, &vDir, &vRight);
		D3DXVec3Normalize(&vUp, &vUp);


		_matrix mWorld;
		//*vDir *= -1;
		D3DXMatrixIdentity(&mWorld);
		memcpy(&mWorld.m[0][0], &vRight, sizeof(_vec3));
		memcpy(&mWorld.m[1][0], &vUp, sizeof(_vec3));
		memcpy(&mWorld.m[2][0], &vDir, sizeof(_vec3));
		memcpy(&mWorld.m[3][0], &vMonsterPos, sizeof(_vec3));

		//_matrix mResult = matResult + mWorld;
		m_pTransformPiVot->Set_WorldMatrix(&mWorld);

		m_fAngle -= 1;
		m_pTransformPiVot->Rotation(ROTATION::ROT_X, D3DXToRadian(m_fAngle));
		//m_pTransformCom->Set_Pos(m_vLaserObj);
		_int iExit = Engine::CBullet::Update_GameObject(_fTimeDelta);
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
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		m_pTextureCom->Set_Texture((_uint)m_fFrame);


		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

void CLaser::Fire_Laser(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage)
{

	m_bIsRender = true;
	m_pColliderCom->SetActive(true);
	m_vStart = _vStartPos;
	m_vEnd = _vEndPos;
	m_vLaserObj = { _vEndPos.x,_vEndPos.y - 50.f,_vEndPos.z };
	m_pTransformPiVot->Set_Pos(_vStartPos);
	m_pTransformCom->Set_Pos(0.f, -100.f, 0.f);
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
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Transform2", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CLaserTex*>(Engine::Clone_Proto(L"Proto_LaserTex"));
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
