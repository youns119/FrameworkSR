#include "pch.h"
#include "../Header/WallTB.h"
#include "Export_Utility.h"

CWallTB::CWallTB(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CTileContorl(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_vecWallDirection({0.f, 0.f, 0.f})
{
	m_iNumber = 0;
	m_iNumber_Type = 2;
}

CWallTB::~CWallTB()
{
}

CWallTB* CWallTB::Create_InfoNumberDirectionTrigger(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _int& _iNumber, Engine::TILE_DIRECTION _eTileDirection, const _int& _iTrigger)
{
	CWallTB* pWallTB = new CWallTB(_pGraphicDev);


	if (FAILED(pWallTB->Ready_GameObject()))
	{
		Safe_Release(pWallTB);
		MSG_BOX("pTerrain Create Failed");
		return nullptr;
	}

	pWallTB->Setup_Position(_vecPos);
	pWallTB->m_vecPos = _vecPos;
	pWallTB->Set_Number(_iNumber);
	pWallTB->m_iNumber = _iNumber;
	pWallTB->Set_TileDirection(_eTileDirection);
	pWallTB->Set_Trigger(_iTrigger);

	return pWallTB;
}

CWallTB* CWallTB::Create_InfoNumberDirectionTrigger2(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot, const _int& _iNumber, const _int& _iTrigger)
{
	CWallTB* pWallTB = new CWallTB(_pGraphicDev);


	if (FAILED(pWallTB->Ready_GameObject()))
	{
		Safe_Release(pWallTB);
		MSG_BOX("pTerrain Create Failed");
		return nullptr;
	}

	pWallTB->Setup_Position(_vecPos);
	pWallTB->m_vecPos = _vecPos;
	pWallTB->Set_Number(_iNumber);
	pWallTB->m_iNumber = _iNumber;
	pWallTB->Set_TileDirection(_vecRot);
	pWallTB->Set_Trigger(_iTrigger);

	return pWallTB;
}

void CWallTB::Set_TileDirection(const _vec3& _vecDir)
{
	m_vecWallDirection = _vecDir;

	_vec3 vUp, vLook, vRight;
	m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);

	_vec3 vOffset =
	{
		vUp.x + vRight.x + m_vecWallDirection.x,
		vUp.y + vRight.y + m_vecWallDirection.y,
		vUp.z + vRight.z + m_vecWallDirection.z
	};

	m_pColliderCom->SetOffsetPos(vOffset);
}

HRESULT CWallTB::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(1.f);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);

	return S_OK;
}

_int CWallTB::Update_GameObject(const _float& _fTimeDelta)
{
	if (!m_bIsRender)
		return 0;

	Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);

	Engine::Add_Collider(m_pColliderCom);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CWallTB::LateUpdate_GameObject()
{
	if (!m_bIsRender)
		return;

	Engine::CGameObject::LateUpdate_GameObject();
}

void CWallTB::Render_GameObject()
{
	if (!m_bIsRender)
		return;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	FAILED_CHECK_RETURN(Setup_Material(), );

	m_pTextureCom->Set_Texture(m_iNumber -1);

	m_pBufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CWallTB::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CWallTBTex*>(Engine::Clone_Proto(L"Proto_WallTBTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Wall"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	pComponent->SetOwner(*this);

	return S_OK;
}

HRESULT CWallTB::Setup_Material()
{
	D3DMATERIAL9 tMtrl;
	ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

	tMtrl.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	tMtrl.Specular = { 1.f, 1.f, 1.f, 1.f };
	tMtrl.Ambient = { 1.f, 1.f, 1.f, 1.f };

	tMtrl.Emissive = { 0.f, 0.f, 0.f, 0.f };
	tMtrl.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrl);

	return S_OK;
}

void CWallTB::Setup_Position(_vec3 _vecPos)
{
	m_pTransformCom->Set_Pos(_vecPos.x, _vecPos.y, _vecPos.z);
}

void CWallTB::Setup_Angle(_vec3 _vecRot)
{
	m_pTransformCom->Set_Angle(_vecRot.x, _vecRot.y, _vecRot.z);
}

void CWallTB::Set_TileDirection(Engine::TILE_DIRECTION _eTileDirection)
{
	switch (_eTileDirection)
	{
	case Engine::TILE_DIRECTION::TILE_FORWARD:
		m_vecWallDirection = { 0.f, 0.f, 1.f };
		break;
	case Engine::TILE_DIRECTION::TILE_RIGHT:
		m_vecWallDirection = { 1.f, 0.f, 0.f };
		break;
	case Engine::TILE_DIRECTION::TILE_LEFT:
		m_vecWallDirection = { -1.f, 0.f, 0.f };
		break;
	case Engine::TILE_DIRECTION::TILE_BACK:
		m_vecWallDirection = { 0.f, 0.f, -1.f };
		break;
	}
}
void CWallTB::Set_IsRender(const _bool& _bool)
{
	m_bIsRender = _bool;
	m_pColliderCom->SetActive(_bool);
	m_pColliderCom->SetShow(_bool);
}
void CWallTB::Free()
{
	CTileContorl::Free();
}
