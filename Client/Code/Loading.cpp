#include "pch.h"
#include "..\Header\Loading.h"
#include "Export_Utility.h"

CLoading::CLoading(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
	, m_bFinish(false)
	, m_hThread(nullptr)
	, m_eID(LOADINGID::LOADING_END)
	, m_szLoading(L"")
{
	ZeroMemory(&m_tCrt, sizeof(CRITICAL_SECTION));

	m_pGraphicDev->AddRef();
}

CLoading::~CLoading()
{
}

CLoading* CLoading::Create(LPDIRECT3DDEVICE9 _pGraphicDev, LOADINGID _eID)
{
	CLoading* pLoading = new CLoading(_pGraphicDev);

	if (FAILED(pLoading->Ready_Loading(_eID)))
	{
		Safe_Release(pLoading);
		return nullptr;
	}

	return pLoading;
}

HRESULT CLoading::Ready_Loading(LOADINGID _eLoading)
{
	InitializeCriticalSection(&m_tCrt);

	m_eID = _eLoading;

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, Thread_Main, this, 0, NULL);

	return S_OK;
}

_uint CLoading::Loading_Stage()
{
	lstrcpy(m_szLoading, L"Etc Loading................");

	// Etc
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Calculator", Engine::CCalculator::Create(m_pGraphicDev)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ComponentCamera", Engine::CComponentCamera::Create(m_pGraphicDev)), E_FAIL);

	//Beomseung
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Left_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Right_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Leg_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Body_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Animator", Engine::CAnimator::Create(m_pGraphicDev)), E_FAIL);

	// Kyubin
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Effect", Engine::CEffect::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcSpriteTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Collider", Engine::CCollider::Create(m_pGraphicDev)), E_FAIL);


	lstrcpy(m_szLoading, L"Buffer Loading................");

	// Buffer
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcCol", Engine::CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTex", Engine::CTerrainTex::Create(m_pGraphicDev, VTXCNTX, VTXCNTZ, VTXITV)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTex", Engine::CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	//YuBhin: Floor Buffer - FirstFloor
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FloorTex", Engine::CFloorTex::Create(m_pGraphicDev)), E_FAIL);
	//YuBhin: Wall Buffer - AcidWall
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WallTex", Engine::CWallTex::Create(m_pGraphicDev)), E_FAIL);

	//Beomseung
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RightArmBuffer", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeftArmBuffer", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Leg_Buffer", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);


	lstrcpy(m_szLoading, L"Texture Loading................");

	// Texture
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Ma.jpg", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerTex2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Player0.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	//Jonghan Stage Change Start
	// WhiteMan
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Idle/Test/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterAttackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Attack/Test/%d.png", TEXTUREID::TEX_NORMAL, 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterHeadShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/HeadShot/Test/%d.png", TEXTUREID::TEX_NORMAL, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterBullShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/BullShot/Test/%d.png", TEXTUREID::TEX_NORMAL, 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterPushOneTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Push/Push_1/Test/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterPushTwoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Push/Push_2/Test/%d.png", TEXTUREID::TEX_NORMAL, 22)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterShotOneTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Shot/Shot_1/Test/%d.png", TEXTUREID::TEX_NORMAL, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterShotTwoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Shot/Shot_2/Test/%d.png", TEXTUREID::TEX_NORMAL, 19)), E_FAIL);
	////ShotGunMan
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunIdleTex", Engine::CTexture::Create(m_pGraphicDev,     L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Idle/Test/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunAttackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Attack/Test/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunHeadShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/HeadShot/Test/%d.png", TEXTUREID::TEX_NORMAL, 20)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunBullShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/BullShot/Test/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunPushOneTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Push/Push_1/Test/%d.png", TEXTUREID::TEX_NORMAL, 24)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunPushTwoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Push/Push_2/Test/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunShotOneTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Shot/Shot_1/Test/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunShotTwoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Shot/Shot_2/Test/%d.png", TEXTUREID::TEX_NORMAL, 18)), E_FAIL);
	//////BlackMan
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManIdleTex", Engine::CTexture::Create(m_pGraphicDev,   L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Idle/Test/%d.png", TEXTUREID::TEX_NORMAL, 9)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManAttackTex", Engine::CTexture::Create(m_pGraphicDev,   L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Attack/Test/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManShieldAttackTex", Engine::CTexture::Create(m_pGraphicDev,   L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Shield/Shield_Shot/Test/%d.png", TEXTUREID::TEX_NORMAL, 12)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManHeadShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/HeadShot/Test/%d.png", TEXTUREID::TEX_NORMAL, 21)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManBullShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/BullShot/Test/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManPushOneTex", Engine::CTexture::Create(m_pGraphicDev,  L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Push/Push_1/Test/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManPushTwoTex", Engine::CTexture::Create(m_pGraphicDev,  L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Push/Push_2/Test/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManShotOneTex", Engine::CTexture::Create(m_pGraphicDev,  L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Shot/Shot_1/Test/%d.png", TEXTUREID::TEX_NORMAL, 24)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManShotTwoTex", Engine::CTexture::Create(m_pGraphicDev,  L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Shot/Shot_2/Test/%d.png", TEXTUREID::TEX_NORMAL, 15)), E_FAIL);
	////// FlyingDrone -> Drone's Action_Count is 5, So i have to change the count of list
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlyingDroneAttackTex", Engine::CTexture::Create(m_pGraphicDev,   L"../Bin/Resource/Texture/MMJ_Monster/Drone_Flying/Attack/Test/%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlyingDroneIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Flying/Idle/Test/%d.png", TEXTUREID::TEX_NORMAL, 12)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlyingDroneDamagedTex", Engine::CTexture::Create(m_pGraphicDev,  L"../Bin/Resource/Texture/MMJ_Monster/Drone_Flying/Damaged/Test/%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	//////SpiderDrone
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpiderDroneAttackTex", Engine::CTexture::Create(m_pGraphicDev,  L"../Bin/Resource/Texture/MMJ_Monster/Drone_Spider/Attack/Test/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpiderDroneIdleTex", Engine::CTexture::Create(m_pGraphicDev,    L"../Bin/Resource/Texture/MMJ_Monster/Drone_Spider/Idle/Test/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpiderDroneWalkTex", Engine::CTexture::Create(m_pGraphicDev,    L"../Bin/Resource/Texture/MMJ_Monster/Drone_Spider/Walk/Test/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpiderDroneDamagedTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Spider/Damaged/Test/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	//////DogDrone
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneAttackTex", Engine::CTexture::Create(m_pGraphicDev,   L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/Attack/Test/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneIdleTex", Engine::CTexture::Create(m_pGraphicDev,     L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/Idle/Test/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneWalkTex", Engine::CTexture::Create(m_pGraphicDev,     L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/Walk/Test/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneDamagedTex", Engine::CTexture::Create(m_pGraphicDev,  L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/Damaged/Test/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneHeadShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/HeadShot/Test/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	//Ammo (Monster Bullet)
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AmmoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Bullet/%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	//Shield (Monster Shield)
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShieldTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Shield/Shield_fall/Test/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	//Boss_Humanoid
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossHumanoidSpawnTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Humanoid/Spawn/Test/%d.png", TEXTUREID::TEX_NORMAL, 17)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossHumanoidAttackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Humanoid/Attack/Test/%d.png", TEXTUREID::TEX_NORMAL, 21)), E_FAIL);
	//Jonghan Stage Change End
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Terrain0.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TerrainTexture2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/Grass_%d.tga", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkyBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/burger%d.dds", TEXTUREID::TEX_CUBE, 4)), E_FAIL);
	
	//Beomseung
	//Pistol
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Pistol_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Pistol/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Pistol_Shoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Pistol/Shoot/%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Pistol_Reload", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Pistol/Reload/%d.png", TEXTUREID::TEX_NORMAL, 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Pistol_Change", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Pistol/PistolChange/%d.png", TEXTUREID::TEX_NORMAL, 17)), E_FAIL);
	//Rifle
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Rifle_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Rifle/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Rifle_Shoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Rifle/Shoot/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Rifle_Reload", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Rifle/Reload/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	//Shotgun
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shotgun_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Shotgun/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shotgun_Shoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Shotgun/Shoot/%d.png", TEXTUREID::TEX_NORMAL, 15)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shotgun_Reload", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Shotgun/Reload/%d.png", TEXTUREID::TEX_NORMAL, 11)), E_FAIL);
	//Sniper
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sniper_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Sniper/Zoom/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sniper_Shoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Sniper/Shoot/%d.png", TEXTUREID::TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sniper_Reload", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Sniper/Reload/%d.png", TEXTUREID::TEX_NORMAL, 19)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeftArmTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerLeft/Left%d.png", TEXTUREID::TEX_NORMAL, 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeftDrink", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerDrink/%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Leg_Kick", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerKick/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Leg_Sliding", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerSliding/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Leg_Idle", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerLeg/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 1)), E_FAIL);

	//YuBhin: Floor Texture - FirstFloor
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FirstFloor", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Floor/Floor1.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	//YuBhin: Floor Texture - Attribute Acid
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AcidFloor", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Floor/ACID A LUM.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	//YuBhin: Wall Texture - Attribute Acid
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AcidWall", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_wall/ACID C.png", TEXTUREID::TEX_NORMAL)), E_FAIL);

	// Kyubin
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MuzzleFlashTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Flare1_Edit.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerBloodTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/PlayerBlood/BLOOD (%d).png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BloodSplaterTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/EDITED Blood Splater - 1.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FanSpreadTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Lightning09.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CircleLinesTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/CircleLines/CircleLines (%d).png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FanSpreadTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/fan spread.png", TEXTUREID::TEX_NORMAL)), E_FAIL);

	// ¿¬¿í
	// UI
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIPlus", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/White/White_sec.png", TEXTUREID::TEX_NORMAL)), E_FAIL);

	lstrcpy(m_szLoading, L"Loading Complete!!");

	m_bFinish = true;

	return 0;
}

unsigned int CLoading::Thread_Main(void* _pArg)
{
	CLoading* pLoading = reinterpret_cast<CLoading*>(_pArg);

	_uint iFlag(0);

	EnterCriticalSection(pLoading->Get_Crt());

	switch (pLoading->Get_LoadingID())
	{
	case LOADINGID::LOADING_STAGE:
		iFlag = pLoading->Loading_Stage();
		break;
	case LOADINGID::LOADING_BOSS:
		break;
	default :
		break;
	}

	LeaveCriticalSection(pLoading->Get_Crt());

	return iFlag;
}

void CLoading::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_tCrt);

	Safe_Release(m_pGraphicDev);
}