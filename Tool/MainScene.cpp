#include "pch.h"
#include "MainScene.h"
#include "GuideLine.h"
#include "ToolCamera.h"


CMainScene::CMainScene(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CScene(_pGraphicDev)
{
}

CMainScene::~CMainScene()
{
}

CMainScene* CMainScene::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CMainScene* pMainScene = new CMainScene(_pGraphicDev);

	if (FAILED(pMainScene->Ready_Scene()))
	{
		Safe_Release(pMainScene);
		MSG_BOX("Stage Create Failed");
		return nullptr;
	}

	return pMainScene;
}

HRESULT CMainScene::Ready_Scene()
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WallTex", Engine::CWallTex::Create(m_pGraphicDev, 30, 30, 30, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AcidWall", Engine::CTexture::Create(m_pGraphicDev, L"../Client/Bin/Resource/Texture/MMJ_wall/ACID C.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FloorTex", Engine::CFloorTex::Create(m_pGraphicDev, 30, 30, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FirstFloor", Engine::CTexture::Create(m_pGraphicDev, L"../Client/Bin/Resource/Texture/MMJ_Floor/Floor1.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GuideLine(L"Layer_GuideLine"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_ToolCamera(L"Layer_ToolCamera"), E_FAIL);


	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

_int CMainScene::Update_Scene(const _float& _fTimeDelta)
{
	_int iExit = Engine::CScene::Update_Scene(_fTimeDelta);
	Engine::Update_Bullet(_fTimeDelta); //Jonghan Change

	return iExit;
}

void CMainScene::LateUpdate_Scene()
{
	Engine::CScene::LateUpdate_Scene();
}

void CMainScene::Render_Scene()
{

}

HRESULT CMainScene::Ready_LightInfo()
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

HRESULT CMainScene::Ready_Layer_GuideLine(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CGuideLine::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"GuideLine", pGameObject), E_FAIL);


	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

HRESULT CMainScene::Ready_Layer_ToolCamera(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CToolCamera::Create(m_pGraphicDev,
		&_vec3(0.f, 10.f, -10.f),
		&_vec3(0.f, 0.f, 1.f),
		&_vec3(0.f, 1.f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);

	return S_OK;
}

void CMainScene::Free()
{
	Engine::CScene::Free();
}
