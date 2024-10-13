#include "pch.h"
#include "../Header/Laser.h"
#include "Export_Utility.h"

#include "../Header/EffectLaserTarget.h"
#include "../Header/Player.h"

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
{
}

CLaser::~CLaser()
{
}

HRESULT CLaser::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(0.5f, 0.5f, 200.f);
	m_pSecondTransformCom->Set_Scale(0.5f, 0.5f, 200.f);

	m_pTransformPiVot->Set_Scale(1.f, 1.f, 1.f);
	return S_OK;
}

_int CLaser::Update_GameObject(const _float& _fTimeDelta)
{
	if (m_bIsRender) {

		Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
		_int iExit = Engine::CBullet::Update_GameObject(_fTimeDelta);
		m_fLinear += 0.005f;
		if (m_fLinear >= 1.f) {
			m_fLinear = 0.f;
			m_bIsRender = false;
			return 0;
		}
		_vec3 vMonsterPos, vDir, vUp, vRight;

		//매 프레임마다 보스쪽에 위치 잡아주기
		CTransform* m_pBossRoBotTransformCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Monster", L"Boss_Robot", L"Com_Transform"));
		m_pBossRoBotTransformCom->Get_Info(INFO::INFO_POS, &vMonsterPos);
		m_pTransformPiVot->Set_Pos(vMonsterPos);
		_vec3 vTarget;
		_vec3 vPos;
		//가상의 물체가 플레이어로 점점 이동
		D3DXVec3Lerp(&vPos, &m_vLaserObj, &m_vEnd, m_fLinear);
		vTarget = vPos;

		//가상물체 바라보기.
		m_pTransformPiVot->LookAtTarget(&vTarget);
		m_pTransformCom->Get_Info(INFO::INFO_POS, &vRight);
		vDir = vTarget - vMonsterPos;
		D3DXVec3Normalize(&vDir, &vDir);

		if (Engine::RayCast(vMonsterPos, vDir))
		{
			CPlayer* m_pPlayer = static_cast<CPlayer*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Player", L"Player"));
			m_pPlayer->Set_PlayerHP(m_pPlayer->Get_PlayerHP() - 1.f);


			// 규빈 : 플레이어 피격 이펙트
			CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPlayerBlood", L"Com_Effect");
			static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);

			pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectRedFlash", L"Com_Effect");
			static_cast<CEffect*>(pComponent)->Operate_Effect();
	
		}
		// 규빈
		// vTarget = p2,  vMonsterPos = p1, p3 = (0, 0, 0);
		//_vec3 vUp = { 0.f, 1.f, 0.f };
		// laser vector = monster pos -> rotated vTarget
		_vec3 vLaserDir = vDir;

		_vec3 vRotationAxis;
		_matrix matRotation;
		//const _matrix* matPivotWorld = m_pTransformPiVot->Get_WorldMatrix();
		//memcpy(&vRotationAxis, &matPivotWorld->m[0], sizeof(_vec3));
		//D3DXMatrixRotationAxis(&matRotation, &vRotationAxis, D3DX_PI * 0.5f);
		////D3DXMatrixRotationX(&matRotation, -D3DX_PI * 0.5f);
		//D3DXVec3TransformNormal(&vLaserDir, &vLaserDir, &matRotation);

		if (vLaserDir.y != 0)
		{
			_float fU = (-1.f - vMonsterPos.y) / vLaserDir.y;
			_vec3 vLaserEffectPos = vMonsterPos + fU * (vLaserDir);
			vLaserEffectPos.y = 0.f;
			CComponent* pComponent(nullptr);
			pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectLaserTarget", L"Com_Transform");
			static_cast<CTransform*>(pComponent)->Set_Pos(vLaserEffectPos);
			pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectLaserTarget", L"Com_Effect");
			if (!static_cast<CEffect*>(pComponent)->Get_Visibility())
			{
				static_cast<CEffect*>(pComponent)->Operate_Effect();
				static_cast<CEffect*>(pComponent)->Set_CallerObject(this);
			}
		}

		return iExit;
	}
	return 0;
}

void CLaser::LateUpdate_GameObject()
{

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
	//가상의 물체는 로봇보스 밑에서 시작!
	m_vLaserObj = { _vStartPos.x,_vStartPos.y - 50.f,_vStartPos.z };
	//플레이어 머리꼭대기로 끝점 잡기
	m_vEnd = { _vEndPos.x,_vEndPos.y + 50.f,_vEndPos.z };

	m_pTransformPiVot->Set_Pos(_vStartPos);
	//레이저 1의 위치
	//레이저 1의 위치
	m_pTransformCom->Set_Pos(-3.f, 0.f, 199.f);
	//레이저 2의 위치
	m_pSecondTransformCom->Set_Pos(3.f, 0.f, 199.f);
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

	return S_OK;
}

void CLaser::Free()
{
	CBullet::Free();
}
