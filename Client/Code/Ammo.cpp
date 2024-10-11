#include "pch.h"
#include "../Header/Ammo.h"
#include "Export_System.h"
#include "Export_Utility.h"

#include "../Header/WallTB.h"
#include "../Header/Wall.h"
#include "../Header/EffectPool.h"

CAmmo::CAmmo(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CBullet(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_pTextureCom_Boss(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_fTimer(0.f)
	, m_fFrame(0.f)
	, m_fMaxFrame(4.f)
	, m_bIsBoss(false)
	, m_fLinear(0.f)
{
}

CAmmo::~CAmmo()
{
}

HRESULT CAmmo::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(0.05f);
	m_pColliderCom->SetShow(false);
	m_pColliderCom->SetActive(false);
	_matrix matWorld;
	m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	matWorld._41 = 0.f;
	matWorld._43 = 10.f;

	m_pTransformCom->Set_WorldMatrix(&matWorld);


	return S_OK;
}

_int CAmmo::Update_GameObject(const _float& _fTimeDelta)
{
	//Jonghan Monster Change Start
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Engine::Add_Collider(m_pColliderCom);

	_int iExit = Engine::CBullet::Update_GameObject(_fTimeDelta);

	m_fFrame += 13.f * _fTimeDelta;

	if (m_fMaxFrame < m_fFrame) // Jonghan Change
		m_fFrame = 0.f;

	if (m_bIsRender)
	{
		m_fTimer += _fTimeDelta;
		if (4.f < m_fTimer && !m_bIsBoss)
		{
			m_bIsRender = false;
			m_pColliderCom->SetShow(false);
			m_pColliderCom->SetActive(false);
			m_fTimer = 0.f;
		}
		//Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform*>
		//	(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_GameLogic", L"Player", L"Com_Transform"));
		//NULL_CHECK_RETURN(pPlayerTransform, -1);
		//
		//_vec3 vPlayerPos;
		//pPlayerTransform->Get_Info(INFO::INFO_POS, &vPlayerPos);
		//
		//m_pTransformCom->Chase_Target(&vPlayerPos, 5.f * _fTimeDelta);

		//Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);
		_matrix		matWorld, matView, matBill, matResult;
		m_pTransformCom->Get_WorldMatrix(&matWorld);

		m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		D3DXMatrixIdentity(&matBill);

		matBill._11 = matView._11;
		matBill._13 = matView._13;
		matBill._31 = matView._31;
		matBill._33 = matView._33;

		D3DXMatrixInverse(&matBill, 0, &matBill);

		matResult = matBill * matWorld;
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

		m_pTransformCom->Set_WorldMatrix(&(matResult));
		if (!m_bIsBoss) {
			m_pTransformCom->Set_Pos(vPos.x + (m_vDir.x * _fTimeDelta * 5.f), vPos.y + (m_vDir.y * _fTimeDelta * 5.f), vPos.z + (m_vDir.z * _fTimeDelta * 5.f)); //Consider Speed of Bullet
		}
		else {
			m_fLinear += 0.2f * _fTimeDelta;
			if (m_fLinear >= 1.f) {
				m_fLinear = 0.f;
				m_bIsRender = false;
				m_pColliderCom->SetActive(false);
			}
			_vec3 vResult;
			D3DXVec3Lerp(&vResult, &m_fStartPos, &m_vDir, m_fLinear);
			m_pTransformCom->Set_Pos(vResult);
		}
		//Jonghan Monster Change End
		CGameObject::Compute_ViewZ(&vPos);

		// ±Ôºó - º® ½ºÆÄÅ©
		_vec3 RayStart = vPos;
		_vec3 RayDir = m_vDir;
		CGameObject* pGameObject(nullptr);
		CComponent* pComponent(nullptr);
		CWall* pWall(nullptr);
		CWallTB* pWallTB(nullptr);

		_float fDist = 0.f;
		_vec3 vHitPosition;
		D3DXVec3Normalize(&RayDir, &RayDir);
		pGameObject = Engine::CCollisionManager::GetInstance()->RayCastWall(RayStart, RayDir, &vHitPosition);
		//pGameObject->OnCollisionEnter(*m_pColliderCom);
		pWall = dynamic_cast<CWall*>(pGameObject);
		pWallTB = dynamic_cast<CWallTB*>(pGameObject);

		_vec3 vNormal{ 0.f, 0.f, 0.f };
		if (pWall)
			vNormal = pWall->Get_TileDirection();
		if (pWallTB)
			vNormal = pWallTB->Get_TileDirection();

		_vec3 vLength = vHitPosition - RayStart;
		if (D3DXVec3Length(&vLength) < 0.3f)
		{
			pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_Spark", L"Com_Transform");
			static_cast<CTransform*>(pComponent)->Set_Pos(vHitPosition);
			static_cast<CTransform*>(pComponent)->Set_Angle(-D3DX_PI * 0.5f * vNormal.z, 0.f, D3DX_PI * 0.5f * vNormal.x);
			pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
			static_cast<CEffectPool*>(pGameObject)->Operate();

			// ²ô±â
			Set_IsRender(FALSE);
			//m_bIsRender = false;
			//m_pColliderCom->SetShow(false);
			//m_pColliderCom->SetActive(false);
			m_fTimer = 0.f;
		}
	}

	return iExit;
}

void CAmmo::LateUpdate_GameObject()
{

	if (m_bIsRender)
		m_pColliderCom->SetActive(true);
	else
		m_pColliderCom->SetActive(false);

	Engine::CBullet::LateUpdate_GameObject();
}

void CAmmo::Render_GameObject()
{
	if (m_bIsRender)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		//Jonghan Monster Change Start

		//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		if (!m_bIsBoss)
			m_pTextureCom->Set_Texture((_uint)m_fFrame); //Jonghan Change
		else
			m_pTextureCom_Boss->Set_Texture((_uint)m_fFrame);

		m_pBufferCom->Render_Buffer();

		//m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}
}

void CAmmo::Fire_Bullet(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vDir, const _float& _fAttackDamage, const _bool& _bIsBoss)
{
	m_bIsRender = true;
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);
	m_fStartPos = _vStartPos;
	m_pTransformCom->Set_Pos(m_fStartPos);
	m_vDir = _vDir;
	m_fAttackDamage = _fAttackDamage;
	m_bIsBoss = _bIsBoss;

	if (m_bIsBoss) //BossÀÏ¶§ scale
	{
		m_pTransformCom->Set_Scale(0.5f, 0.5f, 0.5f);
	}
	else //Boss ¾Æ´Ò¶§ scale
	{
		m_pTransformCom->Set_Scale(0.25f, 0.25f, 0.25f);
	}
}


CAmmo* CAmmo::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CAmmo* pAmmo = new CAmmo(_pGraphicDev);

	if (FAILED(pAmmo->Ready_GameObject()))
	{
		Safe_Release(pAmmo);
		MSG_BOX("Ammo Create Failed");
		return nullptr;
	}

	return pAmmo;
}

HRESULT CAmmo::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_AmmoTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_AmmoTexture", pComponent });

	pComponent = m_pTextureCom_Boss = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossRobot_Bullet"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_BossRobot_BulletTexture", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	pComponent->SetOwner(*this);
	return S_OK;
}

void CAmmo::Free()
{
	CBullet::Free();
}
