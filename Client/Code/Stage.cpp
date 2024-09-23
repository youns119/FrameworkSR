#include "pch.h"
#include "..\Header\Stage.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "..\Header\DynamicCamera.h"
#include "../Header/EffectMuzzleFlash.h"
#include "../Header/EffectPlayerBlood.h"
#include "../Header/EffectBloodSplater.h"
#include "../Header/EffectFanSpread.h"
#include "../Header/EffectCircleLines.h"


CStage::CStage(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CScene(_pGraphicDev)
{
}

CStage::~CStage()
{
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CStage* pStage = new CStage(_pGraphicDev);

	if (FAILED(pStage->Ready_Scene()))
	{
		Safe_Release(pStage);
		MSG_BOX("Stage Create Failed");
		return nullptr;
	}

	return pStage;
}

HRESULT CStage::Ready_Scene()
{
	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Effect(L"Layer_Effect"), E_FAIL);

	Set_Collision();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

_int CStage::Update_Scene(const _float& _fTimeDelta)
{
	if (Engine::Key_Press(DIK_0))
		Engine::Toggle_Collider();

	if (Engine::Key_Press(DIK_9))
	{
		CGameObject* pGameObject = Engine::Get_CurrScene()->Get_GameObject(L"Layer_GameLogic", L"Player");
		dynamic_cast<CPlayer*>(pGameObject)->Toggle_Active();

		pGameObject = Engine::Get_CurrScene()->Get_GameObject(L"Layer_Environment", L"DynamicCamera");
		dynamic_cast<CDynamicCamera*>(pGameObject)->Toggle_Active();
	}

	if (Engine::Key_Press(DIK_8))
		Engine::CTimerManager::GetInstance()->OnOff_Timer();

	_int iExit = Engine::CScene::Update_Scene(_fTimeDelta);
	Engine::Update_Bullet(_fTimeDelta); //Jonghan Change

	return iExit;
}

void CStage::LateUpdate_Scene()
{
	Engine::CScene::LateUpdate_Scene();
	Engine::LateUpdate_Bullet(); //Jonghan Change // Is it Right? becuz BulletManager
}

void CStage::Render_Scene()
{
}

HRESULT CStage::Ready_LightInfo()
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

HRESULT CStage::Ready_Layer_Environment(const _tchar* _pLayerTag)
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

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ComponentCamera", Engine::CComponentCamera::Create(m_pGraphicDev)), E_FAIL);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);

	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	//Jonghan Change Start
	//pGameObject = CMonster::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);

	pGameObject = CWhiteSuit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);

	//pGameObject = CShotGun::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShotGun", pGameObject), E_FAIL);

	//pGameObject = CBlackMan::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlackMan", pGameObject), E_FAIL);

	//pGameObject = CFlyingDrone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FlyingDrone", pGameObject), E_FAIL);

	//pGameObject = CSpiderDrone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpiderDrone", pGameObject), E_FAIL);

	//pGameObject = CDogDrone::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DogDrone", pGameObject), E_FAIL);

	for (_int i = 0; i < 100; ++i)
	{
		Engine::CBullet* pAmmo = nullptr;
		pAmmo = CAmmo::Create(m_pGraphicDev);

		NULL_CHECK_RETURN(pAmmo, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_Bullet(pAmmo), E_FAIL);
	}
	//Jonghan Change End

	pGameObject = CFloor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Floor", pGameObject), E_FAIL);

	pGameObject = CWall::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Wall", pGameObject), E_FAIL);



	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar* _pLayerTag)
{
	// 연욱
	//Engine::CLayer* pLayer = CLayer::Create();
	//NULL_CHECK_RETURN(pLayer, E_FAIL);

	//Engine::CGameObject* pGameObject = nullptr;

	// 연욱
	Engine::CUI* pUI = nullptr;

	pUI = CUIPlus::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	//m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

// kyubin
HRESULT CStage::Ready_Layer_Effect(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CEffectMuzzleFlash::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectMuzzleFlash", pGameObject), E_FAIL);

	pGameObject = CEffectPlayerBlood::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectPlayerBlood", pGameObject), E_FAIL);

	pGameObject = CEffectBloodSplater::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectBloodSplater", pGameObject), E_FAIL);

	pGameObject = CEffectFanSpread::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectFanSpread", pGameObject), E_FAIL);
	m_mapLayer.insert({ _pLayerTag , pLayer });

	pGameObject = CEffectCircleLines::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectCircleLines", pGameObject), E_FAIL);
	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

void CStage::Set_Collision()
{
	Engine::CheckGroup(L"Layer_GameLogic", L"Layer_GameLogic");
}

void CStage::Free()
{
	Engine::CScene::Free();
}