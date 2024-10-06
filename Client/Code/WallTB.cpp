#include "pch.h"
#include "../Header/WallTB.h"
#include "Export_Utility.h"

CWallTB::CWallTB(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CTileContorl(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_vecWallDirection({0.f, 0.f, 0.f})
{
	m_pName = L"Proto_WALLCORNER01";
	m_iNumber = 0;
	m_iNumber_Type = 2;
}

CWallTB::~CWallTB()
{
}

CWallTB* CWallTB::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CWallTB* pWallTB = new CWallTB(_pGraphicDev);

	if (FAILED(pWallTB->Ready_GameObject()))
	{
		Safe_Release(pWallTB);
		MSG_BOX("pWall Create Failed");
		return nullptr;
	}

	return pWallTB;
}

CWallTB* CWallTB::Create_Pos(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos)
{
	CWallTB* pWallTB = new CWallTB(_pGraphicDev);

	if (FAILED(pWallTB->Ready_GameObject()))
	{
		Safe_Release(pWallTB);
		MSG_BOX("pWall Create Failed");
		return nullptr;
	}

	pWallTB->Setup_Position(_vecPos);
	pWallTB->m_vecPos = _vecPos;

	return pWallTB;
}

CWallTB* CWallTB::Create_Info(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _tchar* _pName)
{
	CWallTB* pWallTB = new CWallTB(_pGraphicDev);

	pWallTB->Setup_ImageName(_pName);

	if (FAILED(pWallTB->Ready_GameObject()))
	{
		Safe_Release(pWallTB);
		MSG_BOX("pTerrain Create Failed");
		return nullptr;
	}

	pWallTB->Setup_Position(_vecPos);
	pWallTB->m_vecPos = _vecPos;

	return pWallTB;
}

CWallTB* CWallTB::Create_InfoNumber(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _int& _iNumber)
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
	
	pWallTB->Set_Number(_iNumber - 1);

	return pWallTB;
}

CWallTB* CWallTB::Create_InfoNumberDirection(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _int& _iNumber, Engine::TILE_DIRECTION _eTileDirection)
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
	pWallTB->Set_Number(_iNumber - 1);
	pWallTB->Set_TileDirection(_eTileDirection);

	return pWallTB;
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
	pWallTB->Set_Number(_iNumber - 1);
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
	pWallTB->Set_TileDirection(_vecRot);
	pWallTB->Set_Trigger(_iTrigger);

	return pWallTB;
}

CWallTB* CWallTB::Create_InfoNumberDirection2(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot, const _int& _iNumber)
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
	pWallTB->Set_TileDirection(_vecRot);

	return pWallTB;
}

CWallTB* CWallTB::Create_Rot(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, _vec3 _vecRot)
{
	CWallTB* pWallTB = new CWallTB(_pGraphicDev);

	if (FAILED(pWallTB->Ready_GameObject()))
	{
		Safe_Release(pWallTB);
		MSG_BOX("pWall Create Failed");
		return nullptr;
	}

	pWallTB->Setup_Position(_vecPos);
	pWallTB->Setup_Angle(_vecRot);
	return pWallTB;
}

HRESULT CWallTB::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vUp, vLook, vRight;
	m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);

	_vec3 vOffset = { vUp.x + vRight.x, vUp.y + vRight.y, vUp.z + vRight.z };

	m_pColliderCom->SetTransform(m_pTransformCom);
	m_pColliderCom->SetRadius(1.f);
	m_pColliderCom->SetOffsetPos(vOffset);
	m_pColliderCom->SetLookDir(vLook);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);

	return S_OK;
}

_int CWallTB::Update_GameObject(const _float& _fTimeDelta)
{
	
	Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);

	Engine::Add_Collider(m_pColliderCom);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CWallTB::LateUpdate_GameObject()
{
	Engine::CGameObject::LateUpdate_GameObject();
}

void CWallTB::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	FAILED_CHECK_RETURN(Setup_Material(), );

	m_pTextureCom->Set_Texture(m_iNumber);

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

void CWallTB::Setup_ImageName(const _tchar* _pName)
{
	m_pName = _pName;
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
void CWallTB::Free()
{
	CTileContorl::Free();
}
