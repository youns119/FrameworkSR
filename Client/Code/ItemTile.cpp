#include "pch.h"
#include "../Header/ItemTile.h"
#include "Export_System.h"
#include "Export_Utility.h"

CItemTile::CItemTile(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CTileContorl(_pGraphicDev)
	, m_pBufferCom(nullptr)
{
	m_iNumber = 0;
	m_iNumber_Type = 5;
}
CItemTile::~CItemTile()
{
}


CItemTile* CItemTile::Create_InfoNumber(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _int& _iNumber)
{
	CItemTile* pItemTile = new CItemTile(_pGraphicDev);

	if (FAILED(pItemTile->Ready_GameObject()))
	{
		Safe_Release(pItemTile);
		MSG_BOX("pTerrain Create Failed");
		return nullptr;
	}

	pItemTile->Setup_Position(_vecPos);
	pItemTile->m_vecPos = _vecPos;
	pItemTile->Setup_ImageNumber(_iNumber);
	pItemTile->m_iNumber = _iNumber;
	return pItemTile;
}

CItemTile* CItemTile::Create_InfoNumberTrigger(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vecPos, const _int& _iNumber, const _int& _iTriggerNumber)
{
	CItemTile* pItemTile = new CItemTile(_pGraphicDev);

	if (FAILED(pItemTile->Ready_GameObject()))
	{
		Safe_Release(pItemTile);
		MSG_BOX("pTerrain Create Failed");
		return nullptr;
	}
	pItemTile->Setup_Position(_vecPos);
	pItemTile->m_vecPos = _vecPos;
	pItemTile->Setup_ImageNumber(_iNumber);
	pItemTile->m_iNumber = _iNumber;
	pItemTile->Setup_TriggerNumber(_iTriggerNumber);

	return pItemTile;
}

HRESULT CItemTile::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	_vec3 vUp, vLook, vRight;
	m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);
	m_pTransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	m_pTransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);

	_vec3 vOffset = { vUp.x + vRight.x, vUp.y + vRight.y, vUp.z + vRight.z };


	return S_OK;
}

_int CItemTile::Update_GameObject(const _float& _fTimeDelta)
{
	Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);
	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CItemTile::LateUpdate_GameObject()
{
	Engine::CGameObject::LateUpdate_GameObject();
}

void CItemTile::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	FAILED_CHECK_RETURN(Setup_Material(), );

	m_pTextureCom->Set_Texture(m_iNumber - 1);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CItemTile::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcSpriteTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Item_Map"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

HRESULT CItemTile::Setup_Material()
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

void CItemTile::Setup_Position(_vec3 _vecPos)
{
	m_pTransformCom->Set_Pos(_vecPos.x, _vecPos.y, _vecPos.z);
}

void CItemTile::Setup_Angle(_vec3 _vecRot)
{
}

void CItemTile::Free()
{
	CTileContorl::Free();
}
