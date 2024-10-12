#include "pch.h"
#include "..\Header\BuildStage.h"
#include "..\Header\CutScene.h"
#include "Export_Utility.h"
#include "Export_System.h"
#include "..\Header\DynamicCamera.h"
// �Ժ� (����Ʈ �ش��ϴ� Ŭ���� ��� ����)
#include "../Header/Header_Effect.h"
#include "../Header/FilterFundo.h"

// ����
#include "../Header/UINormal.h"
#include "../Header/UIPlus.h"
#include "../Header/UIShop.h"
#include "../Header/UIInventory.h"
#include "../Header/UIMisterBullet.h"
#include "../Header/UIRoboto.h"
#include "../Header/UIFreeCam.h"
#include "../Header/UIScreen.h"

CBuildStage::CBuildStage(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CScene(_pGraphicDev)
	, m_pPlayer(nullptr)
	, m_hVideoHandle(nullptr)
	, m_bVideoPlaying(false)
{
}

CBuildStage::~CBuildStage()
{
}

CBuildStage* CBuildStage::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CBuildStage* pStage = new CBuildStage(_pGraphicDev);

	if (FAILED(pStage->Ready_Scene()))
	{
		Safe_Release(pStage);
		MSG_BOX("Stage Create Failed");
		return nullptr;
	}

	return pStage;
}

HRESULT CBuildStage::Ready_Scene()
{
	CCollisionManager::GetInstance()->Free();
	CBulletManager::GetInstance()->Free();
	CTrigger::GetInstance()->Free();
	CDrinkSpawner::GetInstance()->Free();



	FAILED_CHECK_RETURN(Ready_LightInfo(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Player(L"Layer_Player"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_MonsterBullet(L"Layer_MonsterBullet"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_Effect(L"Layer_Effect"), E_FAIL);

	//MapLoad(Find_Layer(L"Layer_GameLogic"));
	//ChangeMapLoad();
	Set_Collision();

	if (Engine::Get_ListUI(UITYPE::UI_MISTERBULLET)->empty())
		Engine::Activate_UI(UITYPE::UI_MISTERBULLET);


	Engine::Calculate_Trigger();//10.06

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

_int CBuildStage::Update_Scene(const _float& _fTimeDelta)
{
	// �ݶ��̴� OnOff
	if (Engine::Key_Press(DIK_F1))
		Engine::Toggle_Collider();

	// �����н��� OnOff
	if (Engine::Key_Press(DIK_F2))
	{
		_uint iID = ((_uint)Engine::Get_ControllerID() + 1) % (_uint)CONTROLLERID::CONTROL_END;

		Engine::Set_ControllerID((CONTROLLERID)iID);

		if (Engine::Get_ControllerID() == CONTROLLERID::CONTROL_PLAYER)
			Engine::Deactivate_UI(UITYPE::UI_FREECAM);
		else
			Engine::Activate_UI(UITYPE::UI_FREECAM);
	}

	// �κ��丮 OnOff
	if (Engine::Key_Press(DIK_F3))
		if (Engine::Get_ListUI(UITYPE::UI_INVENTORY)->empty())
			Engine::Stop_Timer(!Engine::Get_Stop());

	// ų�α� ����
	if (Engine::Key_Press(DIK_F4))
	{
		_vec3 vPos = { (float)(rand() % 10), 1.f, (float)(rand() % 10) };
		_int iIndex = rand() % 13;

		CUI* pUI = Engine::Activate_UI(UITYPE::UI_PLUS);
		static_cast<CUIPlus*>(pUI)->Init(vPos, (CUIPlus::UI_PLUS)iIndex);
	}

	// ���� OnOff
	if (Engine::Key_Press(DIK_F5))
	{
		if (Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
			Engine::Activate_UI(UITYPE::UI_SHOP);
		else
			Engine::Deactivate_UI(UITYPE::UI_SHOP);
	}

	// MisterBullet ���� UI OnOff
	if (Engine::Key_Press(DIK_F6))
	{
		if (Engine::Get_ListUI(UITYPE::UI_MISTERBULLET)->empty())
			Engine::Activate_UI(UITYPE::UI_MISTERBULLET);
		else
			Engine::Deactivate_UI(UITYPE::UI_MISTERBULLET);
	}

	// MisterBullet ���� ī��Ʈ Up
	//if (!Engine::Get_ListUI(UITYPE::UI_MISTERBULLET)->empty() &&
	//	Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB))
	//	static_cast<CUIMisterBullet*>(Engine::Get_ListUI(UITYPE::UI_MISTERBULLET)->front())->Add_Count();

	// MisterBullet �������� HP ����
	if (!Engine::Get_ListUI(UITYPE::UI_MISTERBULLET)->empty())
	{
		if (Engine::Key_Hold(DIK_LEFTARROW))
			m_pPlayer->Set_PlayerHP(m_pPlayer->Get_PlayerHP() - 1.f);
		else if (Engine::Key_Hold(DIK_RIGHTARROW))
			m_pPlayer->Set_PlayerHP(m_pPlayer->Get_PlayerHP() + 1.f);
	}

	// ���� UI
	if (Engine::Key_Press(DIK_F8))
	{
		if (Engine::Get_ListUI(UITYPE::UI_SCREEN)->empty())
		{
			Engine::Activate_UI(UITYPE::UI_SCREEN);
			static_cast<CUIScreen*>(Engine::Get_ListUI(UITYPE::UI_SCREEN)->front())->Set_FloorTime(Engine::Get_Elapsed());
		}
		else static_cast<CUIScreen*>(Engine::Get_ListUI(UITYPE::UI_SCREEN)->front())->Set_Return(true);
	}

	// �κ��丮 OnOff
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

	if (Engine::Key_Press(DIK_RETURN))
	{
		Engine::CScene* pStage = CCutScene::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pStage, -1);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage), E_FAIL);

		return 0;
	}

	if (5 < dynamic_cast<CBoss_Humanoid*>(this->Get_GameObject(L"Layer_Monster", L"Boss_Humanoid"))->Get_BossKillCount())
	{

		Engine::CScene* pStage = CCutScene::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pStage, -1);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pStage), E_FAIL);

		return 0;

	}

	_int iExit = Engine::CScene::Update_Scene(_fTimeDelta);
	Engine::Update_Bullet(_fTimeDelta); //Jonghan Change

	return iExit;
}

void CBuildStage::LateUpdate_Scene()
{
	Engine::CScene::LateUpdate_Scene();
	Engine::LateUpdate_Bullet(); //Jonghan Change // Is it Right? becuz BulletManager
}

void CBuildStage::Render_Scene()
{
}

HRESULT CBuildStage::Ready_LightInfo()
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

HRESULT CBuildStage::Ready_Layer_Environment(const _tchar* _pLayerTag)
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

	pGameObject = CSkyBox::Create(m_pGraphicDev, 5);//5=���ù��,7=����
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ComponentCamera", Engine::CComponentCamera::Create(m_pGraphicDev)), E_FAIL);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

HRESULT CBuildStage::Ready_Layer_GameLogic(const _tchar* _pLayerTag)
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

	m_mapLayer.insert({ L"Layer_Wall" , pLayer });
	m_mapLayer.insert({ L"Layer_Monster" , pLayer2 });
	m_mapLayer.insert({ L"Layer_Item" , pLayer3 });

	return S_OK;
}

HRESULT CBuildStage::Ready_Layer_Player(const _tchar* _pLayerTag)
{
	Engine::CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CPlayer::Create(m_pGraphicDev, _vec3{ 25.5f, 1.f, 1.f }, TRUE);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);
	m_pPlayer = static_cast<CPlayer*>(pGameObject);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

HRESULT CBuildStage::Ready_Layer_MonsterBullet(const _tchar* _pLayerTag)
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
	for (_int i = 0; i < 8; ++i)
	{
		Engine::CBullet* pHead = nullptr;
		pHead = CHumanoidHead::Create(m_pGraphicDev);

		NULL_CHECK_RETURN(pHead, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_HumanoidHead(pHead), E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"HumanoidHead", pHead), E_FAIL);
	}

	for (_int i = 0; i < 1; ++i)
	{
		Engine::CBullet* pBoss_Humanoid_Laser = nullptr;
		pBoss_Humanoid_Laser = CBoss_Humanoid_Laser::Create(m_pGraphicDev);

		NULL_CHECK_RETURN(pBoss_Humanoid_Laser, E_FAIL);
		FAILED_CHECK_RETURN(Engine::Add_Boss_Humanoid_Laser(pBoss_Humanoid_Laser), E_FAIL);
		FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Boss_Humanoid_Laser", pBoss_Humanoid_Laser), E_FAIL);
	}
	m_mapLayer.insert({ _pLayerTag , pLayer });
	m_mapLayer.insert({ L"Layer_PlayerBullet" , pLayer2 });

	return S_OK;
}

HRESULT CBuildStage::Ready_Layer_UI(const _tchar* _pLayerTag)
{
	return S_OK;
}

// kyubin
HRESULT CBuildStage::Ready_Layer_Effect(const _tchar* _pLayerTag)
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

	pGameObject = CFilterDying::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"FilterDying", pGameObject), E_FAIL);

	m_mapLayer.insert({ _pLayerTag , pLayer });

	return S_OK;
}

void CBuildStage::PlayVideo(HWND _hWnd, const wstring& _strFilePath)
{
	if (m_bVideoPlaying)
		return;

	m_hVideoHandle = MCIWndCreate(_hWnd,
		NULL,
		WS_CHILD |
		WS_VISIBLE |
		MCIWNDF_NOPLAYBAR, _strFilePath.c_str());

	if (m_hVideoHandle == NULL)
	{
		MessageBox(_hWnd, L"Fail Create Video.", L"Error", MB_OK);
		return;
	}

	MoveWindow(m_hVideoHandle, 0, 0, WINCX, 720, FALSE);

	m_bVideoPlaying = true;
	MCIWndPlay(m_hVideoHandle);
	HDC dc = GetDC(_hWnd);
	HDC memDC = CreateCompatibleDC(dc);
	HBITMAP hBitmap = CreateCompatibleBitmap(dc, WINCX, WINCY);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

	Rectangle(dc, 0, 0, WINCX, WINCY);
	BitBlt(dc, 0, 0, WINCX, WINCY, memDC, 0, 0, SRCCOPY);
	while (MCIWndGetLength(m_hVideoHandle) > MCIWndGetPosition(m_hVideoHandle))
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			MCIWndClose(m_hVideoHandle);
			m_bVideoPlaying = false;
			break;
		}

	}

	SelectObject(memDC, hOldBitmap);
	ReleaseDC(_hWnd, memDC);
	ReleaseDC(_hWnd, dc);

	m_bVideoPlaying = false;

	MCIWndClose(m_hVideoHandle);
}

void CBuildStage::Set_Collision()
{
	Engine::CheckGroup(L"Layer_Player", L"Layer_MonsterBullet");
	Engine::CheckGroup(L"Layer_Player", L"Layer_Item");
	Engine::CheckGroup(L"Layer_Player", L"Layer_Monster");
	Engine::CheckGroup(L"Layer_Player", L"Layer_Door");


	Engine::CheckGroup(L"Layer_PlayerBullet", L"Layer_Monster");
	//Engine::CheckGroup(L"Layer_PlayerBullet", L"Layer_Wall"); //For Particle when collide with Wall & MiniGunBullet
	Engine::CheckGroup(L"Layer_Player", L"Layer_Wall");
	Engine::CheckGroup(L"Layer_Monster", L"Layer_Wall");
}

CLayer* CBuildStage::Find_Layer(const _tchar* _pLayerTag)
{
	CLayer* pLayer = nullptr;

	auto iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(_pLayerTag));
	pLayer = iter->second;

	return pLayer;
}


void CBuildStage::MapLoad2(CLayer* _pLayer, CLayer* _pLayer2, CLayer* _pLayer3)
{
	HANDLE		hFile = CreateFile(L"../Data/4_BuildingStage.txt",	// ���� �̸����� ���Ե� ���
		GENERIC_READ,		// ���� ���� ���(GENERIC_WRITE : ����, GENERIC_READ : �б�)
		NULL,				// ���� ���(������ ���� �ִ� ���¿��� �ٸ� ���μ����� ���� �� �� �㰡 �� ���ΰ�)
		NULL,				// ���� �Ӽ�
		OPEN_EXISTING,		// ���� ���(CREATE_ALWAYS : ������ ���ٸ� ����, ������ �����, OPEN_EXISTING : ������ �ִ� ��쿡�� �ҷ�����)
		FILE_ATTRIBUTE_NORMAL, // ���� �Ӽ�(�ƹ��� �Ӽ��� ���� ���Ϸ� ����)
		NULL);				// ������ ������ �Ӽ��� ������ ���ø� ����


	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"Load File", _T("Fail"), MB_OK);
		return;
	}

	Engine::CLayer* pLayerFloor = CLayer::Create();
	Engine::CLayer* pLayerDoor = CLayer::Create();

	DWORD	dwByte(0);
	_int iNumber_Type(0); // �̰� �ٴ�����(0) ������(1) ��TB����(2) ��������(3) ��(4) ������(5)
	_int iNumber(0); //�̰� �׷� ���° �༮����
	_int iTrigger(0); //�̰� �׷� ���° trigger����//10.06
	_vec3 pPos{};
	_vec3 pRot{};

	while (true)
	{
		ReadFile(hFile, &iNumber_Type, sizeof(_int), &dwByte, nullptr); // �̰� �ٴ�����(0) ������(1) ��TB����(2) ��������(3) 
		ReadFile(hFile, &pPos, sizeof(_vec3), &dwByte, nullptr); // ������ �� ����
		ReadFile(hFile, &pRot, sizeof(_vec3), &dwByte, nullptr); // ȸ�� �Ǵ� �ٶ󺸴� ���� �� ����
		ReadFile(hFile, &iNumber, sizeof(_int), &dwByte, nullptr); // �̰� �׷� ���° �༮����
		ReadFile(hFile, &iTrigger, sizeof(_int), &dwByte, nullptr); // �̰� �׷� ���° trigger����//10.06

		Engine::CGameObject* pGameObject = nullptr;

		if (0 == dwByte)
		{
			break;
		}

		if (iNumber_Type == 0)
		{
			pGameObject = CFloor::Create_InfoNumberTrigger2(m_pGraphicDev, pPos, pRot, iNumber, iTrigger);//10.06


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
				pGameObject = CBoss_Robot::Create(m_pGraphicDev, pPos);
				NULL_CHECK_RETURN(pGameObject, );
				_pLayer2->Add_GameObject(L"Boss_Robot", pGameObject);
				Engine::Set_Trigger(iTrigger, pGameObject);//10.06
			}
		}
		if (iNumber_Type == 4)
		{

			pGameObject = CDoor::Create_InfoNumberDirectionTrigger2(m_pGraphicDev, pPos, pRot, iNumber, iTrigger);//10.06
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

}


void CBuildStage::Free()
{
	Engine::CScene::Free();
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