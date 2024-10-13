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
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Calculator", Engine::CCalculator::Create(m_pGraphicDev)), E_FAIL);

	//Beomseung
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Left_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Right_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Leg_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Body_Transform", Engine::CTransform::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Animator", Engine::CAnimator::Create(m_pGraphicDev)), E_FAIL);

	// Kyubin
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Effect", Engine::CEffect::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_RcSpriteTex", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ParticleSystem", Engine::CParticleSystem::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ParticleSpark", Engine::CParticleSpark::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Collider", Engine::CCollider::Create(m_pGraphicDev)), E_FAIL);

	lstrcpy(m_szLoading, L"Buffer Loading................");

	// Buffer
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_TriCol", Engine::CTriCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_HitBufferCom", Engine::CRcCol::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CubeTex", Engine::CCubeTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MissileTex", Engine::CMissileTex::Create(m_pGraphicDev)), E_FAIL);
	//YuBhin: Floor,Wall
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FloorTex", Engine::CFloorTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WallTex", Engine::CWallTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_WallTBTex", Engine::CWallTBTex::Create(m_pGraphicDev)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_GuideTex", Engine::CGuideTex::Create(m_pGraphicDev, VTXTILEX, VTXTILEZ, VTXITV)), E_FAIL);

	//Beomseung
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerBuffer", Engine::CRcTex::Create(m_pGraphicDev)), E_FAIL);

	lstrcpy(m_szLoading, L"Texture Loading................");

	//Jonghan Stage Change Start
	// WhiteMan
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Idle/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterAttackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Attack/%d.png", TEXTUREID::TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterAttackDelayTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Attack_Delay/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterHeadShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/HeadShot/%d.png", TEXTUREID::TEX_NORMAL, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterBullShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/BullShot/%d.png", TEXTUREID::TEX_NORMAL, 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterPushOneTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Push/Push_1/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterPushTwoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Push/Push_2/%d.png", TEXTUREID::TEX_NORMAL, 22)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterShotOneTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Shot/Shot_1/%d.png", TEXTUREID::TEX_NORMAL, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterShotTwoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Shot/Shot_2/%d.png", TEXTUREID::TEX_NORMAL, 19)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterKatanaDownTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Katana/Down/%d.png", TEXTUREID::TEX_NORMAL, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterKatanaUpFrontTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Katana/Up/Front/%d.png", TEXTUREID::TEX_NORMAL, 19)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterKatanaUpBackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Katana/Up/Back/%d.png", TEXTUREID::TEX_NORMAL, 16)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MonsterExecutionTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/WhiteMan/Execution/%d.png", TEXTUREID::TEX_NORMAL, 15)), E_FAIL);
	////ShotGunMan
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Idle/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunAttackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Attack/%d.png", TEXTUREID::TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunAttackDelayTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Attack_Delay/%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunHeadShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/HeadShot/%d.png", TEXTUREID::TEX_NORMAL, 20)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunBullShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/BullShot/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunPushOneTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Push/Push_1/%d.png", TEXTUREID::TEX_NORMAL, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunPushTwoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Push/Push_2/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunShotOneTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Shot/Shot_1/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunShotTwoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Shot/Shot_2/%d.png", TEXTUREID::TEX_NORMAL, 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunKatanaDownTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Katana/Down/%d.png", TEXTUREID::TEX_NORMAL, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunKatanaUpTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Katana/Up/Back/%d.png", TEXTUREID::TEX_NORMAL, 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShotGunExecutionTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/ShotGun/Execution/%d.png", TEXTUREID::TEX_NORMAL, 15)), E_FAIL);
	//////BlackMan
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Idle/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManShieldIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Idle_Shield/%d.png", TEXTUREID::TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManAttackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Attack/%d.png", TEXTUREID::TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManAttackDelayTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Attack_Delay/%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManShieldAttackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Shield/Shield_Shot/%d.png", TEXTUREID::TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManHeadShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/HeadShot/%d.png", TEXTUREID::TEX_NORMAL, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManBullShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/BullShot/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManPushOneTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Push/Push_1/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManPushTwoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Push/Push_2/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManShotOneTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Shot/Shot_1/%d.png", TEXTUREID::TEX_NORMAL, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManShotTwoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Shot/Shot_2/%d.png", TEXTUREID::TEX_NORMAL, 15)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManKatanaDownTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Katana/Down/%d.png", TEXTUREID::TEX_NORMAL, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManKatanaUpTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Katana/Up/Back/%d.png", TEXTUREID::TEX_NORMAL, 13)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BlackManExecutionTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Execution/%d.png", TEXTUREID::TEX_NORMAL, 15)), E_FAIL);
	////// FlyingDrone -> Drone's Action_Count is 5, So i have to change the count of list
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlyingDroneAttackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Flying/Attack/%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlyingDroneIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Flying/Idle/%d.png", TEXTUREID::TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlyingDroneDamagedTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Flying/Damaged/%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlyingDroneKatanaRightTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Flying/Katana/Right/%d.png", TEXTUREID::TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FlyingDroneKatanaLeftTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Flying/Katana/Left/%d.png", TEXTUREID::TEX_NORMAL, 9)), E_FAIL);
	//////SpiderDrone
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpiderDroneAttackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Spider/Attack/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpiderDroneIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Spider/Idle/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpiderDroneWalkTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Spider/Walk/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpiderDroneDamagedTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Spider/Damaged/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpiderDroneKatanaRightTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Spider/Katana/Right/%d.png", TEXTUREID::TEX_NORMAL, 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SpiderDroneKatanaLeftTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Spider/Katana/Left/%d.png", TEXTUREID::TEX_NORMAL, 11)), E_FAIL);
	//////DogDrone
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneAttackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/Attack/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/Idle/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneWalkTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/Walk/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneDamagedTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/Damaged/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneHeadShotTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/HeadShot/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneKatanaRightTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/Katana/Right/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DogDroneKatanaLeftTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Drone_Dog/Katana/Left/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	//Ammo (Monster Bullet)
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_AmmoTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Bullet/%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Missile", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Bullet/Missile/0.dds", TEXTUREID::TEX_CUBE)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Laser", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Bullet/Laser/0.dds", TEXTUREID::TEX_CUBE)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossHumanoid_Laser", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Bullet/Boss_Humanoid_Laser/0.dds", TEXTUREID::TEX_CUBE)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobot_Bullet", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Bullet/Boss_Bullet/%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiniGunTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Bullet/MiniGun/%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	//Shield (Monster Shield)
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ShieldTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/BlackMan/Shield/Shield_fall/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Boss_ShieldTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Shield/ShieldImg/Shield01.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	//Boss_Humanoid
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossHumanoidSpawnTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Humanoid/Spawn/%d.png", TEXTUREID::TEX_NORMAL, 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossHumanoidAttackTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Humanoid/Attack/%d.png", TEXTUREID::TEX_NORMAL, 21)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossHumanoidIdle", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Humanoid/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossHumanoidHead", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Humanoid_Head/%d.png", TEXTUREID::TEX_NORMAL, 20)), E_FAIL);
	//Boss_Robot
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Idle/Normal/%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotDamagedIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Idle/Damaged/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotMoreDamagedIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Idle/MoreDamaged/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotAttackOneHandTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Attack/One_Hand/Normal/%d.png", TEXTUREID::TEX_NORMAL, 20)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotAttackTwoHandTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Attack/Two_Hand/Normal/%d.png", TEXTUREID::TEX_NORMAL, 25)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotDamagedAttackOneHandTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Attack/One_Hand/Damaged/%d.png", TEXTUREID::TEX_NORMAL, 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotDamagedAttackTwoHandTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Attack/Two_Hand/Damaged/%d.png", TEXTUREID::TEX_NORMAL, 24)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotHitTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Hit/Normal/%d.png", TEXTUREID::TEX_NORMAL, 14)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotDamagedHitTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Hit/Damaged/%d.png", TEXTUREID::TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotShieldTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Shield/Normal/%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotTalkingTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Talking/%d.png", TEXTUREID::TEX_NORMAL, 18)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BossRobotDamagedShieldTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/Boss/Robot/Shield/Damaged/%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	//Drop_Item
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DropKnifeTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Item/Drop_Item/Knife.png", TEXTUREID::TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DropAxeTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Item/Drop_Item/Axe.png", TEXTUREID::TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DropDrinkTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Item/Drop_Item/Drink.png", TEXTUREID::TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DropMachineIdleTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Item/DrinkMachine/Idle/%d.png", TEXTUREID::TEX_NORMAL, 1)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DropMachineActiveTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Item/DrinkMachine/Active/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_DropMachineBrokenTex", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Item/DrinkMachine/Broken/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	//Jonghan Stage Change End

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SkyBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/burger%d.dds", TEXTUREID::TEX_CUBE, 8)), E_FAIL);

	//Beomseung
	// //Execution
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Right_KnifeExecution", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerFinish/Right/Knife/%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	//Pistol
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Pistol_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Pistol/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Pistol_Shoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Pistol/Shoot/%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Pistol_Reload", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Pistol/Reload/%d.png", TEXTUREID::TEX_NORMAL, 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Pistol_Change", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Pistol/PistolChange/%d.png", TEXTUREID::TEX_NORMAL, 17)), E_FAIL);
	//Rifle
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Rifle_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Rifle/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Rifle_Shoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Rifle/Shoot/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Rifle_Reload", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Rifle/Reload/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Rifle_Change", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Rifle/RifleChange/%d.png", TEXTUREID::TEX_NORMAL, 14)), E_FAIL);
	//Shotgun
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shotgun_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Shotgun/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shotgun_Shoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Shotgun/Shoot/%d.png", TEXTUREID::TEX_NORMAL, 15)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shotgun_Reload", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Shotgun/Reload/%d.png", TEXTUREID::TEX_NORMAL, 11)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Shotgun_Change", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Shotgun/ShotgunChange/%d.png", TEXTUREID::TEX_NORMAL, 15)), E_FAIL);
	//Sniper
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sniper_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Sniper/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sniper_Shoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Sniper/Shoot/%d.png", TEXTUREID::TEX_NORMAL, 12)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sniper_Reload", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Sniper/Reload/%d.png", TEXTUREID::TEX_NORMAL, 19)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sniper_ZoomIn", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Sniper/ZoomIn/%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sniper_ZoomIng", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Sniper/ZoomIng/%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Sniper_ZoomOut", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Sniper/ZoomOut/%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	//Katana
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Katana_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Katana/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Katana_Shoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Katana/Attack/%d.png", TEXTUREID::TEX_NORMAL, 27)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Katana_Change", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerRight/Katana/KatanaChange/%d.png", TEXTUREID::TEX_NORMAL, 16)), E_FAIL);
	//MiniGun
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiniGun_GunPoint_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerMiniGun/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiniGun_Body_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerMiniGun/Body/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiniGun_Body_Change", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerMiniGun/Body/Change/0.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiniGun_GunPoint_Shoot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerMiniGun/Shoot/%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiniGun_GunPoint_Change", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerMiniGun/Change/%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiniGun_Panel_Change", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerMiniGun/Panel/Change/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MiniGun_Panel_IDLE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerMiniGun/Panel/IDLE/0.png", TEXTUREID::TEX_NORMAL)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeftIdle", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerLeft/Left%d.png", TEXTUREID::TEX_NORMAL, 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeftChange", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerLeft/Change/Left%d.png", TEXTUREID::TEX_NORMAL, 17)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeftDrink", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerDrink/%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LeftExecution", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerFinish/Left/%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Leg_Kick", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerKick/%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Leg_Sliding", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerSliding/%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Leg_Idle", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerLeg/IDLE/%d.png", TEXTUREID::TEX_NORMAL, 1)), E_FAIL);
	//Player Start
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Player_Start", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Player/PlayerStart/%d.png", TEXTUREID::TEX_NORMAL, 9)), E_FAIL);
	//YuBhin: Floor Texture
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Floor", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Floor/%d.png", TEXTUREID::TEX_NORMAL, 32)), E_FAIL);

	// wallTexture
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Wall", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Wall/%d.png", TEXTUREID::TEX_NORMAL, 87)), E_FAIL);

	//Jonghan MonsterTile Texture
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Monster_Map", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Monster/MapTool/%d.png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Item_Map", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Item/MapTool/%d.png", TEXTUREID::TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Door", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Door/%d.png", TEXTUREID::TEX_NORMAL, 23)), E_FAIL);

	// Kyubin
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MuzzleFlashTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Flare1_Edit.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MuzzleTankTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/muzzle_tank.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_PlayerBloodTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/PlayerBlood/BLOOD (%d).png", TEXTUREID::TEX_NORMAL, 8)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BloodSplaterTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/EDITED Blood Splater - 1.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FanSpreadTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Lightning09.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_CircleLinesTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/CircleLines/CircleLines (%d).png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FanSpreadTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/fan spread.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Glow_White", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Glow White.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_LIKES", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/LIKES.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_FUNDOTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/FUNDO/FUNDO (%d).png", TEXTUREID::TEX_NORMAL, 9)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MinigunShellTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Bullet spritesheet v2.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_MinigunCapsuleTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/CAPSULE EXTRA ANIMsheet.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_BloodParticle", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Blood_Particle.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_ExecutionBlood", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Blood/excutionBlood (%d).png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SquareTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Square.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SmokeTrailTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/sprite_toonSmokeTrail_a.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_SmokeTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Smoke/smoke (%d).png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_KickTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Kick.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_KatanaAttackTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Katana/Slash (%d).png", TEXTUREID::TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_KatanaAdditionalTexture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Katana/KatanaAttack.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Glow_2Texture", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Glow_02.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texure_FullCircle", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/fullcircle.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_Texure_Dying", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Effect/Dying.png", TEXTUREID::TEX_NORMAL)), E_FAIL);

	// ¿¬¿í
	// UI_Damage
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIPlus_White", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/White/White_PlusSec_%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_Execution", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_Execute_%d.png", TEXTUREID::TEX_NORMAL, 13)), E_FAIL);
	// UI_Combo
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UICombo_Combo", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_COMBO.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UICombo_Like", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Icon/LIKES.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UICombo_White", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/White/White_%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UICombo_Sec", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/White/White_sec.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UICombo_DWhite", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/DWhite/DWhite_%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UICombo_DWhite_X", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/DWhite/DWhite_X.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Inventory
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIBack", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Upgrade/Screen/BASE.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIOverlay", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Upgrade/Screen/SCREEN_OVERLAY.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UILiveIcon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Icon/LIVE_ICON.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_LiveStream", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_LiveStream.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_Moderator", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_ModeratorUpgrade.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_Moderator_A", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_ModeratorUpgrade_A.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Normal - CrossHair
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UICrossHair", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/CrossHair/CROSSHAIR_%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	// UI_Player - Reload
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIReload_BlackBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Player/Reload/Reload_Box_0.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIReload_RedBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Player/Reload/Reload_Box_1.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIReload_RGBBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Player/Reload/Reload_Box_RGB.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIReload_R", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Player/Reload/Reload_R.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_Reload", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_Reload.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Player - Item
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIItem_RGBBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Player/Item/RGBBox.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIItem_TextBox", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Player/Item/TextBox.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIItem_Outline", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Player/Item/PICKUP_OUTLINE.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIItem_Inner", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Player/Item/PICKUP_INNER.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIItem_Execution", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_Execution.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIItem_Mouse2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_Mouse2.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Player - Emergency
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIEmergency_Number", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Player/Emergency/%d.png", TEXTUREID::TEX_NORMAL, 3)), E_FAIL);
	// UI_Shop
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIShop_Hand_Base", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Shop/Screen/ShopHand_Base.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_Upgrade", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_Upgrade.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIShop_Hand_Start", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Shop/Screen/ShopHand_Start_%d.png", TEXTUREID::TEX_NORMAL, 6)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIShop_Hand_Load", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Shop/Screen/ShopHand_Load_%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIShop_Buzz", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Shop/Screen/INT_%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	// UI_Card
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIUpgrade_Weapon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Upgrade/Card/UPGRADE_WEAPON.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIUpgrade_Boarder_A", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Upgrade/Card/UPGRADE_BORDER_A.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIUpgrade_Boarder", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Upgrade/Card/UPGRADE_BORDER_%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIUpgrade_Card", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Upgrade/Card/UpgradeCard_%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_Purchase", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_Purchase.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Boss - MisterBullet
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIBoss_Count", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Boss/MisterBullet/Boss_Kill_Count.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Boss - Roboto
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIBoss_LifeBar", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Boss/Roboto/BOSS_LIFEBAR.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIBoss_LifeBar_A", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Boss/Roboto/BOSS_LIFEBAR_A.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIBoss_LifeBar_P", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Boss/Roboto/BOSS_LIFEBAR_P.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIBoss_LifeBar_B", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Boss/Roboto/BOSS_LIFEBAR_B.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIBoss_Logo", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Boss/Roboto/LOGO.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIBoss_Battery", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Boss/Roboto/BATTERY_LIFE.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIBoss_Bar", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Boss/Roboto/Bar.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_Roboto", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Boss/Roboto/Text_Roboto.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Boss - SniperHP
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UISniperHP_Box", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Boss/MisterBullet/SniperHP_Box.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UISniperHP_Bar", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Boss/MisterBullet/SniperHP_Bar.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_DRed", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/DRed/DRed_%d.png", TEXTUREID::TEX_NORMAL, 11)), E_FAIL);
	// UI_Phone
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIPhone_Upper", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Phone/Phone_Upper.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIPhone_Card", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Phone/CARD.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIPhone_RGBBack", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Phone/Phone_RGBBack.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIPhone_Back", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Phone/Phone_Back.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIPhone_Moderator_Normal", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Phone/Moderator/Normal/Normal_%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIPhone_Moderator_Hurt", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Phone/Moderator/Hurt/Hurt_%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIPhone_Moderator_HurtBad", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Phone/Moderator/HurtBad/HurtBad_%d.png", TEXTUREID::TEX_NORMAL, 4)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_DBlack", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/DBlack/DBlack_%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIPhone_OFFLINE", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Phone/OFFLINE.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIPhone_BossBack", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Phone/Phone_Boss_Back.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_DYellow", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/DYellow/DYellow_%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_DYellow_Percent", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/DYellow/DYellow_Percent.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_FreeCam
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIIndicator", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Indicator/PIN.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Screen
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Icon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Victory/LIVE_VictoryICON.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Live", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Victory/Text_VictoryStream.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Balloon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Victory/Balloon.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_BackGround", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Victory/BackGround_RGB.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_Victory", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_Victory.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_Omedeto", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_Omedeto.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_Floor", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_Floor.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Bar", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Bar.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_Dot", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/DGreen/DGreen_Dot.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_DGreen", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/DGreen/DGreen_%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIText_DGSec", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/DGreen/DGreen_Sec.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Grid", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/GridBox_Green.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Grid_", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/GridBox_Green_.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Grid__", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/GridBox_Green__.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Fine", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Screen_Fine.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Line", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Line/Line_%d.png", TEXTUREID::TEX_NORMAL, 10)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Logo", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Screen_Logo.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Chat", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Chat/Chat_%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Follow", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/FOLLOW.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Floor", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Floor_%d.png", TEXTUREID::TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Static", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/STATIC_%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Streamer_Face1", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Streamer/Face1/Face1_%d.png", TEXTUREID::TEX_NORMAL, 7)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Streamer_Face2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Streamer/Face2/Face2_%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Streamer_Face3", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Streamer/Face3/Face3_%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Streamer_Face4", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Streamer/Face4/Face4_%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Streamer_Hair1", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Streamer/Hair1/Hair1_%d.png", TEXTUREID::TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Streamer_Hair2", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Streamer/Hair2/Hair2_%d.png", TEXTUREID::TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Streamer_Hair3", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Streamer/Hair3/Hair3_%d.png", TEXTUREID::TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Streamer_Hair4", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Streamer/Hair4/Hair4_%d.png", TEXTUREID::TEX_NORMAL, 3)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_INT", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Phone/INT_%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_Peace", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/Peace.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_ClickMute", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/ClickMute.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_StreamerNumber", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/StreamerNumber.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIScreen_StreamerName", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Screen/StreamerName.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Action
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIAction_BackGround", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Action/Action_BackGround.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIAction_Outline", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Action/Action_Outline.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIAction_Inner", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Action/Action_Inner.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIAction_Icon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Action/Action_Icon_%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIAction_Text", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Action/Action_Text_%d.png", TEXTUREID::TEX_NORMAL, 2)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIAction_Second", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/White/20Second.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Down
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIDown_Black", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Down/Down_Black.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIDown_Second", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Down/6sec.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Floor
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIFloor_BackGround", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Floor/Floor_BackGround.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIFloor_Floor", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/DWhite/DWhite_Floor_%d.png", TEXTUREID::TEX_NORMAL, 5)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIFloorBoss_Danger", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Floor/DANGER.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIFloorBoss_Bar", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Floor/FloorBoss_Bar.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIFloorBoss_Network", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_Network.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIFloorBoss_Timelimit", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_TimeLimit.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIFloorBoss_Zero", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_00.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIFloorBoss_Balloon", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Floor/Floor_Balloon.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Skill
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UISkill_Back", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Skill/Skill_Back.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UISkill_Outline", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Skill/Skill_Outline.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UISkill_Black", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Skill/Skill_Black.png", TEXTUREID::TEX_NORMAL)), E_FAIL);
	// UI_Ending
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Proto_UIEnding_Thank", Engine::CTexture::Create(m_pGraphicDev, L"../Bin/Resource/Texture/MMJ_Interface/Font/Word/Text/Text_Thankyou.png", TEXTUREID::TEX_NORMAL)), E_FAIL);

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
	default:
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