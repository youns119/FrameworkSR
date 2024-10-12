#include "pch.h"
#include "../Header/Boss_Humanoid_Laser.h"
#include "Export_Utility.h"
#include "../Header/Boss_Humanoid.h"
CBoss_Humanoid_Laser::CBoss_Humanoid_Laser(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CBullet(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pTransformPiVot(nullptr)
	, m_fTimer(0.f)
	, m_fFrame(0.f)
	, m_fMaxFrame(4.f)
{
}

CBoss_Humanoid_Laser::~CBoss_Humanoid_Laser()
{
}

HRESULT CBoss_Humanoid_Laser::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.1f, 0.1f, 20.f);
	m_pTransformPiVot->Set_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

_int CBoss_Humanoid_Laser::Update_GameObject(const _float& _fTimeDelta)
{
	if (m_bIsRender) {
		CBoss_Humanoid* m_pBoss_Humanoid =
			static_cast<CBoss_Humanoid*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Monster", L"Boss_Humanoid"));
		if (m_pBoss_Humanoid->Get_IsDamaged())
		{
			m_fLinear = 0.f;
			m_bIsRender = false;
			return 0;
		}
		Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
		_int iExit = Engine::CBullet::Update_GameObject(_fTimeDelta);
		m_fLinear += 0.0015f;
		if (m_fLinear >= 1.f) {
			m_fLinear = 0.f;
			m_bIsRender = false;
			return 0;
		}
		_vec3 vTarget;
		_vec3 vPos;
		//가상의 물체로 점점 이동
		vPos = Bezier(m_vLaserObj, m_vCurvePos, m_vEnd, m_fLinear);
		//D3DXVec3Lerp(&vPos, &m_vLaserObj, &m_vEnd, m_fLinear);
		vTarget = vPos;
		//가상물체 바라보기.
		m_pTransformPiVot->LookAtTarget(&vTarget);
		return iExit;
	}
	return 0;
}

void CBoss_Humanoid_Laser::LateUpdate_GameObject()
{
	Engine::CBullet::LateUpdate_GameObject();
}

void CBoss_Humanoid_Laser::Render_GameObject()
{
	if (m_bIsRender)
	{
		_matrix m_matWolrd;
		m_matWolrd = (*m_pTransformCom->Get_WorldMatrix()) * (*m_pTransformPiVot->Get_WorldMatrix());
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWolrd);
		m_pTextureCom->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
}

void CBoss_Humanoid_Laser::Boss_Sniper_Laser(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos)
{
	m_bIsRender = true;
	m_vStart = _vStartPos;
	m_vEnd = { _vEndPos.x, _vEndPos.y,_vEndPos.z };
	m_vLaserObj = { _vStartPos.x + 90.f,_vStartPos.y,_vStartPos.z };
	m_vCurvePos = { _vEndPos.x - 100.f,_vEndPos.y + 30.f, m_vLaserObj.z };
	m_pTransformPiVot->Set_Pos(_vStartPos.x, _vStartPos.y + 0.5f, _vStartPos.z);
	m_pTransformCom->Set_Pos(0.f, 0.f, 20.f);
}

_vec3 CBoss_Humanoid_Laser::Bezier(_vec3 _vStartPos, _vec3 vCurvePos, _vec3 vEndPos, _float fTime)
{
	return (1 - fTime) * (1 - fTime) * _vStartPos +
		2 * (1 - fTime) * fTime * vCurvePos +
		fTime * fTime * vEndPos;
}

CBoss_Humanoid_Laser* CBoss_Humanoid_Laser::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CBoss_Humanoid_Laser* pBoss_Humanoid_Laser = new CBoss_Humanoid_Laser(_pGraphicDev);

	if (FAILED(pBoss_Humanoid_Laser->Ready_GameObject()))
	{
		Safe_Release(pBoss_Humanoid_Laser);
		MSG_BOX("Boss_Humanoid_Laser Create Failed");
		return nullptr;
	}

	return pBoss_Humanoid_Laser;
}

HRESULT CBoss_Humanoid_Laser::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pTransformPiVot = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform2", pComponent });

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_CubeTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_BossHumanoid_Laser"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_BossHumanoid_Laser", pComponent });

	return S_OK;
}

void CBoss_Humanoid_Laser::Free()
{
	CBullet::Free();
}
