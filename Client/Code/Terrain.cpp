#include "pch.h"
#include "..\Header\Terrain.h"
#include "Export_Utility.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CGameObject(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pTextureCom(nullptr)
{
}

CTerrain::~CTerrain()
{
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CTerrain* pTerrain = new CTerrain(_pGraphicDev);

	if (FAILED(pTerrain->Ready_GameObject()))
	{
		Safe_Release(pTerrain);
		MSG_BOX("pTerrain Create Failed");
		return nullptr;
	}

	return pTerrain;
}

HRESULT CTerrain::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTerrain::Update_GameObject(const _float& _fTimeDelta)
{
	Key_Input(_fTimeDelta);

	Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CTerrain::LateUpdate_GameObject()
{
	Engine::CGameObject::LateUpdate_GameObject();
}

void CTerrain::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTerrain::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CTerrainTex*>(Engine::Clone_Proto(L"Proto_TerrainTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_TerrainTexture"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CTerrain::Key_Input(const _float& _fTimeDelta)
{
	_vec3 vUp;

	m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);

	if (GetAsyncKeyState(VK_UP))
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), _fTimeDelta, 10.f);
	if (GetAsyncKeyState(VK_DOWN))
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), _fTimeDelta, -10.f);
	if (GetAsyncKeyState('Q'))
		m_pTransformCom->Rotation(ROTATION::ROT_X, D3DXToRadian(180.f * _fTimeDelta));
	if (GetAsyncKeyState('A'))
		m_pTransformCom->Rotation(ROTATION::ROT_X, D3DXToRadian(-180.f * _fTimeDelta));
	if (GetAsyncKeyState('W'))
		m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(180.f * _fTimeDelta));
	if (GetAsyncKeyState('S'))
		m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(-180.f * _fTimeDelta));
	if (GetAsyncKeyState('E'))
		m_pTransformCom->Rotation(ROTATION::ROT_Z, D3DXToRadian(180.f * _fTimeDelta));
	if (GetAsyncKeyState('D'))
		m_pTransformCom->Rotation(ROTATION::ROT_Z, D3DXToRadian(-180.f * _fTimeDelta));
}

void CTerrain::Free()
{
	Engine::CGameObject::Free();
}