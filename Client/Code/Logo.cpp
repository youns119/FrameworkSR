#include "pch.h"
#include "..\Header\Logo.h"
#include "Export_Utility.h"

CLogo::CLogo(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CScene(_pGraphicDev)
{
}

CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_Prototype(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);

	_matrix		matView, matProj;

	D3DXMatrixLookAtLH(&matView,
		&_vec3(0.f, 0.f, -10.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60.f), (_float)WINCX / WINCY, 0.1f, 1000.f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);


	return S_OK;
}

_int CLogo::Update_Scene(const _float& _fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(_fTimeDelta);

	return iExit;
}

void CLogo::LateUpdate_Scene()
{
	Engine::CScene::LateUpdate_Scene();
}

void CLogo::Render_Scene()
{
	Engine::CScene::Render_Scene();
}

HRESULT CLogo::Ready_Prototype()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);

	return S_OK;
}

HRESULT CLogo::Ready_Layer_Environment(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CBackGround::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

CLogo* CLogo::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CLogo* pLogo = new CLogo(_pGraphicDev);

	if (FAILED(pLogo->Ready_Scene()))
	{
		Safe_Release(pLogo);
		MSG_BOX("Logo Create Failed");
		return nullptr;
	}

	return pLogo;
}

void CLogo::Free()
{
	Engine::CScene::Free();
}