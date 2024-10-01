#include "pch.h"
#include "..\Header\Stage.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "..\Header\DynamicCamera.h"
// 규빈 (이텍트 해당하는 클래스 헤더 모음)
#include "../Header/Header_Effect.h"
#include "../Header/FilterFundo.h"

// 연욱
#include "../Header/UINormal.h"
#include "../Header/UIPlus.h"
#include "../Header/UIShop.h"
#include "../Header/UIInventory.h"
#include "../Header/UIFreeCam.h"

CStage::CStage(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CScene(_pGraphicDev)
	, m_pPlayer(nullptr)
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
	FAILED_CHECK_RETURN(Ready_Layer_Player(L"Layer_Player"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Monster(L"Layer_Monster"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Effect(L"Layer_Effect"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Item(L"Layer_Item"), E_FAIL);

	Set_Collision();

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

_int CStage::Update_Scene(const _float& _fTimeDelta)
{
	if (Engine::Key_Press(DIK_F1))
		Engine::Toggle_Collider();

	if (Engine::Key_Press(DIK_F2))
	{
		_uint iID = ((_uint)Engine::Get_ControllerID() + 1) % (_uint)CONTROLLERID::CONTROL_END;

		Engine::Set_ControllerID((CONTROLLERID)iID);

		if (Engine::Get_ControllerID() == CONTROLLERID::CONTROL_PLAYER)
			Engine::Deactivate_UI(UITYPE::UI_FREECAM);
		else
			Engine::Activate_UI(UITYPE::UI_FREECAM);
	}

	if (Engine::Key_Press(DIK_F3))
		if (Engine::Get_ListUI(UITYPE::UI_INVENTORY)->empty())
			Engine::Stop_Timer(!Engine::Get_Stop());

	if (Engine::Key_Press(DIK_F4))
	{
		_vec3 vPos = { (float)(rand() % 10), 1.f, (float)(rand() % 10) };

		CUI* pUI = Engine::Activate_UI(UITYPE::UI_PLUS);
		static_cast<CUIPlus*>(pUI)->Set_Pos(vPos);
		static_cast<CUIPlus*>(pUI)->Set_Index(Engine::Get_ListUI(UITYPE::UI_PLUS)->size());
	}

	if (Engine::Key_Press(DIK_F5))
	{
		if (Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
			Engine::Activate_UI(UITYPE::UI_SHOP);
		else
			Engine::Deactivate_UI(UITYPE::UI_SHOP);
	}

	if (Engine::Key_Press(DIK_TAB))
	{
		if (Engine::Get_ListUI(UITYPE::UI_INVENTORY)->empty())
		{
			Engine::Activate_UI(UITYPE::UI_INVENTORY);
			Engine::Stop_Timer(true);
		}
		else
		{
			Engine::Deactivate_UI(UITYPE::UI_INVENTORY);
			Engine::Stop_Timer(false);
		}
	}

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

	//pGameObject = CPlayer::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
	//m_pPlayer = static_cast<CPlayer*>(pGameObject);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pGameObject = CFloor::Create_Pos(m_pGraphicDev,{(float)j,0.f,(float)i});
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Floor", pGameObject), E_FAIL);
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pGameObject = CFloor::Create_Pos(m_pGraphicDev, { (float)j,4.f,(float)i });
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Floor", pGameObject), E_FAIL);
		}
	}

	pGameObject = CWall::Create_Pos(m_pGraphicDev, { 0.f ,0.f,0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Wall", pGameObject), E_FAIL);
	pGameObject = CWall::Create_Pos(m_pGraphicDev, { 0.f ,0.f,2.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Wall", pGameObject), E_FAIL);
	pGameObject = CWall::Create_Pos(m_pGraphicDev, { 4.f ,0.f,0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Wall", pGameObject), E_FAIL);
	pGameObject = CWall::Create_Pos(m_pGraphicDev, { 4.f ,0.f,2.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Wall", pGameObject), E_FAIL);
	
	pGameObject = CWall::Create_Pos(m_pGraphicDev, { 0.f ,2.f,0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Wall", pGameObject), E_FAIL);
	pGameObject = CWall::Create_Pos(m_pGraphicDev, { 0.f ,2.f,2.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Wall", pGameObject), E_FAIL);
	pGameObject = CWall::Create_Pos(m_pGraphicDev, { 4.f ,2.f,0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Wall", pGameObject), E_FAIL);
	pGameObject = CWall::Create_Pos(m_pGraphicDev, { 4.f ,2.f,2.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Wall", pGameObject), E_FAIL);
	
	pGameObject = CWallTB::Create_Pos(m_pGraphicDev, { 0.f ,0.f, 0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WallTB", pGameObject), E_FAIL);
	pGameObject = CWallTB::Create_Pos(m_pGraphicDev, { 2.f ,0.f,0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WallTB", pGameObject), E_FAIL);
	pGameObject = CWallTB::Create_Pos(m_pGraphicDev, { 0.f ,0.f,4.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WallTB", pGameObject), E_FAIL);
	pGameObject = CWallTB::Create_Pos(m_pGraphicDev, { 2.f ,0.f,4.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WallTB", pGameObject), E_FAIL);
	
	pGameObject = CWallTB::Create_Pos(m_pGraphicDev, { 0.f ,2.f, 0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WallTB", pGameObject), E_FAIL);
	pGameObject = CWallTB::Create_Pos(m_pGraphicDev, { 2.f ,2.f,0.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WallTB", pGameObject), E_FAIL);
	pGameObject = CWallTB::Create_Pos(m_pGraphicDev, { 0.f ,2.f,4.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WallTB", pGameObject), E_FAIL);
	pGameObject = CWallTB::Create_Pos(m_pGraphicDev, { 2.f ,2.f,4.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"WallTB", pGameObject), E_FAIL);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_Player(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
	m_pPlayer = static_cast<CPlayer*>(pGameObject);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_Monster(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	//Jonghan Change Start

	pGameObject = CWhiteSuit::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);

	pGameObject = CShotGun::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShotGun", pGameObject), E_FAIL);

	Engine::CGameObject* pShield = nullptr;
	pShield = CShield::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pShield, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Shield", pShield), E_FAIL);
	pGameObject = CBlackMan::Create(m_pGraphicDev, pShield);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BlackMan", pGameObject), E_FAIL);

	pGameObject = CFlyingDrone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FlyingDrone", pGameObject), E_FAIL);

	pGameObject = CSpiderDrone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SpiderDrone", pGameObject), E_FAIL);

	pGameObject = CDogDrone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DogDrone", pGameObject), E_FAIL);

	pGameObject = CBoss_Humanoid::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Humanoid", pGameObject), E_FAIL);

	pGameObject = CBoss_Robot::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Robot", pGameObject), E_FAIL);

	for (_int i = 0; i < 100; ++i)
	{
		Engine::CBullet* pAmmo = nullptr;
		pAmmo = CAmmo::Create(m_pGraphicDev);

		NULL_CHECK_RETURN(pAmmo, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_Bullet(pAmmo), E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bullet", pAmmo), E_FAIL);
	}
	//Jonghan Change End

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar* _pLayerTag)
{
	// 연욱
	Engine::CUI* pUI = nullptr;

	for (int i = 0; i < 20; i++)
	{
		pUI = CUIPlus::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pUI, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);
		pUI->Set_GameObject(m_pPlayer);
	}

	pUI = CUINormal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);
	pUI->Set_GameObject(m_pPlayer);

	pUI = CUIInventory::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);
	pUI->Set_GameObject(m_pPlayer);

	pUI = CUIShop::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);
	pUI->Set_GameObject(m_pPlayer);

	pUI = CUIFreeCam::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);
	pUI->Set_GameObject(m_pPlayer);

	Engine::Activate_UI(UITYPE::UI_NORMAL);

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

	pGameObject = CEffectPool::Create(m_pGraphicDev, (CGameObject * (*)(LPDIRECT3DDEVICE9))CEffectBloodSplater::Create);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectPool_BloodSplater", pGameObject), E_FAIL);

	pGameObject = CEffectPool::Create(m_pGraphicDev, (CGameObject * (*)(LPDIRECT3DDEVICE9))CSpark::Create);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectPool_Spark", pGameObject), E_FAIL);

	//pGameObject = CEffectBloodSplater::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectBloodSplater", pGameObject), E_FAIL);

	pGameObject = CEffectFanSpread::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectFanSpread", pGameObject), E_FAIL);
	m_mapLayer.insert({ _pLayerTag , pLayer });

	pGameObject = CEffectCircleLines::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectCircleLines", pGameObject), E_FAIL);
	m_mapLayer.insert({ _pLayerTag , pLayer });

	//pGameObject = CSpark::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectSpark", pGameObject), E_FAIL);
	//m_mapLayer.insert({ _pLayerTag , pLayer });

	pGameObject = CEffectHeal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectHeal", pGameObject), E_FAIL);
	m_mapLayer.insert({ _pLayerTag , pLayer });

	pGameObject = CEffectBossRobotBooster::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectBossRobotBooster", pGameObject), E_FAIL);
	m_mapLayer.insert({ _pLayerTag , pLayer });


	//pGameObject = CFilterFundo::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FilterFundo", pGameObject), E_FAIL);

	pGameObject = CEffectPool::Create(m_pGraphicDev, (CGameObject * (*)(LPDIRECT3DDEVICE9))CEffectMinigunShell::Create);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectPool_MinigunShell", pGameObject), E_FAIL);

	return S_OK;
}

HRESULT CStage::Ready_Layer_Item(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CKnife::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Knife", pGameObject), E_FAIL);

	pGameObject = CAxe::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Axe", pGameObject), E_FAIL);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

void CStage::Set_Collision()
{
	Engine::CheckGroup(L"Layer_Player", L"Layer_GameLogic");
	Engine::CheckGroup(L"Layer_Player", L"Layer_Monster");
	Engine::CheckGroup(L"Layer_Player", L"Layer_Item");
}

void CStage::Free()
{
	Engine::CScene::Free();
}