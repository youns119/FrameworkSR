#include "pch.h"
#include "..\Header\Stage.h"
#include "../Header/GameStage2.h"
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
#include "../Header/UIMisterBullet.h"
#include "../Header/UIRoboto.h"
#include "../Header/UIFreeCam.h"
#include "../Header/UIScreen.h"
#include "../Header/UIPhone.h"
#include "../Header/UIPlayer.h"
#include "../Header/UIAction.h"
#include "../Header/UIDown.h"
#include "../Header/UICombo.h"
#include "../Header/UIFloor.h"
#include "../Header/UIVictory.h"
#include "../Header/UISkill.h"

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
	FAILED_CHECK_RETURN(Ready_Layer_MonsterBullet(L"Layer_MonsterBullet"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Effect(L"Layer_Effect"), E_FAIL);
	//FAILED_CHECK_RETURN(Ready_Layer_Item(L"Layer_Item"), E_FAIL); //맵 로드 문제로 Ready_Layer_GameLogic함수에서 아이템 레이어 생성해주고 있습니다.

	//MapLoad(Find_Layer(L"Layer_GameLogic"));
	//ChangeMapLoad();
	Set_Collision();


	Engine::Calculate_Trigger();//10.06

	Engine::Play_Sound(L"ReadyGo.wav", CHANNELID::SOUND_EFFECT, 1.f);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

_int CStage::Update_Scene(const _float& _fTimeDelta)
{
	// 콜라이더 OnOff
	if (Engine::Key_Press(DIK_F1))
		Engine::Toggle_Collider();

	

	

	// 자유ㅠ시점 OnOff
	if (Engine::Key_Press(DIK_F2))
	{
		_uint iID = ((_uint)Engine::Get_ControllerID() + 1) % (_uint)CONTROLLERID::CONTROL_END;

		Engine::Set_ControllerID((CONTROLLERID)iID);

		if (Engine::Get_ControllerID() == CONTROLLERID::CONTROL_PLAYER)
			Engine::Deactivate_UI(UITYPE::UI_FREECAM);
		else
			Engine::Activate_UI(UITYPE::UI_FREECAM);
	}

	// 인벤토리 OnOff
	if (Engine::Key_Press(DIK_F3))
		if (Engine::Get_ListUI(UITYPE::UI_INVENTORY)->empty())
			Engine::Stop_Timer(!Engine::Get_Stop());

	// 킬로그 생성
	if (Engine::Key_Press(DIK_F4))
	{
		_vec3 vPos = { (float)(rand() % 10), 1.f, (float)(rand() % 10) };
		_int iIndex = rand() % 13;

		CUI* pUI = Engine::Activate_UI(UITYPE::UI_PLUS);
		static_cast<CUIPlus*>(pUI)->Init(vPos, (CUIPlus::UI_PLUS)iIndex);
	}

	// 상점 OnOff
	if (Engine::Key_Press(DIK_F5))
	{
		if (Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
			Engine::Activate_UI(UITYPE::UI_SHOP);
		else
			Engine::Deactivate_UI(UITYPE::UI_SHOP);
	}

	// MisterBullet 보스 UI OnOff
	if (Engine::Key_Press(DIK_F6))
	{
		if (Engine::Get_ListUI(UITYPE::UI_MISTERBULLET)->empty())
			Engine::Activate_UI(UITYPE::UI_MISTERBULLET);
		else
			Engine::Deactivate_UI(UITYPE::UI_MISTERBULLET);
	}

	// MisterBullet 보스 카운트 Up
	if (!Engine::Get_ListUI(UITYPE::UI_MISTERBULLET)->empty() &&
		Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
		static_cast<CUIMisterBullet*>(Engine::Get_ListUI(UITYPE::UI_MISTERBULLET)->front())->Add_Count();

	// MisterBullet 스나이퍼 HP 조절
	if (!Engine::Get_ListUI(UITYPE::UI_MISTERBULLET)->empty())
	{
		if (Engine::Key_Hold(DIK_LEFTARROW))
			m_pPlayer->Set_PlayerHP(m_pPlayer->Get_PlayerHP() - 1.f);
		else if (Engine::Key_Hold(DIK_RIGHTARROW))
			m_pPlayer->Set_PlayerHP(m_pPlayer->Get_PlayerHP() + 1.f);
	}

	//if (Engine::Key_Press(DIK_F8))
	//{
	//	if (Engine::Get_ListUI(UITYPE::UI_SCREEN)->empty())
	//	{
	//		Engine::Activate_UI(UITYPE::UI_SCREEN);
	//		static_cast<CUIScreen*>(Engine::Get_ListUI(UITYPE::UI_SCREEN)->front())->Set_FloorTime(Engine::Get_Elapsed()); //줄어들기전
	//	}
	//	else static_cast<CUIScreen*>(Engine::Get_ListUI(UITYPE::UI_SCREEN)->front())->Set_Return(true); //줄어들어있으니까 줄어들어라
	//}

	if (Engine::Key_Press(DIK_F9))
	{
		if (Engine::Get_ListUI(UITYPE::UI_ACTION)->empty())
			Engine::Activate_UI(UITYPE::UI_ACTION);
	}

	// 인벤토리 OnOff
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

	if (Engine::Key_Press(DIK_RETURN))
	{
		Engine::CScene* pStage = CGameStage2::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pStage, -1);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage), E_FAIL);

		return 0;
	}

	
	if (m_pPlayer->Get_Clear())
	{
		if (Engine::Get_ListUI(UITYPE::UI_SHOP)->empty() == false)
		{
			Engine::CScene* pStage = CGameStage2::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pStage, -1);

			FAILED_CHECK_RETURN(Engine::Set_Scene(pStage), E_FAIL);

			return 0;

		}
		return 0;
	}
	

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

	pGameObject = CSkyBox::Create(m_pGraphicDev,7);//5=도시배경,7=까만배경
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ComponentCamera", Engine::CComponentCamera::Create(m_pGraphicDev)), E_FAIL);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	Engine::CLayer* pLayer2 = CLayer::Create();
	Engine::CLayer* pLayer3 = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	NULL_CHECK_RETURN(pLayer2, E_FAIL);
	NULL_CHECK_RETURN(pLayer3, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	MapLoad2(pLayer, pLayer2, pLayer3);

	pGameObject = CDrinkMachine::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DrinkMachine", pGameObject), E_FAIL);

	for (_int i = 0; i < 32; i++)
	{
		Engine::CSoda* pSoda = nullptr;
		pSoda = CDrink::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pSoda, E_FAIL);
		FAILED_CHECK_RETURN(pLayer3->Add_GameObject(L"Drink", pSoda), E_FAIL);
		Engine::Set_DrinkObject(pSoda);
	}

	m_mapLayer.insert({ L"Layer_Wall" , pLayer});
	m_mapLayer.insert({ L"Layer_Monster" , pLayer2});
	m_mapLayer.insert({ L"Layer_Item" , pLayer3 });

	return S_OK;
}

HRESULT CStage::Ready_Layer_Player(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CPlayer::Create(m_pGraphicDev, _vec3{ 8.75f, 1.f, 1.f });
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
	m_pPlayer = static_cast<CPlayer*>(pGameObject);
	m_pPlayer->Set_CurrFloor(1);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_MonsterBullet(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CLayer* pLayer2 = CLayer::Create();
	NULL_CHECK_RETURN(pLayer2, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	//Jonghan Change Start

	for (_int i = 0; i < 1000; ++i)
	{
		Engine::CBullet* pAmmo = nullptr;
		pAmmo = CAmmo::Create(m_pGraphicDev);

		NULL_CHECK_RETURN(pAmmo, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_Bullet(pAmmo), E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bullet", pAmmo), E_FAIL);
	}

	for (_int i = 0; i < 30; ++i)
	{
		Engine::CBullet* pMissile = nullptr;
		pMissile = CMissile::Create(m_pGraphicDev);

		NULL_CHECK_RETURN(pMissile, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_Missile(pMissile), E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Missile", pMissile), E_FAIL);
	}
	//Jonghan Change End
	for (_int i = 0; i < 10; ++i)
	{
		Engine::CBullet* pLaser = nullptr;
		pLaser = CLaser::Create(m_pGraphicDev);

		NULL_CHECK_RETURN(pLaser, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_Laser(pLaser), E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Laser", pLaser), E_FAIL);
	}
	for (_int i = 0; i < 128; ++i)
	{
		Engine::CBullet* pMiniGun = nullptr;
		pMiniGun = CMiniGun::Create(m_pGraphicDev);

		NULL_CHECK_RETURN(pMiniGun, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_MiniGun(pMiniGun), E_FAIL);
		FAILED_CHECK_RETURN(pLayer2->Add_GameObject(L"MiniGun", pMiniGun), E_FAIL);
	}
	m_mapLayer.insert({ _pLayerTag , pLayer });
	m_mapLayer.insert({ L"Layer_PlayerBullet" , pLayer2 });

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
	}

	pUI = CUINormal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUIInventory::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUIShop::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUIMisterBullet::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUIFreeCam::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUIScreen::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUIPhone::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUIPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUIAction::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUICombo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUIVictory::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUIFloor::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	pUI = CUISkill::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pUI, E_FAIL);
	FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);

	for (_int i = 0; i < 20; i++)
	{
		pUI = CUIDown::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pUI, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_UI(pUI), E_FAIL);
	}

	Engine::Activate_UI(UITYPE::UI_NORMAL);
	Engine::Activate_UI(UITYPE::UI_PHONE);
	Engine::Activate_UI(UITYPE::UI_PLAYER);

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

	pGameObject = CEffectPool::Create(m_pGraphicDev, (CGameObject * (*)(LPDIRECT3DDEVICE9))CEffectSpark::Create);
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

	pGameObject = CEffectHeal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectHeal", pGameObject), E_FAIL);
	m_mapLayer.insert({ _pLayerTag , pLayer });

	//pGameObject = CEffectBossRobotBooster::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectBossRobotBooster_Right", pGameObject), E_FAIL);
	//m_mapLayer.insert({ _pLayerTag , pLayer });

	//pGameObject = CEffectBossRobotBooster::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectBossRobotBooster_Left", pGameObject), E_FAIL);
	//m_mapLayer.insert({ _pLayerTag , pLayer });


	pGameObject = CFilterFundo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FilterFundo", pGameObject), E_FAIL);

	pGameObject = CEffectPool::Create(m_pGraphicDev, (CGameObject * (*)(LPDIRECT3DDEVICE9))CEffectMinigunShell::Create);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectPool_MinigunShell", pGameObject), E_FAIL);

	pGameObject = CEffectExecutionBlood::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectExecutionBlood", pGameObject), E_FAIL);

	pGameObject = CEffectFlash::Create(m_pGraphicDev, _vec4(0.f, 1.f, 0.f, 0.5f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectGreenFlash", pGameObject), E_FAIL);

	pGameObject = CEffectFlash::Create(m_pGraphicDev, _vec4(1.f, 0.f, 0.f, 0.5f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectRedFlash", pGameObject), E_FAIL);

	pGameObject = CEffectPool::Create(m_pGraphicDev, (CGameObject * (*)(LPDIRECT3DDEVICE9))CEffectSmokeTrail::Create);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectPool_SmokeTrail", pGameObject), E_FAIL);

	pGameObject = CEffectPool::Create(m_pGraphicDev, (CGameObject * (*)(LPDIRECT3DDEVICE9))CEffectExplosion::Create);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectPool_Explosion", pGameObject), E_FAIL);

	pGameObject = CEffectKick::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectKick", pGameObject), E_FAIL);

	pGameObject = CEffectKatanaAttack::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectKatanaAttack", pGameObject), E_FAIL);

	pGameObject = CEffectPool::Create(m_pGraphicDev, (CGameObject * (*)(LPDIRECT3DDEVICE9))CEffectBloodJet::Create);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectPool_BloodJet", pGameObject), E_FAIL);

	pGameObject = CEffectSniperMuzzleFlash::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectSniperMuzzleFlash", pGameObject), E_FAIL);

	pGameObject = CEffectShockWave::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectShockWave", pGameObject), E_FAIL);

	pGameObject = CEffectBigExplosion::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"EffectBigExplosion", pGameObject), E_FAIL);

	pGameObject = CFilterDying::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FilterDying", pGameObject), E_FAIL);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

void CStage::Set_Collision()
{
	Engine::CheckGroup(L"Layer_Player", L"Layer_MonsterBullet");
	Engine::CheckGroup(L"Layer_Player", L"Layer_Item");
	Engine::CheckGroup(L"Layer_Player", L"Layer_Monster");
	Engine::CheckGroup(L"Layer_Player", L"Layer_Door");


	Engine::CheckGroup(L"Layer_PlayerBullet", L"Layer_Monster");
	//Engine::CheckGroup(L"Layer_PlayerBullet", L"Layer_Wall"); //For Particle when collide with Wall & MiniGunBullet // 레이 캐스팅으로 처리함
	Engine::CheckGroup(L"Layer_Player", L"Layer_Wall");
	Engine::CheckGroup(L"Layer_Monster", L"Layer_Wall");
}

CLayer* CStage::Find_Layer(const _tchar* _pLayerTag)
{
	CLayer* pLayer = nullptr;

	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(_pLayerTag));
	pLayer = iter->second;

	return pLayer;
}


void CStage::MapLoad2(CLayer* _pLayer, CLayer* _pLayer2, CLayer* _pLayer3)
{
	HANDLE		hFile = CreateFile(L"../Data/1_TutorialStage.txt",	// 파일 이름까지 포함된 경로
		GENERIC_READ,		// 파일 접근 모드(GENERIC_WRITE : 쓰기, GENERIC_READ : 읽기)
		NULL,				// 공유 방식(파일이 열려 있는 상태에서 다른 프로세스가 오픈 할 때 허가 할 것인가)
		NULL,				// 보안 속성
		OPEN_EXISTING,		// 생성 방식(CREATE_ALWAYS : 파일이 없다면 생성, 있으면 덮어쓰기, OPEN_EXISTING : 파일이 있는 경우에만 불러오기)
		FILE_ATTRIBUTE_NORMAL, // 파일 속성(아무런 속성이 없는 파일로 생성)
		NULL);				// 생성될 파일의 속성을 제공할 템플릿 파일


	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Load File", _T("Fail"), MB_OK);
		return;
	}

	Engine::CLayer* pLayerFloor = CLayer::Create();
	Engine::CLayer* pLayerDoor = CLayer::Create();

	DWORD	dwByte(0);
	_int iNumber_Type(0); // 이게 바닥인지(0) 벽인지(1) 벽TB인지(2) 몬스터인지(3) 문(4) 아이템(5)
	_int iNumber(0); //이게 그럼 몇번째 녀석인지
	_int iTrigger(0); //이게 그럼 몇번째 trigger인지//10.06
	_vec3 pPos{};
	_vec3 pRot{};
	_vec3 pRotation{};
	_vec3 pScale{};

	while (true)
	{
		ReadFile(hFile, &iNumber_Type, sizeof(_int), &dwByte, nullptr); // 이게 바닥인지(0) 벽인지(1) 벽TB인지(2) 몬스터인지(3) 
		ReadFile(hFile, &pPos, sizeof(_vec3), &dwByte, nullptr); // 포지션 값 저장
		ReadFile(hFile, &pRot, sizeof(_vec3), &dwByte, nullptr); // 회전 또는 바라보는 방향 값 저장
		ReadFile(hFile, &iNumber, sizeof(_int), &dwByte, nullptr); // 이게 그럼 몇번째 녀석인지
		ReadFile(hFile, &iTrigger, sizeof(_int), &dwByte, nullptr); // 이게 그럼 몇번째 trigger인지//10.06

		Engine::CGameObject* pGameObject = nullptr;

		if (0 == dwByte)
		{
			break;
		}

		if (iNumber_Type == 0)
		{
			ReadFile(hFile, &pScale, sizeof(_vec3), &dwByte, nullptr); // 크기

			pGameObject = CFloor::Create_Info(m_pGraphicDev, pPos, pRot, pScale, iNumber, iTrigger);//10.06
			NULL_CHECK_RETURN(pGameObject, );
			pLayerFloor->Add_GameObject(L"Floor", pGameObject);
			Engine::Set_Trigger(iTrigger, pGameObject);//10.06
		}
		if (iNumber_Type == 1)
		{

			pGameObject = CWall::Create_InfoNumberDirectionTrigger2(m_pGraphicDev, pPos, pRot, iNumber, iTrigger);//10.06
			NULL_CHECK_RETURN(pGameObject, );
			_pLayer->Add_GameObject(L"Wall", pGameObject);
			Engine::Set_Trigger(iTrigger, pGameObject);//10.06
		}
		if (iNumber_Type == 2)
		{

			pGameObject = CWallTB::Create_InfoNumberDirectionTrigger2(m_pGraphicDev, pPos, pRot, iNumber, iTrigger);//10.06
			NULL_CHECK_RETURN(pGameObject, );
			_pLayer->Add_GameObject(L"WallTB", pGameObject);
			Engine::Set_Trigger(iTrigger, pGameObject);//10.06
		}
		if (iNumber_Type == 3)
		{
			if (iNumber == 1)
			{
				pGameObject = CWhiteSuit::Create(m_pGraphicDev, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer2->Add_GameObject(L"Monster", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.06

			}
			else if (iNumber == 2)
			{
				pGameObject = CShotGun::Create(m_pGraphicDev, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer2->Add_GameObject(L"ShotGun", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.06

			}
			else if (iNumber == 3)
			{
				Engine::CGameObject* pShield = nullptr;
				pShield = CShield::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pShield, );
				_pLayer2->Add_GameObject(L"Shield", pShield);
				pGameObject = CBlackMan::Create(m_pGraphicDev, pShield, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer2->Add_GameObject(L"BlackMan", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.06
				Engine::Set_Trigger(iTrigger, pShield);//10.06
			}
			else if (iNumber == 4)
			{
				pGameObject = CFlyingDrone::Create(m_pGraphicDev, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer2->Add_GameObject(L"FlyingDrone", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.06
			}
			else if (iNumber == 5)
			{
				pGameObject = CSpiderDrone::Create(m_pGraphicDev, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer2->Add_GameObject(L"SpiderDrone", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.06
			}
			else if (iNumber == 6)
			{
				pGameObject = CDogDrone::Create(m_pGraphicDev, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer2->Add_GameObject(L"DogDrone", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.06
			}
			else if (iNumber == 7)
			{
				pGameObject = CBoss_Humanoid::Create(m_pGraphicDev, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer2->Add_GameObject(L"Boss_Humanoid", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.06
			}
			else if (iNumber == 8)
			{
				Engine::CGameObject* pShield = nullptr;
				pShield = CBoss_Shield::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pShield, );
				_pLayer2->Add_GameObject(L"Boss_Shield", pShield);
				pGameObject = CBoss_Robot::Create(m_pGraphicDev, pShield, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer2->Add_GameObject(L"Boss_Robot", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.06
			}
		}
		if (iNumber_Type == 4)
		{
			ReadFile(hFile, &pRotation, sizeof(_vec3), &dwByte, nullptr); // 회전
			ReadFile(hFile, &pScale, sizeof(_vec3), &dwByte, nullptr); // 크기

			pGameObject = CDoor::Create_Infoload(m_pGraphicDev, pPos, pRotation, pScale, iNumber, pRot, iTrigger);//10.06
			NULL_CHECK_RETURN(pGameObject, );
			pLayerDoor->Add_GameObject(L"Door", pGameObject);
			Engine::Set_Trigger(iTrigger, pGameObject);//10.06
		}
		if (iNumber_Type == 5)
		{
			if (iNumber == 1)
			{
				pGameObject = CDrinkMachine::Create(m_pGraphicDev, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer->Add_GameObject(L"DrinkMachine", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.07
			}
			else if (iNumber == 2)
			{
				pGameObject = CAxe::Create(m_pGraphicDev, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer3->Add_GameObject(L"Axe", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.07
			}
			else if (iNumber == 3)
			{
				pGameObject = CKnife::Create(m_pGraphicDev, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer3->Add_GameObject(L"Knife", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.07
			}
		}

	}

	CloseHandle(hFile);
	m_mapLayer.insert({ L"Layer_Floor", pLayerFloor });
	m_mapLayer.insert({ L"Layer_Door", pLayerDoor });

	//MessageBox(g_hWnd, L"LoadTest 완료", _T("성공"), MB_OK);
}


void CStage::Free()
{
	Engine::CScene::Free();
	//CUIManager::DestroyInstance();
	//CCollisionManager::DestroyInstance();
	//CBulletManager::DestroyInstance();
	//CTrigger::DestroyInstance();
	//CDrinkSpawner::DestroyInstance();
	//CParticleSystem::Free()
}



//pGameObject = CWhiteSuit::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Monster", pGameObject), E_FAIL);
	//pGameObject = CShotGun::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ShotGun", pGameObject), E_FAIL);
	//Engine::CGameObject* pShield = nullptr;
	//pShield = CShield::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pShield, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Shield", pShield), E_FAIL);
	//pGameObject = CBlackMan::Create(m_pGraphicDev, pShield);
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
	//pGameObject = CBoss_Humanoid::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Humanoid", pGameObject), E_FAIL);
	//pGameObject = CBoss_Robot::Create(m_pGraphicDev);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Robot", pGameObject), E_FAIL);