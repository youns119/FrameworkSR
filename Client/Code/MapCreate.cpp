#include "pch.h"
#include "../Header/MapCreate.h"
#include "..\Header\DynamicCamera.h"
#include "..\Header\SkyBox.h"
#include "..\Header\Floor.h"
#include "..\Header\Wall.h"


CMapCreate::CMapCreate(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CScene(_pGraphicDev)
{
}

CMapCreate::~CMapCreate()
{
}

CMapCreate* CMapCreate::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CMapCreate* pMapCreate = new CMapCreate(_pGraphicDev);

	if (FAILED(pMapCreate->Ready_Scene()))
	{
		Safe_Release(pMapCreate);
		MSG_BOX("Stage Create Failed");
		return nullptr;
	}

	return pMapCreate;
}

HRESULT CMapCreate::Ready_Scene()
{

	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_ToolEnvironment(L"Layer_ToolEnvironment"), E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;

}

_int CMapCreate::Update_Scene(const _float& _fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(_fTimeDelta);

	return iExit;
}

void CMapCreate::LateUpdate_Scene()
{
	Engine::CScene::LateUpdate_Scene();

}

void CMapCreate::Render_Scene()
{
	ShowGUI();
	ImGui::Render();
}

HRESULT CMapCreate::Ready_LightInfo()
{
	D3DLIGHT9 tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;

	tLightInfo.Diffuse = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Specular = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Ambient = { 1.f, 1.f, 1.f, 1.f };
	tLightInfo.Direction = { 1.f, -1.f, 1.f };

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

	return S_OK;
}

HRESULT CMapCreate::Ready_Layer_ToolEnvironment(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	_vec3 vEye(0.f, 10.f, -10.f);
	_vec3 vAt(0.f, 0.f, 1.f);
	_vec3 vUp(0.f, 1.f, 0.f);

	pGameObject = CDynamicCamera::Create
	(
		m_pGraphicDev,
		&vEye,
		&vAt,
		&vUp
	);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);


	m_mapLayer.insert({ _pLayerTag , pLayer });


	return S_OK;
}

HRESULT CMapCreate::Ready_Layer_Tile(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;


	pGameObject = CFloor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Floor", pGameObject), E_FAIL);

	pGameObject = CWall::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Wall", pGameObject), E_FAIL);

	return E_NOTIMPL;
}

void CMapCreate::ShowGUI()
{
	static _vec2 vecPosition = _vec2(100.0f, 100.0f);
	ImGui::SliderFloat2("Position", vecPosition, -800.f, 800.f);

}

void CMapCreate::Free()
{
	Engine::CScene::Free();
}
