#include "pch.h"
#include "..\Header\Player.h"
#include "Export_System.h"
#include "Export_Utility.h"
// 규빈
#include "../Header/EffectPool.h"
#include "../Header/EffectMuzzleFlash.h"
#include "../Header/Floor.h"
#include "../Header/Monster.h"
#include "../Header/DrinkMachine.h"
#include "../Header/Item.h"
#include "../Header/Boss_Humanoid.h"
#include"../Header/Wall.h"
#include"../Header/WallTB.h"

#include "../Header/UIAction.h"
#include "../Header/UIDown.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vStartPos)
	: Engine::CCharacter(_pGraphicDev)
	, m_pRight_TransformCom(nullptr)
	, m_pBody_TransformCom(nullptr)
	, m_pCComponentCamera(nullptr)
	, m_pLeft_TransformCom(nullptr)
	, m_pLeg_TransformCom(nullptr)
	, m_pPlayer_Buffer(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_bJumpCheck(false)
	, m_bLeftHandUse(true)
	, m_bLegUse(false)
	, m_bIsHasItem(false)
	, m_bIsDrinking(false)
	, m_bIsRotation(false)
	, m_bIsTrapOn(false)
	, m_bIsSlideOn(false)
	, m_bIsLeft(false)
	, m_bIsRight(false)
	, m_bIsShaking(false)
	, m_bIsClear(false)
	, m_bIsBoss(false)
	, m_bIsShop(false)
	, m_fDamage(5.f)
	, m_fSmileTimer(0.f)
	, m_fShakingTimer(0.f)
	, m_fShakingSize(0.f)
	, m_fTrapTime(0.f)
	, m_fHP(0.f)
	, m_fTimerHP(0.f)
	, m_fJumpPower(0.f)
	, m_fTilePos(0.f)
	, m_fSpeed(0.f)
	, m_fMaxAttackDelay(0.f)
	, m_fCurAttackDelay(1.5f)
	, m_fDashSpeed(0.f)
	, m_fTime_Skill(1.f) //For TimeSkill
	, m_fSkillCool(30.f)
	, m_fSkillTime(0.f)
	, m_iCurAmmo(6) //because Pistol
	, m_iMaxAmmo(6) //because Pistol
	, m_Right_CurState(START)
	, m_Right_PreState(RIGHT_STATE_END)
	, m_Left_CurState(LEFT_IDLE)
	, m_Left_PreState(LEFT_STATE_END)
	, m_Leg_CurState(LEG_IDLE)
	, m_Leg_PreState(LEG_STATE_END)
	, m_WeaponState(PISTOL)
	, m_eItemType(Engine::ITEM_TYPE::ITEM_END)
	, m_vStartPos(_vStartPos)
	//Beomseung
{
	ZeroMemory(&m_fFrameStart, sizeof(m_fFrameStart));
	ZeroMemory(&m_fFrameEnd, sizeof(m_fFrameEnd));
	ZeroMemory(&m_fFrameSpeed, sizeof(m_fFrameSpeed));
	ZeroMemory(&m_pAnimator, sizeof(m_pAnimator));
	ZeroMemory(&m_flinear, sizeof(m_flinear));
	m_vDefaultPos[RIGHT] = { WINCX / 3.f,WINCY / -3.f,2.f };
	m_vDefaultPos[LEFT] = { WINCX / -3.f,WINCY / -1.8f,2.f };
	m_vDefaultSize[RIGHT] = { 500.f,500.f,0.f };
	m_vDefaultSize[LEFT] = { 500.f,500.f,0.f };
	m_vDefaultSize[LEG] = { 500.f,500.f,0.f };

	for (_int i = 0; i < LEG_STATE::LEG_STATE_END; ++i)
		m_pLeg_TextureCom[i] = nullptr;
	for (_int i = 0; i < LEFT_STATE::LEFT_STATE_END; ++i)
		m_pLeft_TextureCom[i] = nullptr;
	for (_int i = 0; i < WEAPON_STATE::WEAPON_STATE_END; ++i)
		for (_int j = 0; j < RIGHT_STATE::RIGHT_STATE_END; ++j)
			m_pRight_TextureCom[i][j] = nullptr;
}

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vStartPos, _bool _bBossStage)
	: Engine::CCharacter(_pGraphicDev)
	, m_pRight_TransformCom(nullptr)
	, m_pBody_TransformCom(nullptr)
	, m_pCComponentCamera(nullptr)
	, m_pLeft_TransformCom(nullptr)
	, m_pLeg_TransformCom(nullptr)
	, m_pPlayer_Buffer(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_bJumpCheck(false)
	, m_bLeftHandUse(true)
	, m_bLegUse(false)
	, m_bIsHasItem(false)
	, m_bIsDrinking(false)
	, m_bIsRotation(false)
	, m_bIsTrapOn(false)
	, m_bIsSlideOn(false)
	, m_bIsLeft(false)
	, m_bIsRight(false)
	, m_bIsShaking(false)
	, m_bIsClear(false)
	, m_bIsBoss(_bBossStage)
	, m_bIsShop(false)
	, m_fDamage(5.f)
	, m_fShakingTimer(0.f)
	, m_fShakingSize(0.f)
	, m_fTrapTime(0.f)
	, m_fHP(0.f)
	, m_fTimerHP(0.f)
	, m_fSmileTimer(0.f)
	, m_fJumpPower(0.f)
	, m_fTilePos(0.f)
	, m_fSpeed(0.f)
	, m_fMaxAttackDelay(0.f)
	, m_fCurAttackDelay(1.5f)
	, m_fDashSpeed(0.f)
	, m_fTime_Skill(1.f) //For TimeSkill
	, m_fSkillCool(30.f)
	, m_fSkillTime(0.f)
	, m_iCurAmmo(6) //because Pistol
	, m_iMaxAmmo(6) //because Pistol
	, m_Right_CurState(START)
	, m_Right_PreState(RIGHT_STATE_END)
	, m_Left_CurState(LEFT_IDLE)
	, m_Left_PreState(LEFT_STATE_END)
	, m_Leg_CurState(LEG_IDLE)
	, m_Leg_PreState(LEG_STATE_END)
	, m_WeaponState(PISTOL)
	, m_eItemType(Engine::ITEM_TYPE::ITEM_END)
	, m_vStartPos(_vStartPos)
	//Beomseung
{
	ZeroMemory(&m_fFrameStart, sizeof(m_fFrameStart));
	ZeroMemory(&m_fFrameEnd, sizeof(m_fFrameEnd));
	ZeroMemory(&m_fFrameSpeed, sizeof(m_fFrameSpeed));
	ZeroMemory(&m_pAnimator, sizeof(m_pAnimator));
	ZeroMemory(&m_flinear, sizeof(m_flinear));
	m_vDefaultPos[RIGHT] = { WINCX / 3.f,WINCY / -3.f,2.f };
	m_vDefaultPos[LEFT] = { WINCX / -3.f,WINCY / -1.8f,2.f };
	m_vDefaultSize[RIGHT] = { 500.f,500.f,0.f };
	m_vDefaultSize[LEFT] = { 500.f,500.f,0.f };
	m_vDefaultSize[LEG] = { 500.f,500.f,0.f };

	for (_int i = 0; i < LEG_STATE::LEG_STATE_END; ++i)
		m_pLeg_TextureCom[i] = nullptr;
	for (_int i = 0; i < LEFT_STATE::LEFT_STATE_END; ++i)
		m_pLeft_TextureCom[i] = nullptr;
	for (_int i = 0; i < WEAPON_STATE::WEAPON_STATE_END; ++i)
		for (_int j = 0; j < RIGHT_STATE::RIGHT_STATE_END; ++j)
			m_pRight_TextureCom[i][j] = nullptr;
}

CPlayer::~CPlayer()
{
}

void CPlayer::Rooting_Item(Engine::ITEM_TYPE _eItemType)
{
	m_eItemType = _eItemType;
	if (ITEM_TYPE::ITEM_DRINK != _eItemType)
	{
		m_bIsHasItem = true;
	}
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vStartPos)
{
	CPlayer* pPlayer = new CPlayer(_pGraphicDev, _vStartPos);

	if (FAILED(pPlayer->Ready_GameObject()))
	{
		Safe_Release(pPlayer);
		MSG_BOX("pPlayer Create Failed");
		return nullptr;
	}

	return pPlayer;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 _pGraphicDev, _vec3 _vStartPos, _bool _bIsBossStage)
{
	CPlayer* pPlayer = new CPlayer(_pGraphicDev, _vStartPos, _bIsBossStage);

	if (FAILED(pPlayer->Ready_GameObject()))
	{
		Safe_Release(pPlayer);
		MSG_BOX("pPlayer Create Failed");
		return nullptr;
	}

	return pPlayer;
}

HRESULT CPlayer::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	SetAnimation();

	m_pBody_TransformCom->Set_Pos(m_vStartPos);

	_vec3 vDir = { 0.5f, 0.5f, 0.5f };

	m_pColliderCom->SetTransform(m_pBody_TransformCom);
	m_pColliderCom->SetRadius(1.f);
	m_pColliderCom->SetLookDir(vDir);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);

	m_fHP = 99.f;
	m_fSpeed = 10.f;
	m_fTimerHP = 20.f;
	// 규빈 : 알파소팅을 위한 설정, 
	m_fViewZ = 10.f;
	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& _fTimeDelta)
{
	// 연욱 - 테스트용 플레이어 체력 깎기
	//if (Engine::Key_Hold(DIK_DOWN))
	

	Calculate_TimerHP(_fTimeDelta );


	Picking_Terrain();
	Damage_Terrain();//유빈 - 산성,용암지형에 플레이어가 대미지를 받음
	Speed_Terrain();
	if (m_bIsTrapOn)
	{
		if (m_fTrapTime > 2.5f)	// 5초 정도로 설정// 왜 2.5로 되는지..모르겠음
		{
			m_bIsTrapOn = false;
			m_fTrapTime = 0;
		}
		else
		{
			m_fTrapTime += _fTimeDelta * m_fTime_Skill;
		}

	}
	if (m_bIsSlideOn)
	{
		_vec3 vLook;
		_vec3 vRight;
		_vec3 vUp;
		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		m_pBody_TransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);
		m_pBody_TransformCom->Get_Info(INFO::INFO_UP, &vUp);
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, m_fSpeed);
	}
	if (Engine::Get_ControllerID() == CONTROLLERID::CONTROL_PLAYER)
	{
		if (Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
		{
			//Key_Input(_fTimeDelta * m_fTime_Skill);
			Key_Input(_fTimeDelta);
			Mouse_Move(_fTimeDelta * m_fTime_Skill);
		}

		Animation_End_Check();
	}

	if (Engine::Get_ListUI(UITYPE::UI_INVENTORY)->empty())
		if (Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
			Mouse_Fix();

	Jump(_fTimeDelta);

	Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);
	Engine::Add_Collider(m_pColliderCom);

	if (m_fTime_Skill == 3.0f)
	{
		m_fSkillTime += _fTimeDelta;
		m_fSkillCool = 0.f;
		if (m_fSkillTime >= 3.f) Skill_Timer();
	}
	else m_fSkillCool += _fTimeDelta;

	_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	Moving_Rotate();
	Shaking_Camera(_fTimeDelta);
	Animation_Pos();
	m_pLeg_TransformCom->Update_Component(_fTimeDelta);
	m_pRight_TransformCom->Update_Component(_fTimeDelta);
	m_pLeft_TransformCom->Update_Component(_fTimeDelta);
	return iExit;

}

void CPlayer::LateUpdate_GameObject()
{

	Engine::CGameObject::LateUpdate_GameObject();
}

void CPlayer::Render_GameObject()
{
	if (!Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
	{
		m_bIsShop = true;
		return;
	}
	else if (m_bIsShop)
	{
		Engine::Stop_All();
		m_bIsShop = false;
		m_pAnimator[RIGHT]->PlayAnimation(L"Player_Start", false);
		Engine::Play_Sound(L"Fist_Combo.wav", CHANNELID::SOUND_PLAYER, 0.6f);
		m_Right_CurState = START;
		m_bLeftHandUse = false;

		m_pAnimator[LEFT]->PlayAnimation(L"Left_Idle", true);
		m_pLeft_TransformCom->Set_Scale(m_vDefaultSize[LEFT]);
		m_pLeft_TransformCom->Set_Pos(m_vDefaultPos[LEFT]);

		m_pAnimator[LEG]->PlayAnimation(L"Leg_Idle", false);
		m_pLeg_TransformCom->Set_Scale(m_vDefaultSize[LEG]);
		m_pLeg_TransformCom->Set_Pos(0, WINCY / -3.f, 2.f);
		return;
	}

	//Beomseung Fix
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	_matrix mat = *m_pRight_TransformCom->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);

	m_pAnimator[RIGHT]->Render_Animator();
	m_pPlayer_Buffer->Render_Buffer();

	if (m_bLeftHandUse) {
		mat = *m_pLeft_TransformCom->Get_WorldMatrix();
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
		m_pAnimator[LEFT]->Render_Animator();
		m_pPlayer_Buffer->Render_Buffer();
	}
	if (m_bLegUse) {
		_matrix mat = *m_pLeg_TransformCom->Get_WorldMatrix();
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
		m_pAnimator[LEG]->Render_Animator();
		m_pPlayer_Buffer->Render_Buffer();
	}
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component()
{
	CComponent* pComponent = NULL;

	//Beomseung
	//Buffer

	pComponent = m_pPlayer_Buffer = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_PlayerBuffer"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_PlayerBuffer", pComponent });

	//Texture

	//Right
	//Start
	pComponent = m_pRight_TextureCom[PISTOL][START] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Player_Start")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Player_Start", pComponent });
	//Pistol
	pComponent = m_pRight_TextureCom[PISTOL][IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Pistol_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Pistol_IDLE", pComponent });

	pComponent = m_pRight_TextureCom[PISTOL][SHOOT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Pistol_Shoot")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Pistol_Shoot", pComponent });

	pComponent = m_pRight_TextureCom[PISTOL][RELOAD] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Pistol_Reload")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Pistol_Reload", pComponent });

	pComponent = m_pRight_TextureCom[PISTOL][CHANGE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Pistol_Change")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Pistol_Change", pComponent });

	pComponent = m_pRight_TextureCom[PISTOL][EXECUTION] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Right_KnifeExecution")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Right_KnifeExecution", pComponent });

	//Rifle
	pComponent = m_pRight_TextureCom[RIFLE][IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Rifle_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Rifle_IDLE", pComponent });

	pComponent = m_pRight_TextureCom[RIFLE][SHOOT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Rifle_Shoot")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Rifle_Shoot", pComponent });

	pComponent = m_pRight_TextureCom[RIFLE][RELOAD] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Rifle_Reload")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Rifle_Reload", pComponent });

	pComponent = m_pRight_TextureCom[RIFLE][CHANGE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Rifle_Change")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Rifle_Change", pComponent });

	//Shotgun
	pComponent = m_pRight_TextureCom[SHOTGUN][IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Shotgun_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Shotgun_IDLE", pComponent });

	pComponent = m_pRight_TextureCom[SHOTGUN][SHOOT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Shotgun_Shoot")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Shotgun_Shoot", pComponent });

	pComponent = m_pRight_TextureCom[SHOTGUN][RELOAD] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Shotgun_Reload")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Shotgun_Reload", pComponent });

	pComponent = m_pRight_TextureCom[SHOTGUN][CHANGE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Shotgun_Change")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Shotgun_Change", pComponent });

	//Sniper
	pComponent = m_pRight_TextureCom[SNIPER][IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_IDLE", pComponent });

	pComponent = m_pRight_TextureCom[SNIPER][SHOOT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_Shoot")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_Shoot", pComponent });

	pComponent = m_pRight_TextureCom[SNIPER][RELOAD] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_Reload")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_Reload", pComponent });

	pComponent = m_pRight_TextureCom[SNIPER][ZOOMIN] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_ZoomIn")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_ZoomIn", pComponent });

	pComponent = m_pRight_TextureCom[SNIPER][ZOOMING] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_ZoomIng")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_ZoomIng", pComponent });

	pComponent = m_pRight_TextureCom[SNIPER][ZOOMOUT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_ZoomOut")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_ZoomOut", pComponent });

	//Katana
	pComponent = m_pRight_TextureCom[KATANA][IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Katana_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Katana_IDLE", pComponent });

	pComponent = m_pRight_TextureCom[KATANA][SHOOT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Katana_Shoot")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Katana_Shoot", pComponent });

	pComponent = m_pRight_TextureCom[KATANA][CHANGE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Katana_Change")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Katana_Change", pComponent });

	pComponent = m_pRight_TextureCom[MINIGUN][IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MiniGun_GunPoint_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_MiniGun_GunPoint_IDLE", pComponent });

	pComponent = m_pRight_TextureCom[MINIGUN][SHOOT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MiniGun_GunPoint_Shoot")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_MiniGun_GunPoint_Shoot", pComponent });

	pComponent = m_pRight_TextureCom[MINIGUN][CHANGE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MiniGun_GunPoint_Change")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_MiniGun_GunPoint_Change", pComponent });

	//Left
	pComponent = m_pLeft_TextureCom[LEFT_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LeftIdle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture2", pComponent });

	pComponent = m_pLeft_TextureCom[LEFT_CHANGE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LeftChange"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_LeftChange", pComponent });

	pComponent = m_pLeft_TextureCom[DRINK] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LeftDrink")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_LeftDrink", pComponent });

	pComponent = m_pLeft_TextureCom[LEFT_EXECUTION] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LeftExecution")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_LeftExecution", pComponent });

	pComponent = m_pLeft_TextureCom[MINIGUN_BODY_IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MiniGun_Body_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_MiniGun_Body_IDLE", pComponent });

	pComponent = m_pLeft_TextureCom[MINIGUN_BODY_CHANGE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MiniGun_Body_Change")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_MiniGun_Body_Change", pComponent });

	//Leg
	pComponent = m_pLeg_TextureCom[LEG_IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Leg_Idle")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Leg_Idle", pComponent });

	pComponent = m_pLeg_TextureCom[KICK] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Leg_Kick")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Leg_Kick", pComponent });

	pComponent = m_pLeg_TextureCom[SLIDING] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Leg_Sliding")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Leg_Sliding", pComponent });

	pComponent = m_pLeg_TextureCom[MINIGUN_PANEL_CHANGE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MiniGun_Panel_Change")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_MiniGun_Panel_Change", pComponent });

	pComponent = m_pLeg_TextureCom[MINIGUN_PANEL_IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_MiniGun_Panel_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_MiniGun_Panel_IDLE", pComponent });

	//Transform
	pComponent = m_pRight_TransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Right_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Right_Transform", pComponent });

	pComponent = m_pLeft_TransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Left_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Left_Transform", pComponent });

	pComponent = m_pLeg_TransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Leg_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Leg_Transform", pComponent });

	pComponent = m_pBody_TransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Body_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Body_Transform", pComponent });


	//Calculator
	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Calculator", pComponent });

	//Animator
	pComponent = m_pAnimator[RIGHT] = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Player_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Right_Animator", pComponent });

	pComponent = m_pAnimator[LEFT] = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Player_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Left_Animator", pComponent });

	pComponent = m_pAnimator[LEG] = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Player_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Leg_Animator", pComponent });

	//Camera
	pComponent = m_pCComponentCamera = dynamic_cast<CComponentCamera*>(Engine::Clone_Proto(L"Proto_ComponentCamera"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ComponentCamera", pComponent });
	pComponent->SetOwner(*this);

	pComponent = m_pColliderCom = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Proto_Collider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Collider", pComponent });
	pComponent->SetOwner(*this);

	return S_OK;
}

void CPlayer::Key_Input(const _float& _fTimeDelta)
{
	if (0.f < m_fCurAttackDelay)
		m_fCurAttackDelay -= _fTimeDelta;

	_vec3 vLook;
	_vec3 vRight;
	_vec3 vUp;
	m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	m_pBody_TransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);
	m_pBody_TransformCom->Get_Info(INFO::INFO_UP, &vUp);


	if (Engine::Key_Hold(DIK_W)) {
		//Beomseung
		if (!m_bLegUse)
			m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, m_fSpeed);
	}
	if (Engine::Key_Hold(DIK_S)) {
		//Beomseung   
		if (!m_bLegUse)
			m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, -m_fSpeed);

	}
	if (Engine::Key_Hold(DIK_A)) {
		//Beomseung    
		if (!m_bLegUse)
		{
			m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), _fTimeDelta, -m_fSpeed);
			m_bIsLeft = true;
		}
	}
	if (Engine::Key_Release(DIK_A)) {
		//Beomseung    
		if (!m_bLegUse)
			m_bIsLeft = false;
	}
	if (Engine::Key_Hold(DIK_D)) {
		//Beomseung    
		if (!m_bLegUse)
		{
			m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), _fTimeDelta, m_fSpeed);
			m_bIsRight = true;
		}
	}
	if (Engine::Key_Release(DIK_D)) {
		//Beomseung    
		if (!m_bLegUse)
			m_bIsRight = false;
	}
	if (Engine::Key_Press(DIK_SPACE)) {
		if (!m_bJumpCheck)
		{
			m_bJumpCheck = true;
			m_fJumpPower = 7.5f;
		}
		
	}

	if (Engine::Key_Press(DIK_X)) {
		//m_bLeftHandUse = false;
		//m_Right_CurState = EXECUTION;
		//m_pAnimator[RIGHT]->PlayAnimation(L"FinishKill", false);

		if (m_fSkillCool >= 30.f)
			Skill_Timer();
	}

	if (Engine::Key_Hold(DIK_R)) {
		if (RELOAD != m_Right_CurState && m_iCurAmmo < m_iMaxAmmo)
		{
			switch (m_WeaponState) {
			case PISTOL:
				m_Right_CurState = RELOAD;
				m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Reload", false);
				Engine::Play_Sound(L"PistolReload.wav", CHANNELID::SOUND_EFFECT, 0.8f);
				break;
			case RIFLE:
				m_Right_CurState = RELOAD;
				m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Reload", false);
				Engine::Play_Sound(L"RifleReload.wav", CHANNELID::SOUND_EFFECT, 0.8f);
				m_bLeftHandUse = false;
				break;
			case SHOTGUN:
				m_Right_CurState = RELOAD;
				m_pAnimator[RIGHT]->PlayAnimation(L"Shotgun_Reload", false);
				Engine::Play_Sound(L"ShotgunReload.wav", CHANNELID::SOUND_EFFECT, 0.8f);
				break;
			case SNIPER:
				m_Right_CurState = RELOAD;
				m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_Reload", false);
				Engine::Play_Sound(L"SniperReload.wav", CHANNELID::SOUND_EFFECT, 0.8f);
				m_bLeftHandUse = false;
				break;
			default:
				break;
			}
			m_iCurAmmo = m_iMaxAmmo;
			//Engine::Play_Sound(L"Reload.wav", CHANNELID::SOUND_EFFECT, 0.8f);
		}
		m_iCurAmmo = m_iMaxAmmo;
	}

	if (Engine::Key_Press(DIK_1))
	{
		if (PISTOL != m_WeaponState)
		{
			m_bLeftHandUse = true;
			m_bLegUse = false;
			m_WeaponState = PISTOL;
			m_Right_CurState = CHANGE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Change", false);
			m_iCurAmmo = 6;
			m_iMaxAmmo = 6;
			m_fMaxAttackDelay = 0.5f;
			m_fDamage = 5.f;
		}
	}

	if (Engine::Key_Press(DIK_2))
	{
		if (RIFLE != m_WeaponState)
		{
			m_WeaponState = RIFLE;
			m_bLegUse = false;
			m_Right_CurState = CHANGE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Change", false);
			m_bLeftHandUse = false;
			m_iCurAmmo = 30;
			m_iMaxAmmo = 30;
			m_fMaxAttackDelay = 1.0f;
			m_fDamage = 2.f;
			Engine::Play_Sound(L"Change_Rifle.wav", CHANNELID::SOUND_PLAYER, 0.7f);
		}
	}

	if (Engine::Key_Press(DIK_3))
	{
		if (SHOTGUN != m_WeaponState)
		{
			m_bLeftHandUse = false;
			m_bLegUse = false;
			m_WeaponState = SHOTGUN;
			m_Right_CurState = CHANGE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Shotgun_Change", false);
			m_iCurAmmo = 2;
			m_iMaxAmmo = 2;
			m_fMaxAttackDelay = 1.5f;
			m_fDamage = 8.f;
			Engine::Play_Sound(L"Change_ShotGun.wav", CHANNELID::SOUND_PLAYER, 0.7f);
		}
	}

	if (Engine::Key_Press(DIK_4))
	{
		if (SNIPER != m_WeaponState && m_bIsBoss)
		{
			m_bLeftHandUse = true;
			m_bLegUse = false;
			m_WeaponState = SNIPER;
			m_Right_CurState = IDLE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_Idle", true);
			m_iCurAmmo = 10;
			m_iMaxAmmo = 10;
			m_fMaxAttackDelay = 1.5f;
			m_fDamage = 10.f;
		}
	}

	if (Engine::Key_Press(DIK_5))
	{
		if (KATANA != m_WeaponState)
		{
			m_bLeftHandUse = false;
			m_bLegUse = false;
			m_WeaponState = KATANA;
			m_Right_CurState = CHANGE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Katana_Change", false);
			m_iCurAmmo = 100;//Dummy Code
			m_iMaxAmmo = 100;//Dummy Code
			m_fMaxAttackDelay = 1.5f;
			m_fDamage = 15.f;
			Engine::Play_Sound(L"katana_Intro.wav", CHANNELID::SOUND_PLAYER, 1.0f);
		}
	}

	if (Engine::Key_Press(DIK_6))
	{
		if (MINIGUN != m_WeaponState && m_bIsBoss)
		{
			m_bLegUse = true;
			m_bLeftHandUse = true;
			m_WeaponState = MINIGUN;
			m_Right_CurState = CHANGE;
			m_Left_CurState = MINIGUN_BODY_CHANGE;
			m_Leg_CurState = MINIGUN_PANEL_CHANGE;
			m_pAnimator[RIGHT]->PlayAnimation(L"MiniGun_GunPoint_Idle", false);
			m_pAnimator[LEFT]->PlayAnimation(L"MiniGun_Body_Change", false);
			m_pAnimator[LEG]->PlayAnimation(L"MiniGun_Panel_Change", false);
			m_iCurAmmo = 100;
			m_iMaxAmmo = 100;
			m_fMaxAttackDelay = 1.0f;
			m_fDamage = 2.f;
			Engine::Play_Sound(L"uh-oh-big.wav", CHANNELID::SOUND_PLAYER, 0.7f);
		}
	}


	

	if (Engine::Key_Hold(DIK_O)) {
		//Beomseung   
		m_bLegUse = true;
		m_Leg_CurState = SLIDING;
		m_pAnimator[LEG]->PlayAnimation(L"Leg_Sliding", false);
	}


	// Kyubin
	//if (Engine::Key_Press(DIK_X))
	//{
	//	CComponent* pComponent(nullptr);

		//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster_Right", L"Com_Effect");
		//if (pComponent)
		//	static_cast<CEffect*>(pComponent)->Operate_Effect();

		//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster_Left", L"Com_Effect");
		//if (pComponent)
		//	static_cast<CEffect*>(pComponent)->Operate_Effect();

		//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectLaserTarget", L"Com_Effect");
		//if (pComponent)
		//{
		//	static_cast<CEffect*>(pComponent)->Set_CallerObject(this);
		//	static_cast<CEffect*>(pComponent)->Operate_Effect();
		//}

	//}

	//if (Engine::Key_Release(DIK_X))
	//{
	//	CComponent* pComponent(nullptr);

		//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster_Left", L"Com_Effect");
		//if (pComponent)
		//	static_cast<CEffect*>(pComponent)->Stop_Effect();

		//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster_Right", L"Com_Effect");
		//if (pComponent)
		//	static_cast<CEffect*>(pComponent)->Stop_Effect();

	//}
	if (Engine::Key_Press(DIK_Z))
	{
		CComponent* pComponent(nullptr);
		CGameObject* pGameObject(nullptr);

		_vec3 vPos, vLook;
		m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &vPos);
		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);

		//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectFanSpread", L"Com_Effect");
		//static_cast<CEffect*>(pComponent)->Operate_Effect();

	}
	if (Engine::Key_Press(DIK_M))
	{
		CComponent* pComponent(nullptr);
		CGameObject* pGameObject(nullptr);

		_vec3 vPos, vLook;
		m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &vPos);
		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);

		// 폭발
		//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_Explosion", L"Com_Transform");
		//static_cast<CTransform*>(pComponent)->Set_Pos(vPos + vLook * 1.f);
		//pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
		//static_cast<CEffectPool*>(pGameObject)->Set_CallerObject(this);
		//static_cast<CEffectPool*>(pGameObject)->Operate();

		//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBigExplosion", L"Com_Transform");
		//static_cast<CTransform*>(pComponent)->Set_Pos(vPos + vLook * 8.f);
		//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBigExplosion", L"Com_Effect");
		//static_cast<CEffect*>(pComponent)->Operate_Effect();

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotDie", L"Com_Effect");
		if (pComponent)
			static_cast<CEffect*>(pComponent)->Operate_Effect();

	}
}

void CPlayer::Mouse_Move(const _float& _fTimeDelta)
{
	_long dwMouseMove(0);

	if (Engine::Get_Stop())
		return;

	if (dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_Y)) // 위로 넘어가거나 밑으로 넘어가는 각도 제한
	{
		_float fAngleX = m_pBody_TransformCom->Get_Angle()->x;
		if (D3DXToRadian(80.f) > fAngleX && D3DXToRadian(-80.f) < fAngleX)
		{
			if (EXECUTION != m_Right_CurState)
				m_pBody_TransformCom->Rotation(ROTATION::ROT_X, D3DXToRadian(dwMouseMove / 20.f));
		}
		else if (D3DXToRadian(80.f) <= fAngleX)
		{
			m_pBody_TransformCom->Rotation(ROTATION::ROT_X, D3DXToRadian(-0.45f));
		}
		else if (D3DXToRadian(-80.f) >= fAngleX)
		{
			m_pBody_TransformCom->Rotation(ROTATION::ROT_X, D3DXToRadian(0.45f));
		}
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X))
	{
		if (EXECUTION != m_Right_CurState)
			m_pBody_TransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(dwMouseMove / 20.f));
	}
	if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB)) {

		if (1 > m_iCurAmmo)
		{
			Engine::Play_Sound(L"NoBullet.wav", CHANNELID::SOUND_EFFECT, 0.5f); //재장전 소리가 필요함
			return;
		}

		if (0.5f < m_fCurAttackDelay)
		{
			//You can't shot!
			return;
		}

		if (0.f > m_fSmileTimer)
		{
			_int iIndex = rand() % 13;

			if (0 == iIndex % 4)
				Engine::Play_Sound(L"Player_Laughs_01.wav", CHANNELID::SOUND_PLAYER, 0.8f);
			else if (1 == iIndex % 4)
				Engine::Play_Sound(L"Player_Laughs_02.wav", CHANNELID::SOUND_PLAYER, 0.8f);
			else if (2 == iIndex % 4)
				Engine::Play_Sound(L"Player_Laughs_03.wav", CHANNELID::SOUND_PLAYER, 0.8f);
			else
				Engine::Play_Sound(L"Player_Laughs_04.wav", CHANNELID::SOUND_PLAYER, 0.8f);
			m_fSmileTimer = 7.f;
		}

		_vec3 RayStart, RayDir, vPos;
		m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &RayStart);
		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &RayDir);
		// 규빈
		_vec3 vMuzzlePos{};
		CComponent* pComponent(nullptr);
		m_pRight_TransformCom->Get_Info(INFO::INFO_POS, &vMuzzlePos);

		switch (m_WeaponState) {
		case PISTOL:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Shoot", false);
			Engine::Play_Sound(L"Shoot_Single.wav", CHANNELID::SOUND_PLAYER_GUN, 0.9f);

			// 규빈
			vMuzzlePos.x -= 100.f;
			vMuzzlePos.y += 200.f;

			break;
		case RIFLE:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Shoot", false);
			Engine::Play_Sound(L"Player_Rifle.wav", CHANNELID::SOUND_PLAYER_GUN, 0.9f);
			// 규빈
			vMuzzlePos.x += -120.f;
			vMuzzlePos.y += 100.f;
			break;
		case SHOTGUN:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Shotgun_Shoot", false);
			Engine::Play_Sound(L"Player_ShotGun.wav", CHANNELID::SOUND_PLAYER_GUN, 0.9f);
			m_bIsShaking = true;
			m_fShakingSize = 20.f;
			m_fShakingTimer = 0.5f;
			// 규빈
			vMuzzlePos.x += -180.f;
			vMuzzlePos.y += 210.f;
			break;
		case SNIPER:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_Shoot", false);
			Engine::Play_Sound(L"Player_Sniper.wav", CHANNELID::SOUND_PLAYER_GUN, 0.8f);
			m_bIsShaking = true;
			m_fShakingSize = 40.f;
			m_fShakingTimer = 0.5f;
			// 규빈
			vMuzzlePos.x += -160.f;
			vMuzzlePos.y += 200.f;
			pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectSniperMuzzleFlash", L"Com_Effect");
			static_cast<CEffect*>(pComponent)->Operate_Effect();

			break;
		case KATANA:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Katana_Shoot", false);
			Engine::Play_Sound(L"Katana_Combo.wav", CHANNELID::SOUND_PLAYER_GUN, 1.0f);
			break;
		case MINIGUN:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"MiniGun_GunPoint_Shoot", true);
			//Engine::Fire_Bullet(m_pGraphicDev, RayStart, RayDir, 5, CBulletManager::BULLET_PISTOL);
			break;
		}
		m_fCurAttackDelay = m_fMaxAttackDelay;
		m_iCurAmmo--;

		// 규빈
		if (m_WeaponState != WEAPON_STATE::KATANA &&
			m_WeaponState != WEAPON_STATE::MINIGUN &&
			m_WeaponState != WEAPON_STATE::SNIPER)
		{
			CEffectMuzzleFlash* pMuzzleFlash(nullptr);
			pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectMuzzleFlash", L"Com_Effect");
			pMuzzleFlash = static_cast<CEffectMuzzleFlash*>(static_cast<CTransform*>(pComponent)->GetOwner());
			pMuzzleFlash->Set_InitPos(vMuzzlePos);
			static_cast<CEffect*>(pComponent)->Operate_Effect();
		}

		if (m_WeaponState != KATANA && m_WeaponState != MINIGUN) {
			//Engine::RayCast2(RayStart, RayDir);
			if (Engine::FireRayCast(RayStart + _vec3(0.f, 0.5f, 0.f), RayDir, vPos, m_fDamage))
			{
				CGameObject* pGameObject(nullptr);

				pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_BloodJet", L"Com_Transform");
				//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_BloodSplater", L"Com_Transform");
				static_cast<CTransform*>(pComponent)->Set_Pos(vPos);
				static_cast<CTransform*>(pComponent)->Set_Scale(0.3f, 0.3f, 1.f);
				pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
				static_cast<CEffectPool*>(pGameObject)->Operate();
			}
			else // 레이캐스팅 -> 몬스터가 검출되지 않은 경우에
			{
				// 규빈 - 벽 스파크
				CGameObject* pGameObject(nullptr);
				CComponent* pComponent(nullptr);
				CWall* pWall(nullptr);
				CWallTB* pWallTB(nullptr);
				CFloor* pFloor(nullptr);

				_float fDist = 0.f;
				_vec3 vHitPosition;
				D3DXVec3Normalize(&RayDir, &RayDir);
				pGameObject = Engine::CCollisionManager::GetInstance()->RayCastWall(RayStart + _vec3(0.f, 0.5f, 0.f), RayDir, &vHitPosition);
				//pGameObject->OnCollisionEnter(*m_pColliderCom);
				pWall = dynamic_cast<CWall*>(pGameObject);
				pWallTB = dynamic_cast<CWallTB*>(pGameObject);
				pFloor = dynamic_cast<CFloor*>(pGameObject);

				_vec3 vNormal{ 0.f, 0.f, 0.f };
				if (pWall)
					vNormal = pWall->Get_TileDirection();
				else if (pWallTB)
					vNormal = pWallTB->Get_TileDirection();
				else if (pFloor)
					vNormal = { 0.f, 1.f, 0.f };

				pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_Spark", L"Com_Transform");
				static_cast<CTransform*>(pComponent)->Set_Pos(vHitPosition);
				static_cast<CTransform*>(pComponent)->Set_Angle(-D3DX_PI * 0.5f * vNormal.z, 0.f, D3DX_PI * 0.5f * vNormal.x);
				pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
				static_cast<CEffectPool*>(pGameObject)->Operate();

			}
		}
	}

	if (Engine::Mouse_Hold(MOUSEKEYSTATE::DIM_LB))
	{
		CComponent* pComponent(nullptr);
		CEffectMuzzleFlash* pMuzzleFlash(nullptr);
		_vec3 RayStart, RayDir, vPos;
		_vec3 vMuzzlePos{};

		switch (m_WeaponState)
		{
		case CPlayer::RIFLE:


			if (0.f < m_fCurAttackDelay)
				m_fCurAttackDelay -= _fTimeDelta;
			if (0.65f > m_fCurAttackDelay)
			{
				m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &RayStart);
				m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &RayDir);
				// 규빈
				m_pRight_TransformCom->Get_Info(INFO::INFO_POS, &vMuzzlePos);
				m_fCurAttackDelay = m_fMaxAttackDelay;
				m_iCurAmmo--;
				if (1 > m_iCurAmmo)
				{
					m_Right_CurState = IDLE;
					break;
				}
				m_Right_CurState = SHOOT;
				m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Shoot", false);
				Engine::Play_Sound(L"Player_Rifle.wav", CHANNELID::SOUND_PLAYER_GUN, 0.9f);
				// 규빈
				vMuzzlePos.x += -120.f;
				vMuzzlePos.y += 100.f;
				m_bIsShaking = true;
				m_fShakingSize = 15.f;
				m_fShakingTimer = 0.2f;
				pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectMuzzleFlash", L"Com_Effect");
				pMuzzleFlash = static_cast<CEffectMuzzleFlash*>(static_cast<CTransform*>(pComponent)->GetOwner());
				pMuzzleFlash->Set_InitPos(vMuzzlePos);
				static_cast<CEffect*>(pComponent)->Operate_Effect();
				if (Engine::FireRayCast(RayStart + _vec3(0.f, 0.5f, 0.f), RayDir, vPos, m_fDamage))
				{
					CGameObject* pGameObject(nullptr);

					pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_BloodJet", L"Com_Transform");
					//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_BloodSplater", L"Com_Transform");
					static_cast<CTransform*>(pComponent)->Set_Pos(vPos);
					static_cast<CTransform*>(pComponent)->Set_Scale(0.3f, 0.3f, 1.f);
					pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
					static_cast<CEffectPool*>(pGameObject)->Operate();
				}
				else // 레이캐스팅 -> 몬스터가 검출되지 않은 경우에
				{
					// 규빈 - 벽 스파크
					CGameObject* pGameObject(nullptr);
					CComponent* pComponent(nullptr);
					CWall* pWall(nullptr);
					CWallTB* pWallTB(nullptr);
					CFloor* pFloor(nullptr);

					_float fDist = 0.f;
					_vec3 vHitPosition;
					D3DXVec3Normalize(&RayDir, &RayDir);
					pGameObject = Engine::CCollisionManager::GetInstance()->RayCastWall(RayStart + _vec3(0.f, 0.5f, 0.f), RayDir, &vHitPosition);
					//pGameObject->OnCollisionEnter(*m_pColliderCom);
					pWall = dynamic_cast<CWall*>(pGameObject);
					pWallTB = dynamic_cast<CWallTB*>(pGameObject);
					pFloor = dynamic_cast<CFloor*>(pGameObject);

					_vec3 vNormal{ 0.f, 0.f, 0.f };
					if (pWall)
						vNormal = pWall->Get_TileDirection();
					else if (pWallTB)
						vNormal = pWallTB->Get_TileDirection();
					else if (pFloor)
						vNormal = { 0.f, 1.f, 0.f };

					pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_Spark", L"Com_Transform");
					static_cast<CTransform*>(pComponent)->Set_Pos(vHitPosition);
					static_cast<CTransform*>(pComponent)->Set_Angle(-D3DX_PI * 0.5f * vNormal.z, 0.f, D3DX_PI * 0.5f * vNormal.x);
					pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
					static_cast<CEffectPool*>(pGameObject)->Operate();

				}
			}

			break;
		case CPlayer::MINIGUN:
			if (1 > m_iCurAmmo)
			{
				m_Right_CurState = IDLE;
				m_pAnimator[RIGHT]->PlayAnimation(L"MiniGun_GunPoint_Change", false);
			}
			m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &RayStart);
			m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &RayDir);
			// 규빈
			m_pRight_TransformCom->Get_Info(INFO::INFO_POS, &vMuzzlePos);
			if (0.f < m_fCurAttackDelay)
				m_fCurAttackDelay -= _fTimeDelta;
			if (0.65f > m_fCurAttackDelay)
			{

				Engine::Fire_Bullet(m_pGraphicDev, RayStart, RayDir, m_fDamage, CBulletManager::BULLET_MINIGUN);
				Engine::Play_Sound(L"Shoot_MiniGun1.wav", CHANNELID::SOUND_PLAYER_GUN, 0.5f);
				m_fCurAttackDelay = m_fMaxAttackDelay;
				m_iCurAmmo--;
				m_bIsShaking = true;
				m_fShakingSize = 35.f;
				m_fShakingTimer = 0.3f;


				pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectMuzzleFlash", L"Com_Effect");
				pMuzzleFlash = static_cast<CEffectMuzzleFlash*>(static_cast<CTransform*>(pComponent)->GetOwner());
				vMuzzlePos.y += 100.f;
				pMuzzleFlash->Set_InitPos(vMuzzlePos);
				static_cast<CEffect*>(pComponent)->Operate_Effect();


				// 탄피
				pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_MinigunShell", L"Com_Transform");
				CGameObject* pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
				static_cast<CEffectPool*>(pGameObject)->Operate();

			}
			break;
		default:
			break;
		}
		if (0.f > m_fSmileTimer)
		{
			_int iIndex = rand() % 13;

			if (0 == iIndex % 4)
				Engine::Play_Sound(L"Player_Laughs_01.wav", CHANNELID::SOUND_PLAYER, 0.8f);
			else if (1 == iIndex % 4)
				Engine::Play_Sound(L"Player_Laughs_02.wav", CHANNELID::SOUND_PLAYER, 0.8f);
			else if (2 == iIndex % 4)
				Engine::Play_Sound(L"Player_Laughs_03.wav", CHANNELID::SOUND_PLAYER, 0.8f);
			else
				Engine::Play_Sound(L"Player_Laughs_04.wav", CHANNELID::SOUND_PLAYER, 0.8f);
			m_fSmileTimer = 7.f;
		}
	}
	if (Engine::Mouse_Release(MOUSEKEYSTATE::DIM_LB))
	{
		if (MINIGUN == m_WeaponState)
		{
			if (1 < m_iCurAmmo)
			{
				m_Right_CurState = IDLE;
				m_pAnimator[RIGHT]->PlayAnimation(L"MiniGun_GunPoint_Idle", false);
			}
		}
	}

	if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
	{
		if (SNIPER == m_WeaponState)
		{
			m_Right_CurState = ZOOMIN;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_ZoomIn", false);
			m_bLeftHandUse = false;
		}
		else
		{
			Engine::Play_Sound(L"Dash.wav", CHANNELID::SOUND_PLAYER_LEG, 0.7f);
			m_bLegUse = true;
			m_Leg_CurState = LEG_IDLE;
			m_pAnimator[LEG]->PlayAnimation(L"Leg_Idle", false);
			CComponent* pComponent(nullptr);
			pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectCircleLines", L"Com_Effect");
			static_cast<CEffect*>(pComponent)->Operate_Effect();
			Rotate_Arms(false);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, 0.f, 2.f);
			m_pLeft_TransformCom->Set_Pos(WINCX / -3.f, 0.f, 2.f);
			m_fDashSpeed = m_fSpeed * 2.5f;
		}
	}
	if (Engine::Mouse_Hold(MOUSEKEYSTATE::DIM_RB))
	{
		_vec3 vLook;
		if (0.f < m_fDashSpeed)
			m_fDashSpeed -= (_fTimeDelta * 33.f);
		else
			m_fDashSpeed = 0.f;

		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, m_fDashSpeed);
	}
	if (Engine::Mouse_Release(MOUSEKEYSTATE::DIM_RB))
	{
		if (SNIPER == m_WeaponState)
		{
			if (m_Right_CurState == ZOOMING || m_Right_CurState == ZOOMIN) {
				m_Right_CurState = ZOOMOUT;
				m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_ZoomOut", false);
				m_bLeftHandUse = false;
			}
		}
		else
		{
			CComponent* pComponent(nullptr);
			m_bLegUse = false;
			pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectCircleLines", L"Com_Effect");
			static_cast<CEffect*>(pComponent)->Stop_Effect();
			Rotate_Arms(true);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
			m_pLeft_TransformCom->Set_Pos(WINCX / -3.f, WINCY / -1.8f, 2.f);
		}
	}

}

void CPlayer::Mouse_Fix()
{
	POINT ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
	ShowCursor(FALSE);
}

void CPlayer::Jump(const _float& _fTimeDelta)
{
	if (m_bJumpCheck)
	{
		m_fJumpPower -= 0.75f;
		_vec3 vPos;
		_vec3 vUp = { 0.f, 1.f, 0.f };

		m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &vPos);

		if (vPos.y + _fTimeDelta * m_fJumpPower <= vPos.y - m_fTilePos + 1.f)
		{
			m_bJumpCheck = false;
			m_fJumpPower = 0;
			m_pBody_TransformCom->Set_Pos(vPos.x, vPos.y - m_fTilePos + 1.f, vPos.z);
		}
		else
			m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), _fTimeDelta, m_fJumpPower);
	}
}

void CPlayer::Picking_Terrain()
{
	_vec3 vPos;
	m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &vPos);

	m_fTilePos = Engine::FloorRayCast(vPos);

	if (!m_bJumpCheck && m_fTilePos > 0.f)
		m_pBody_TransformCom->Set_Pos(vPos.x, vPos.y - m_fTilePos + 1.f, vPos.z);
}

void CPlayer::Damage_Terrain()
{
	_vec3 vPos;
	CGameObject* pGameObject;
	m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &vPos);

	pGameObject = Engine::FloorRayCast2(vPos); // 레이픽킹된 바닥 정보를 받아옴

	if (pGameObject != nullptr && dynamic_cast<CFloor*>(pGameObject)->Get_Damage() > 0)// 바닥 정보가 있고 대미지가 있을 경우에만 동작
	{
		if (!m_bIsTrapOn)
		{
			m_fHP -= dynamic_cast<CFloor*>(pGameObject)->Get_Damage();
			m_bIsTrapOn = true;
			Engine::Play_Sound(L"Damaged_Floor.wav", CHANNELID::SOUND_EFFECT, 0.5f);

			CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectRedFlash", L"Com_Effect");
			if (pComponent)
				static_cast<CEffect*>(pComponent)->Operate_Effect();
		}
	}
}


void CPlayer::Speed_Terrain()
{
	_vec3 vPos;
	CGameObject* pGameObject;
	m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &vPos);

	pGameObject = Engine::FloorRayCast2(vPos); // 레이픽킹된 바닥 정보를 받아옴

	if (pGameObject != nullptr && dynamic_cast<CFloor*>(pGameObject)->Get_SlidSpeed() == true)// 슬라이드 타일인지 체크
	{
		m_bIsSlideOn = true;
		m_bLegUse = true;
		m_Leg_CurState = SLIDING;
		m_pAnimator[LEG]->PlayAnimation(L"Leg_Sliding", false);
	}
	else
	{
		m_bIsSlideOn = false;
	}
}


void CPlayer::SetAnimation()
{
	//Right
	//Jonghan
	m_pAnimator[RIGHT]->CreateAnimation(L"Execution_Knife", m_pRight_TextureCom[PISTOL][EXECUTION], 2.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Player_Start", m_pRight_TextureCom[PISTOL][START], 3.5f);
	//Beomseung
	m_pAnimator[RIGHT]->CreateAnimation(L"Pistol_Idle", m_pRight_TextureCom[PISTOL][IDLE], 8.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Pistol_Shoot", m_pRight_TextureCom[PISTOL][SHOOT], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Pistol_Reload", m_pRight_TextureCom[PISTOL][RELOAD], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Pistol_Change", m_pRight_TextureCom[PISTOL][CHANGE], 18.f);

	m_pAnimator[RIGHT]->CreateAnimation(L"Rifle_Idle", m_pRight_TextureCom[RIFLE][IDLE], 8.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Rifle_Shoot", m_pRight_TextureCom[RIFLE][SHOOT], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Rifle_Reload", m_pRight_TextureCom[RIFLE][RELOAD], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Rifle_Change", m_pRight_TextureCom[RIFLE][CHANGE], 18.f);

	m_pAnimator[RIGHT]->CreateAnimation(L"Shotgun_Idle", m_pRight_TextureCom[SHOTGUN][IDLE], 8.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Shotgun_Shoot", m_pRight_TextureCom[SHOTGUN][SHOOT], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Shotgun_Reload", m_pRight_TextureCom[SHOTGUN][RELOAD], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Shotgun_Change", m_pRight_TextureCom[SHOTGUN][CHANGE], 18.f);

	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_Idle", m_pRight_TextureCom[SNIPER][IDLE], 8.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_Shoot", m_pRight_TextureCom[SNIPER][SHOOT], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_Reload", m_pRight_TextureCom[SNIPER][RELOAD], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_ZoomIn", m_pRight_TextureCom[SNIPER][ZOOMIN], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_ZoomIng", m_pRight_TextureCom[SNIPER][ZOOMING], 5.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_ZoomOut", m_pRight_TextureCom[SNIPER][ZOOMOUT], 13.f);

	//MiniGun
	m_pAnimator[RIGHT]->CreateAnimation(L"MiniGun_GunPoint_Idle", m_pRight_TextureCom[MINIGUN][IDLE], 18.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"MiniGun_GunPoint_Shoot", m_pRight_TextureCom[MINIGUN][SHOOT], 24.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"MiniGun_GunPoint_Change", m_pRight_TextureCom[MINIGUN][CHANGE], 12.f);
	m_pAnimator[LEFT]->CreateAnimation(L"MiniGun_Body_Idle", m_pLeft_TextureCom[MINIGUN_BODY_IDLE], 13.f);
	m_pAnimator[LEFT]->CreateAnimation(L"MiniGun_Body_Change", m_pLeft_TextureCom[MINIGUN_BODY_CHANGE], 13.f);
	m_pAnimator[LEG]->CreateAnimation(L"MiniGun_Panel_Change", m_pLeg_TextureCom[MINIGUN_PANEL_CHANGE], 13.f);
	m_pAnimator[LEG]->CreateAnimation(L"MiniGun_Panel_Idle", m_pLeg_TextureCom[MINIGUN_PANEL_IDLE], 13.f);

	m_pAnimator[RIGHT]->CreateAnimation(L"Katana_Idle", m_pRight_TextureCom[KATANA][IDLE], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Katana_Shoot", m_pRight_TextureCom[KATANA][SHOOT], 18.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Katana_Change", m_pRight_TextureCom[KATANA][CHANGE], 18.f);

	//Left
	m_pAnimator[LEFT]->CreateAnimation(L"Left_Idle", m_pLeft_TextureCom[LEFT_IDLE], 8.f);
	m_pAnimator[LEFT]->CreateAnimation(L"Left_Change", m_pLeft_TextureCom[LEFT_CHANGE], 18.f);
	m_pAnimator[LEFT]->CreateAnimation(L"Left_Drink", m_pLeft_TextureCom[DRINK], 8.f);
	m_pAnimator[LEFT]->CreateAnimation(L"Left_Execution", m_pLeft_TextureCom[LEFT_EXECUTION], 2.f);

	//Leg
	m_pAnimator[LEG]->CreateAnimation(L"Leg_Idle", m_pLeg_TextureCom[LEG_IDLE], 8.f);
	m_pAnimator[LEG]->CreateAnimation(L"Leg_Kick", m_pLeg_TextureCom[KICK], 18.f);
	m_pAnimator[LEG]->CreateAnimation(L"Leg_Sliding", m_pLeg_TextureCom[SLIDING], 18.f);

	//m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Change", false);
	//m_pRight_TransformCom->Set_Scale(600.f, 600.f, 0.f);
	//m_pRight_TransformCom->Set_Pos(m_vDefaultPos[RIGHT]);
	//
	
	//
	//m_pAnimator[LEG]->PlayAnimation(L"Leg_Idle", false);
	//m_pLeg_TransformCom->Set_Scale(m_vDefaultSize[LEG]);
	//m_pLeg_TransformCom->Set_Pos(0, WINCY / -3.f, 2.f);

	m_pAnimator[RIGHT]->PlayAnimation(L"Player_Start", false);
	m_Right_CurState = START;
	m_bLeftHandUse = false;

	m_pAnimator[LEFT]->PlayAnimation(L"Left_Idle", true);
	m_pLeft_TransformCom->Set_Scale(m_vDefaultSize[LEFT]);
	m_pLeft_TransformCom->Set_Pos(m_vDefaultPos[LEFT]);

	m_pAnimator[LEG]->PlayAnimation(L"Leg_Idle", false);
	m_pLeg_TransformCom->Set_Scale(m_vDefaultSize[LEG]);
	m_pLeg_TransformCom->Set_Pos(0, WINCY / -3.f, 2.f);
}

void CPlayer::Animation_End_Check()
{
	if (m_pAnimator[LEG]->GetCurrAnim()->GetFinish())
	{
		if (m_WeaponState == MINIGUN)
		{
			m_Leg_CurState = MINIGUN_PANEL_IDLE;
			m_pAnimator[LEG]->PlayAnimation(L"MiniGun_Panel_Idle", false);
		}
		else if (m_Leg_CurState == KICK)
		{
			m_Leg_CurState = LEG_IDLE;
			m_bLegUse = false;
		}
		else {
			m_Leg_CurState = LEG_IDLE;
			//m_bLegUse = false;
		}
		m_flinear[LEG] = 0.f;
	}

	if (m_pAnimator[RIGHT]->GetCurrAnim()->GetFinish())
	{
		if (m_Right_CurState == START)
		{

			_int iTemp = rand() % 100;

			if (m_bIsBoss && !m_bIsShop)
			{
				if (nullptr != dynamic_cast<CBoss_Humanoid*>(Engine::Get_CurrScene()->Get_GameObject(L"Layer_Monster", L"Boss_Humanoid")))
				{
					Engine::Play_BGM(L"Boss_Sniper.wav", 0.6f);
				}
			}
			else if (!m_bIsShop)
			{
				if (0 == iTemp % 6)
					Engine::Play_BGM(L"Stage_01.wav", 0.4f);
				else if (1 == iTemp % 6)
					Engine::Play_BGM(L"Stage_02.wav", 0.4f);
				else if (2 == iTemp % 6)
					Engine::Play_BGM(L"Stage_04.wav", 0.4f);
				else if (3 == iTemp % 6)
					Engine::Play_BGM(L"Stage_05.wav", 0.4f);
				else if (4 == iTemp % 6)
					Engine::Play_BGM(L"Stage_06.wav", 0.4f);
				else
					Engine::Play_BGM(L"Stage_08.wav", 0.4f);
			}

			m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Change", false);
			m_pRight_TransformCom->Set_Scale(600.f, 600.f, 0.f);
			m_pRight_TransformCom->Set_Pos(m_vDefaultPos[RIGHT]);
			m_Right_CurState = IDLE;
			m_pAnimator[LEFT]->PlayAnimation(L"Left_Change", false);
			m_Left_CurState = LEFT_CHANGE;
			m_pLeft_TransformCom->Set_Scale(m_vDefaultSize[LEFT]);
			m_pLeft_TransformCom->Set_Pos(m_vDefaultPos[LEFT]);
			m_bLeftHandUse = true;
			m_fTimerHP = 20.f;
			return;
		}
		switch (m_WeaponState) {
		case PISTOL:
			m_Right_CurState = IDLE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Idle", true);
			break;
		case RIFLE:
			m_Right_CurState = IDLE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Idle", true);
			break;
		case SHOTGUN:
			m_Right_CurState = IDLE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Shotgun_Idle", true);
			break;
		case SNIPER:
			if (m_Right_CurState == ZOOMIN)
			{
				m_Right_CurState = ZOOMING;
				m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_ZoomIng", true);
				break;
			}
			m_Right_CurState = IDLE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_Idle", true);
			break;
		case KATANA:
			m_Right_CurState = IDLE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Katana_Idle", true);
			break;
		case MINIGUN:
			m_Right_CurState = IDLE;
			m_pAnimator[RIGHT]->PlayAnimation(L"MiniGun_GunPoint_Idle", true);
			break;
		}
		m_bLeftHandUse = true;
		m_flinear[RIGHT] = 0;
	}
	if (m_pAnimator[LEFT]->GetCurrAnim()->GetFinish())
	{
		if (m_WeaponState == MINIGUN) {
			m_Left_CurState = MINIGUN_BODY_IDLE;
			m_pAnimator[LEFT]->PlayAnimation(L"MiniGun_Body_Idle", false);
		}
		else if (m_Left_CurState == LEFT_CHANGE)
		{
			m_Left_CurState = LEFT_IDLE;
			m_pAnimator[LEFT]->PlayAnimation(L"Left_Idle", true);
		}
		else if (m_Left_CurState == LEFT_EXECUTION)
		{
			m_bLegUse = true;
			m_Leg_CurState = KICK;
			m_pAnimator[LEG]->PlayAnimation(L"Leg_Kick", false);
			Engine::Play_Sound(L"Kick.wav", CHANNELID::SOUND_PLAYER, 0.7f);
			m_Left_CurState = LEFT_IDLE;
			m_pAnimator[LEFT]->PlayAnimation(L"Left_Idle", true);
			m_bIsDrinking = false;
			m_bIsShaking = true;
			m_fShakingSize = 20.f;
			m_fShakingTimer = 0.3f;

			ActionUI(0);
		}
		else {
			m_Left_CurState = LEFT_IDLE;
			m_pAnimator[LEFT]->PlayAnimation(L"Left_Idle", true);
			m_bIsDrinking = false;
		}
		m_flinear[LEFT] = 0.f;
	}
}

void CPlayer::Animation_Pos()
{
	_vec3 vPos;
	_vec3 vStart;
	_vec3 vEnd;

	switch (m_Leg_CurState) {
	case KICK:
		if (m_pAnimator[LEG]->GetCurrAnim()->GetCurrFrame() >= 5)
		{
			m_pLeg_TransformCom->Set_Pos(0, -400, 2.f);
		}
		break;
	case MINIGUN_PANEL_CHANGE:
		m_flinear[LEG] += 0.1f;
		if (m_flinear[LEG] >= 1.f) {
			m_flinear[LEG] = 1.f;
		}
		m_pLeg_TransformCom->Set_Scale(m_vDefaultSize[LEG] * 1.2f);
		vStart = { 0.f, -WINCY + 100.f , 2.f };
		vEnd = { 0.f,-300.f,2.f };
		D3DXVec3Lerp(&vPos, &vStart, &vEnd, m_flinear[LEG]);
		m_pLeg_TransformCom->Set_Pos(vPos);
		break;
	case MINIGUN_PANEL_IDLE:
		m_pLeg_TransformCom->Set_Scale(m_vDefaultSize[LEG] * 1.2f);
		m_pLeg_TransformCom->Set_Pos(0.f, -400.f, 2.f);
		break;
	default:
		m_pLeg_TransformCom->Set_Pos(0, WINCY / -3.f, 2.f);
		break;
	}
	switch (m_WeaponState)
	{
	case PISTOL:
		switch (m_Right_CurState)
		{
		case CHANGE:
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 1.3f);
			m_pRight_TransformCom->Set_Pos(m_vDefaultPos[RIGHT]);
			break;
		case RELOAD:
			if (m_pAnimator[RIGHT]->GetCurrAnim()->GetCurrFrame() >= 4 && m_pAnimator[RIGHT]->GetCurrAnim()->GetCurrFrame() <= 9)
			{
				m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 1.3f);
			}
			else {
				m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT]);
			}
			break;
		case EXECUTION:
			vStart = { 3000.f, 0.f, 1.f };
			vEnd = { 270.f, -125.f, 1.f };
			D3DXVec3Lerp(&vPos, &vStart, &vEnd, m_flinear[RIGHT]);

			// 규빈: 형님 이렇게 대충 한번 해봤는데요... 이거 맞을까요?
			if (m_flinear[RIGHT] > 0.1f)
			{
				CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectExecutionBlood", L"Com_EffectFirst");
				if (pComponent)
					static_cast<CEffect*>(pComponent)->Operate_Effect();
			}
			if (m_flinear[RIGHT] > 0.7f)
			{
				CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectExecutionBlood", L"Com_EffectSecond");
				if (pComponent)
					static_cast<CEffect*>(pComponent)->Operate_Effect();

				m_fHP = 99.f; //Full Hp Restore
				m_fTimerHP = 20.f;
			}
			m_pRight_TransformCom->Set_Pos(vPos);
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 0.7f);
			break;
		case START:
			m_pRight_TransformCom->Set_Pos(0.f, m_vDefaultPos[RIGHT].y + 250.f, 2.f);
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 0.8f);
			break;
		default:
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT]);
			m_pRight_TransformCom->Set_Pos(m_vDefaultPos[RIGHT]);
			break;
		}
		break;
	case RIFLE:
		switch (m_Right_CurState)
		{
		case IDLE:
		case SHOOT:
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] / 2.f);
			m_pRight_TransformCom->Set_Pos(m_vDefaultPos[RIGHT]);
			break;
		case RELOAD:
		case CHANGE:
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT]);
			m_pRight_TransformCom->Set_Pos(0.f, -WINCY / 3.f, 2.f);
			break;
		case EXECUTION:
			vStart = { 3000.f, 0.f, 1.f };
			vEnd = { 270.f, -125.f, 1.f };
			D3DXVec3Lerp(&vPos, &vStart, &vEnd, m_flinear[RIGHT]);

			if (m_flinear[RIGHT] > 0.1f)
			{
				CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectExecutionBlood", L"Com_EffectFirst");
				if (pComponent)
					static_cast<CEffect*>(pComponent)->Operate_Effect();
			}
			if (m_flinear[RIGHT] > 0.7f)
			{
				CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectExecutionBlood", L"Com_EffectSecond");
				if (pComponent)
					static_cast<CEffect*>(pComponent)->Operate_Effect();

				m_fHP = 99.f; //Full Hp Restore
				m_fTimerHP = 20.f;
			}
			m_pRight_TransformCom->Set_Pos(vPos);
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 0.7f);
			break;
		}
		break;
	case SHOTGUN:
		switch (m_Right_CurState)
		{
		case IDLE:
		case SHOOT:
		case RELOAD:
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT]);
			m_pRight_TransformCom->Set_Pos(m_vDefaultPos[RIGHT]);
			break;
		case CHANGE:
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 1.2f);
			if (m_pAnimator[RIGHT]->GetCurrAnim()->GetCurrFrame() >= 10)
			{
				_vec3 vRight;
				m_pRight_TransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);
				m_pRight_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), 1, 10.f);
			}
			else {
				m_pRight_TransformCom->Set_Pos(0.f, WINCY / -4.f, 2.f);
			}
			break;
		case EXECUTION:
			vStart = { 3000.f, 0.f, 1.f };
			vEnd = { 270.f, -125.f, 1.f };
			D3DXVec3Lerp(&vPos, &vStart, &vEnd, m_flinear[RIGHT]);

			if (m_flinear[RIGHT] > 0.1f)
			{
				CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectExecutionBlood", L"Com_EffectFirst");
				if (pComponent)
					static_cast<CEffect*>(pComponent)->Operate_Effect();
			}
			if (m_flinear[RIGHT] > 0.7f)
			{
				CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectExecutionBlood", L"Com_EffectSecond");
				if (pComponent)
					static_cast<CEffect*>(pComponent)->Operate_Effect();

				m_fHP = 99.f; //Full Hp Restore
				m_fTimerHP = 20.f;
			}
			m_pRight_TransformCom->Set_Pos(vPos);
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 0.7f);
			break;
		}
		break;
	case SNIPER:
		switch (m_Right_CurState)
		{
		case IDLE:
		case SHOOT:
		case ZOOMOUT:
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT]);
			m_pRight_TransformCom->Set_Pos(m_vDefaultPos[RIGHT]);
			m_pCComponentCamera->SetFov(D3DXToRadian(60.f));
			break;
		case RELOAD:
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT]);
			m_pRight_TransformCom->Set_Pos(0.f, WINCY / -3.f, 2.f);
			break;
		case ZOOMIN:
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT]);
			m_pRight_TransformCom->Set_Pos(m_vDefaultPos[RIGHT]);
			break;
		case ZOOMING:
			m_bLeftHandUse = false;
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 2);
			m_pRight_TransformCom->Set_Pos(0.f, 0.f, 2.f);
			m_pCComponentCamera->SetFov(D3DXToRadian(10.f));
			break;
		}
		break;
	case KATANA:
		switch (m_Right_CurState)
		{
		case IDLE:
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 1.1f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -4.f, 2.f);
			break;
		case SHOOT:
			// 규빈 - 카타나 공격 이펙트
			{
				_float fCurFrame = m_pAnimator[RIGHT]->GetCurrAnim()->GetCurrFrame();
				if (fCurFrame >= 2.f && fCurFrame <= 3.f)
				{
					CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectKatanaAttack", L"Com_EffectFirst");
					static_cast<CEffect*>(pComponent)->Operate_Effect();
				}
				if (fCurFrame >= 9.f && fCurFrame <= 10.f)
				{
					CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectKatanaAttack", L"Com_EffectSecond");
					static_cast<CEffect*>(pComponent)->Operate_Effect();
				}
				if (fCurFrame >= 16.f && fCurFrame <= 17.f)
				{
					CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectKatanaAttack", L"Com_EffectThird");
					static_cast<CEffect*>(pComponent)->Operate_Effect();
				}
			}
			//상점 클릭하면
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 1.4f);
			m_pRight_TransformCom->Set_Pos(0.f, 0.f, 2.f);
			break;
		case CHANGE:
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 1.5f);
			m_pRight_TransformCom->Set_Pos(0.f, WINCY / -3.f, 2.f);
			break;
		case EXECUTION:
			vStart = { 3000.f, 0.f, 1.f };
			vEnd = { 270.f, -125.f, 1.f };
			D3DXVec3Lerp(&vPos, &vStart, &vEnd, m_flinear[RIGHT]);

			if (m_flinear[RIGHT] > 0.1f)
			{
				CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectExecutionBlood", L"Com_EffectFirst");
				if (pComponent)
					static_cast<CEffect*>(pComponent)->Operate_Effect();
			}
			if (m_flinear[RIGHT] > 0.7f)
			{
				CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectExecutionBlood", L"Com_EffectSecond");
				if (pComponent)
					static_cast<CEffect*>(pComponent)->Operate_Effect();

				m_fHP = 99.f; //Full Hp Restore
				m_fTimerHP = 20.f;
			}
			m_pRight_TransformCom->Set_Pos(vPos);
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 0.7f);
			break;
		}
		break;
	case MINIGUN:
		switch (m_Right_CurState) {
		case CHANGE:
			if (m_flinear[RIGHT] >= 1.f) {
				m_flinear[RIGHT] = 1.f;
			}
			vStart = { 0.f, -WINCY + 100.f , 2.f };
			vEnd = { 0.f,-150.f,2.f };
			D3DXVec3Lerp(&vPos, &vStart, &vEnd, m_flinear[RIGHT]);
			m_pRight_TransformCom->Set_Pos(vPos);
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 0.5f);
			break;
		case IDLE:
			m_pRight_TransformCom->Set_Pos(0.f, -150.f, 2.f);
			m_pRight_TransformCom->Set_Scale(m_vDefaultSize[RIGHT] * 0.5f);
			break;
		}
	}
	switch (m_Left_CurState)
	{
	case LEFT_IDLE:
		m_pLeft_TransformCom->Set_Scale(m_vDefaultSize[LEFT]);
		m_pLeft_TransformCom->Set_Pos(m_vDefaultPos[LEFT]);
		break;
	case LEFT_CHANGE:
		m_flinear[LEFT] += 0.2;
		if (m_flinear[LEFT] >= 1.f) {
			m_flinear[LEFT] = 1.f;
		}
		m_pLeft_TransformCom->Set_Scale(m_vDefaultSize[LEFT]);
		vStart = { m_vDefaultPos[LEFT].x, -WINCY, m_vDefaultPos[LEFT].z };
		vEnd = m_vDefaultPos[LEFT];
		D3DXVec3Lerp(&vPos, &vStart, &vEnd, m_flinear[LEFT]);
		m_pLeft_TransformCom->Set_Pos(vPos);
		break;
	case DRINK:
		m_pLeft_TransformCom->Set_Scale(m_vDefaultSize[LEFT]);
		m_pLeft_TransformCom->Set_Pos(m_vDefaultPos[LEFT]);
		break;
	case MINIGUN_BODY_IDLE:
		m_pLeft_TransformCom->Set_Pos(0.f, -250.f, 2.f);
		m_pLeft_TransformCom->Set_Scale(m_vDefaultSize[LEFT] * 0.5f);
		break;
	case MINIGUN_BODY_CHANGE:
		if (m_flinear[LEFT] >= 1.f) {
			m_flinear[LEFT] = 1.f;
		}
		vStart = { 0.f, -WINCY + 100.f , 2.f };
		vEnd = { 0.f,-250.f,2.f };
		D3DXVec3Lerp(&vPos, &vStart, &vEnd, m_flinear[RIGHT]);
		m_pLeft_TransformCom->Set_Pos(vPos);
		m_pLeft_TransformCom->Set_Scale(m_vDefaultSize[LEFT] * 0.5f);
		break;
	case LEFT_EXECUTION:
		if (m_flinear[RIGHT] >= 1.f) {
			m_flinear[RIGHT] = 1.f;
		}
		m_flinear[RIGHT] += 0.045f;
		vStart = { -1300.f, -150.f, 1.f };
		vEnd = { 900.f, -450.f, 1.f };
		D3DXVec3Lerp(&vPos, &vStart, &vEnd,  m_flinear[RIGHT]);
		m_pLeft_TransformCom->Set_Pos(vPos);
		break;
	}
}

void CPlayer::Rotate_Arms(const _bool& _bIsRecover)
{
	if (_bIsRecover && m_bIsRotation) //원상복구
	{
		m_pRight_TransformCom->Rotation(ROTATION::ROT_Z, -1.f);
		m_pLeft_TransformCom->Rotation(ROTATION::ROT_Z, 0.5f);
		m_bIsRotation = false;
	}
	else if (!_bIsRecover && !m_bIsRotation) //돌릴때(1. 대쉬할 때, )
	{
		m_pRight_TransformCom->Rotation(ROTATION::ROT_Z, 1.f);
		m_pLeft_TransformCom->Rotation(ROTATION::ROT_Z, -0.5f);
		m_bIsRotation = true;
	}
}

void CPlayer::OnCollision(CCollider& _pOther)
{
	CMonster* pMonster = dynamic_cast<CMonster*>(_pOther.GetOwner());

	if (m_Leg_CurState == KICK && m_pAnimator[LEG]->GetCurrAnim()->GetCurrFrame() <= 1.f)
	{
		_vec3 vLook;
		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		if (pMonster == nullptr)
			return;
		if (pMonster->Get_IsDead())
			return;

		pMonster->AddForce(16.5f, vLook, 5.f);
	}
	if (m_Leg_CurState == SLIDING)
	{
		_vec3 vLook;
		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		if (pMonster == nullptr)
			return;
		if (pMonster->Get_IsDead())
			return;

		pMonster->AddForce(16.5f, vLook, 5.f);
	}
	if (m_WeaponState == KATANA && m_Right_CurState == SHOOT)
	{
		if (pMonster == nullptr) {
			return;
		}
		if (pMonster->Get_IsDead())
		{
			return;
		}
		if (m_pAnimator[RIGHT]->GetCurrAnim()->GetCurrFrame() <= 2.f) {
		}
		else if (7.f < m_pAnimator[RIGHT]->GetCurrAnim()->GetCurrFrame() && 11.f > m_pAnimator[RIGHT]->GetCurrAnim()->GetCurrFrame()) {
		}
		else if (14.f < m_pAnimator[RIGHT]->GetCurrAnim()->GetCurrFrame() && 18.f > m_pAnimator[RIGHT]->GetCurrAnim()->GetCurrFrame()) {
		}
		else
			return;
		pMonster->Damaged_By_Player(DAMAGED_STATE::DAMAGED_KATANA, m_fDamage);

	}

	Collide_Wall(_pOther);
}

void CPlayer::OnCollisionEnter(CCollider& _pOther)
{
	CComponent* pComponent = nullptr;
	CMonster* pMonster = dynamic_cast<CMonster*>(_pOther.GetOwner());
	if (nullptr != pMonster) //is it Monster == _pOther
	{
		if (!pMonster->Get_IsDead())
		{
			if (m_bLegUse)
			{
				if (m_bIsHasItem)
				{
					_vec3 vLook;
					m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
					if (pMonster->Get_Execution(vLook, true)) //Hunmanoid
					{
						Rotate_Arms(true);
						m_flinear[RIGHT] = 0.f;
						m_bLegUse = false;
						m_Left_CurState = LEFT_EXECUTION;
						m_pAnimator[LEFT]->PlayAnimation(L"Left_Execution", false);
						m_Right_CurState = EXECUTION;
						m_pAnimator[RIGHT]->PlayAnimation(L"Execution_Knife", false);
						Engine::Play_Sound(L"Execution_Combo.wav", CHANNELID::SOUND_PLAYER_GUN, 0.7f);
						m_fDashSpeed = 0.f;
						m_bIsHasItem = false;
					}
					else //Drone
					{
						m_Leg_CurState = KICK;
						m_pAnimator[LEG]->PlayAnimation(L"Leg_Kick", false);
						Engine::Play_Sound(L"Kick.wav", CHANNELID::SOUND_PLAYER, 0.7f);
						m_bIsShaking = true;
						m_fShakingSize = 20.f;
						m_fShakingTimer = 0.3f;
					}
				}
				else
				{
					m_Leg_CurState = KICK;
					m_pAnimator[LEG]->PlayAnimation(L"Leg_Kick", false);
					Engine::Play_Sound(L"Kick.wav", CHANNELID::SOUND_PLAYER, 0.7f);
					m_bIsShaking = true;
					m_fShakingSize = 20.f;
					m_fShakingTimer = 0.3f;
					pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectKick", L"Com_Effect");
					static_cast<CEffect*>(pComponent)->Operate_Effect();
				}
			}
			else
			{
				pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPlayerBlood", L"Com_Effect");
				static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);

				m_fTimerHP -= 1.f;
				m_fHP -= 1.f;

				pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectRedFlash", L"Com_Effect");
				if (pComponent)
					static_cast<CEffect*>(pComponent)->Operate_Effect();

			}
		}

		return;
	}

	CBullet* pBullet = dynamic_cast<CBullet*>(_pOther.GetOwner()); //is it Monster_Bullet == _pOther

	if (nullptr != pBullet)
	{
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPlayerBlood", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);

		m_fTimerHP -= 1.f;
		m_fHP -= 1.f;

		Engine::Play_Sound(L"Blood_02.wav", CHANNELID::SOUND_PLAYER_DAMAGED, 0.65f);

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectRedFlash", L"Com_Effect");
		if (pComponent)
			static_cast<CEffect*>(pComponent)->Operate_Effect();
		return;
	}

	if (nullptr != dynamic_cast<CDrink*>(_pOther.GetOwner()) && !m_bIsDrinking) //처형과 음료수 구분하기
	{
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectHeal", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);

		m_fHP += 10.f;
		if (99.f < m_fHP)
			m_fHP = 99.f;

		m_fTimerHP = 20.f;

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectGreenFlash", L"Com_Effect");
		if (pComponent)
			static_cast<CEffect*>(pComponent)->Operate_Effect();
		Engine::Play_Sound(L"DrinkSoda.wav", CHANNELID::SOUND_PLAYER, 0.9f);
		m_bIsDrinking = true;
		m_Left_CurState = DRINK;
		m_pAnimator[LEFT]->PlayAnimation(L"Left_Drink", false);

		ActionUI(1);

		return;
	}
	else if (nullptr != dynamic_cast<CItem*>(_pOther.GetOwner()))
	{
		
		//this code will change to Setting UI
		return;
	}
	else if(nullptr != dynamic_cast<CDrinkMachine*>(_pOther.GetOwner()))
	{
		if (m_bLegUse)
		{
			m_Leg_CurState = KICK;
			m_pAnimator[LEG]->PlayAnimation(L"Leg_Kick", false);
			m_fDashSpeed = 0.f;
			m_bIsShaking = true;
			m_fShakingSize = 20.f;
			m_fShakingTimer = 0.3f;
			dynamic_cast<CDrinkMachine*>(_pOther.GetOwner())->Break_Machine();
		}
	}
	
	Collide_Wall(_pOther);
}

void CPlayer::OnCollisionExit(CCollider& _pOther)
{

}

void CPlayer::Collide_Wall(CCollider& _pOther)
{
	// 벽 충돌 밀어내기
	CGameObject* pGameObject = _pOther.GetOwner();

	if (Engine::Get_CurrScene()->Get_Layer(pGameObject) == L"Layer_Wall")
	{
		CCollider::AABB* vBoxThis = m_pColliderCom->GetAABB();
		CCollider::AABB* vBoxOther = _pOther.GetAABB();

		_vec3 vCenterThis = (vBoxThis->vMin + vBoxThis->vMax) / 2.f;
		_vec3 vCenterOther = (vBoxOther->vMin + vBoxOther->vMax) / 2.f;

		_vec3 vOverlap = vCenterThis - vCenterOther;
		_float fOverlapX = (vBoxThis->vMax.x - vBoxThis->vMin.x) / 2.0f + (vBoxOther->vMax.x - vBoxOther->vMin.x) / 2.0f - fabs(vOverlap.x);
		_float fOverlapZ = (vBoxThis->vMax.z - vBoxThis->vMin.z) / 2.0f + (vBoxOther->vMax.z - vBoxOther->vMin.z) / 2.0f - fabs(vOverlap.z);

		if (!(fOverlapX < 0 || fOverlapZ < 0))
		{
			_vec3 vPos;
			m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &vPos);

			if (fOverlapX < fOverlapZ)
			{
				if (vOverlap.x > 0)
					m_pBody_TransformCom->Set_Pos(vPos.x + fOverlapX, vPos.y, vPos.z);
				else
					m_pBody_TransformCom->Set_Pos(vPos.x - fOverlapX, vPos.y, vPos.z);
			}
			else
			{
				if (vOverlap.z > 0)
					m_pBody_TransformCom->Set_Pos(vPos.x, vPos.y, vPos.z + fOverlapZ);
				else
					m_pBody_TransformCom->Set_Pos(vPos.x, vPos.y, vPos.z - fOverlapZ);
			}

			m_pBody_TransformCom->Update_Component(0.f);
			m_pColliderCom->LateUpdate_Component();
			m_pCComponentCamera->LateUpdate_Component();
		}
	}
}

void CPlayer::Calculate_TimerHP(const _float& _fTimeDelta)
{
	if (m_bIsClear)
		return;

	if (0.f < m_fTimerHP)
		m_fTimerHP -= _fTimeDelta;
	else
		m_fTimerHP = 0.f;

	if (20.f < m_fTimerHP)
		m_fTimerHP = 20.f;
	//if u need, Use (_int)m_fTimerHP;

	m_fSmileTimer -= _fTimeDelta;
}

void CPlayer::Skill_Timer()
{
	if (1.5f < m_fTime_Skill) //느려진상황이니 원상복귀
	{
		Engine::Set_PlayerSkillTimer(1.0f);
		m_fTime_Skill = 1.0f;

		m_fSkillCool = 0.f;
		m_fSkillTime = 0.f;

		Play_Sound(L"Slowmo_Up.wav", CHANNELID::SOUND_INTERFACE, 0.8f);
	}
	else
	{
		Engine::Set_PlayerSkillTimer(0.33f); //세배 느리게함
		m_fTime_Skill = 3.0f;

		Play_Sound(L"Slowmo_Down.wav", CHANNELID::SOUND_INTERFACE, 0.8f);
	}
}

void CPlayer::Moving_Rotate()
{
	_matrix matWorld, matRot, matResult;
	if (m_bIsLeft)
	{
		D3DXMatrixRotationZ(&matRot, D3DXToRadian(-5.f));
		matWorld = *m_pBody_TransformCom->Get_WorldMatrix();

		matResult = matRot * matWorld;
		m_pBody_TransformCom->Set_WorldMatrix(&matResult);
	}
	else if (m_bIsRight)
	{
		D3DXMatrixRotationZ(&matRot, D3DXToRadian(5.f));
		matWorld = *m_pBody_TransformCom->Get_WorldMatrix();

		matResult = matRot * matWorld;
		m_pBody_TransformCom->Set_WorldMatrix(&matResult);
	}
}

void CPlayer::Shaking_Camera(const _float& _fTimeDelta)
{
	if (m_bIsShaking)
	{
		_matrix matWorld, matRot, matResult;

		m_fShakingTimer -= _fTimeDelta;
		_int iRan = rand() % (_int)m_fShakingSize;
		_int iMinus = rand() % 2;
		_float fRan;
		if (0 < iMinus)
			fRan = iRan * 0.1f;
		else
			fRan = iRan * -0.1f;

		D3DXMatrixRotationZ(&matRot, D3DXToRadian(fRan));
		matWorld = *m_pBody_TransformCom->Get_WorldMatrix();

		matResult = matRot * matWorld;
		m_pBody_TransformCom->Set_WorldMatrix(&matResult);
		if (0.f > m_fShakingTimer)
		{
			m_bIsShaking = false;
		}
	}
}

void CPlayer::ActionUI(_int _iActionType)
{
	for (_int i = 0; i < 3; i++)
	{
		CUIDown* pUIDown = static_cast<CUIDown*>(Engine::Activate_UI(UITYPE::UI_DOWN));
		pUIDown->Init(i);
	}

	if (Engine::Get_ListUI(UITYPE::UI_ACTION)->empty())
	{
		Engine::Activate_UI(UITYPE::UI_ACTION);
		static_cast<CUIAction*>(Engine::Get_ListUI(UITYPE::UI_ACTION)->front())->Set_ActionType((CUIAction::ACTION_TYPE)_iActionType);
	}
	else if (static_cast<CUIAction*>(Engine::Get_ListUI(UITYPE::UI_ACTION)->front())->Get_ActionType() != (CUIAction::ACTION_TYPE)_iActionType)
	{
		Engine::Deactivate_UI(UITYPE::UI_ACTION);
		Engine::Activate_UI(UITYPE::UI_ACTION);
		static_cast<CUIAction*>(Engine::Get_ListUI(UITYPE::UI_ACTION)->front())->Set_ActionType((CUIAction::ACTION_TYPE)_iActionType);
	}

	Engine::Play_Sound(L"FullLife.wav", CHANNELID::SOUND_INTERFACE, 1.f);
}

void CPlayer::Free()
{
	Engine::CGameObject::Free();
}