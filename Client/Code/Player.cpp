#include "pch.h"
#include "..\Header\Player.h"
#include "Export_System.h"
#include "Export_Utility.h"
// ±Ôºó
#include "../Header/EffectPool.h"
#include "../Header/EffectMuzzleFlash.h"
#include "../Header/Monster.h"
#include "../Header/Item.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CCharacter(_pGraphicDev)
	, m_pRight_TransformCom(nullptr)
	, m_pBody_TransformCom(nullptr)
	, m_pCComponentCamera(nullptr)
	, m_pLeft_TransformCom(nullptr)
	, m_pLeg_TransformCom(nullptr)
	, m_pPlayer_Buffer(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pColliderCom(nullptr)
	, bJumpCheck(false)
	, bLeftHandUse(true)
	, bLegUse(false)
	, fJumpPower(0.f)
	, flinear(0.f)
	, fTilePos(0.f)
	, fSpeed(0.f)
	, m_fDashSpeed(0.f)
	, m_Right_CurState(CHANGE)
	, m_Right_PreState(RIGHT_STATE_END)
	, m_Left_CurState(LEFT_IDLE)
	, m_Left_PreState(LEFT_STATE_END)
	, m_Leg_CurState(LEG_IDLE)
	, m_Leg_PreState(LEG_STATE_END)
	, m_WeaponState(PISTOL)
	, m_bIsHasItem(false)
	, m_eItemType(Engine::ITEM_TYPE::ITEM_END)
	//Beomseung
{
	ZeroMemory(&m_fFrameStart, sizeof(m_fFrameStart));
	ZeroMemory(&m_fFrameEnd, sizeof(m_fFrameEnd));
	ZeroMemory(&m_fFrameSpeed, sizeof(m_fFrameSpeed));
	ZeroMemory(&m_pAnimator, sizeof(m_pAnimator));
	vDefaultPos[RIGHT] = { WINCX / 3.f,WINCY / -3.f,2.f };
	vDefaultPos[LEFT] = { WINCX / -3.f,WINCY / -3.f,2.f };
	vDefaultSize[RIGHT] = { 500.f,500.f,0.f };
	vDefaultSize[LEFT] = { 300.f,300.f,0.f };
	vDefaultSize[LEG] = { 500.f,500.f,0.f };

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

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CPlayer* pPlayer = new CPlayer(_pGraphicDev);

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

	//m_pRight_TransformCom->Set_Pos(2.f, 1.f, 2.f);
	//m_pLeft_TransformCom->Set_Pos(-2.f, 1.f, 2.f);

	m_pColliderCom->SetTransform(m_pBody_TransformCom);
	m_pColliderCom->SetRadius(1.f);
	m_pColliderCom->SetShow(true);
	m_pColliderCom->SetActive(true);

	fSpeed = 10.f;

	// ±Ôºó : ¾ËÆÄ¼ÒÆÃÀ» À§ÇÑ ¼³Á¤, 
	m_fViewZ = 10.f;
	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& _fTimeDelta)
{
	if (Engine::Get_ControllerID() == CONTROLLERID::CONTROL_PLAYER)
	{
		Picking_Terrain();

		if (Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
		{
			Key_Input(_fTimeDelta);
			Mouse_Move(_fTimeDelta);
		}

		Animation_End_Check();
	}

	if (Engine::Get_ListUI(UITYPE::UI_INVENTORY)->empty())
		if (Engine::Get_ListUI(UITYPE::UI_SHOP)->empty())
			Mouse_Fix();

	Jump(_fTimeDelta);

	Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);
	Engine::Add_Collider(m_pColliderCom);

	_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
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
	//Beomseung Fix
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	_matrix mat = *m_pRight_TransformCom->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);

	m_pAnimator[RIGHT]->Render_Animator();
	m_pPlayer_Buffer->Render_Buffer();

	if (bLeftHandUse) {
		mat = *m_pLeft_TransformCom->Get_WorldMatrix();
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
		m_pAnimator[LEFT]->Render_Animator();
		m_pPlayer_Buffer->Render_Buffer();
	}
	if (bLegUse) {
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

	//Left
	pComponent = m_pLeft_TextureCom[LEFT_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LeftArmTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture2", pComponent });

	pComponent = m_pLeft_TextureCom[DRINK] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LeftDrink")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_LeftDrink", pComponent });

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
	_vec3 vLook;
	_vec3 vRight;
	_vec3 vUp;
	m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	m_pBody_TransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);
	m_pBody_TransformCom->Get_Info(INFO::INFO_UP, &vUp);

	if (Engine::Key_Hold(DIK_W)) {
		//Beomseung
		if (!bLegUse)
			m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, fSpeed);
	}
	if (Engine::Key_Hold(DIK_S)) {
		//Beomseung   
		if (!bLegUse)
			m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, -fSpeed);

	}
	if (Engine::Key_Hold(DIK_A)) {
		//Beomseung    
		if (!bLegUse)
			m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), _fTimeDelta, -fSpeed);

	}
	if (Engine::Key_Hold(DIK_D)) {
		//Beomseung    
		if (!bLegUse)
			m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), _fTimeDelta, fSpeed);
	}
	if (Engine::Key_Press(DIK_SPACE)) {
		bJumpCheck = true;
		fJumpPower = 20.0f;
	}

	if (Engine::Key_Hold(DIK_J)) {
		bLeftHandUse = false;
		m_Right_CurState = FINISHKILL;
		m_pAnimator[RIGHT]->PlayAnimation(L"FinishKill", false);

	}

	if (Engine::Key_Hold(DIK_R)) {
		switch (m_WeaponState) {
		case PISTOL:
			m_Right_CurState = RELOAD;
			m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Reload", false);
			break;
		case RIFLE:
			m_Right_CurState = RELOAD;
			m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Reload", false);
			bLeftHandUse = false;
			break;
		case SHOTGUN:
			m_Right_CurState = RELOAD;
			m_pAnimator[RIGHT]->PlayAnimation(L"Shotgun_Reload", false);
			break;
		case SNIPER:
			m_Right_CurState = RELOAD;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_Reload", false);
			bLeftHandUse = false;
			break;
		default:
			break;
		}
	}

	if (Engine::Key_Hold(DIK_1)) {
		bLeftHandUse = true;
		bLegUse = false;
		m_WeaponState = PISTOL;
		m_Right_CurState = CHANGE;
		m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Change", false);
		flinear = 0.f;
	}

	if (Engine::Key_Hold(DIK_2)) {
		m_WeaponState = RIFLE;
		bLegUse = false;
		m_Right_CurState = CHANGE;
		m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Change", false);
		bLeftHandUse = false;
		flinear = 0.f;
	}

	if (Engine::Key_Hold(DIK_3)) {
		bLeftHandUse = false;
		bLegUse = false;
		m_WeaponState = SHOTGUN;
		m_Right_CurState = CHANGE;
		m_pAnimator[RIGHT]->PlayAnimation(L"Shotgun_Change", false);
		flinear = 0.f;
	}

	if (Engine::Key_Hold(DIK_4)) {
		bLeftHandUse = true;
		bLegUse = false;
		m_WeaponState = SNIPER;
		m_Right_CurState = IDLE;
		m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_Idle", true);
		flinear = 0.f;
	}

	if (Engine::Key_Hold(DIK_5)) {
		bLeftHandUse = false;
		bLegUse = false;
		m_WeaponState = KATANA;
		m_Right_CurState = CHANGE;
		m_pAnimator[RIGHT]->PlayAnimation(L"Katana_Change", false);
		flinear = 0.f;
	}

	if (Engine::Key_Hold(DIK_6)) {
		bLegUse = true;
		bLeftHandUse = true;
		m_WeaponState = MINIGUN;
		m_Right_CurState = IDLE;
		m_Left_CurState = MINIGUN_BODY_CHANGE;
		m_Leg_CurState = MINIGUN_PANEL_CHANGE;
		m_pAnimator[RIGHT]->PlayAnimation(L"MiniGun_GunPoint_Idle", true);
		m_pAnimator[LEFT]->PlayAnimation(L"MiniGun_Body_Change", false);
		m_pAnimator[LEG]->PlayAnimation(L"MiniGun_Panel_Change", false);
		flinear = 0.f;
	}

	

	if (Engine::Key_Hold(DIK_O)) {
		//Beomseung   
		bLegUse = true;
		m_Leg_CurState = SLIDING;
		m_pAnimator[LEG]->PlayAnimation(L"Leg_Sliding", false);
	}


	// Kyubin
	if (Engine::Key_Press(DIK_X))
	{
		CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPlayerBlood", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster_Right", L"Com_Effect");
		if (pComponent)
			static_cast<CEffect*>(pComponent)->Operate_Effect();

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster_Left", L"Com_Effect");
		if (pComponent)
			static_cast<CEffect*>(pComponent)->Operate_Effect();
	}
	if (Engine::Key_Release(DIK_X))
	{
		CComponent* pComponent(nullptr);

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster_Left", L"Com_Effect");
		if (pComponent)
			static_cast<CEffect*>(pComponent)->Stop_Effect();

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster_Right", L"Com_Effect");
		if (pComponent)
			static_cast<CEffect*>(pComponent)->Stop_Effect();
	}
	if (Engine::Key_Press(DIK_Z))
	{
		CComponent* pComponent(nullptr);
		CGameObject* pGameObject(nullptr);
		//CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectMuzzleFlash", L"Com_Effect");
		//static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);

		//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBloodSplater", L"Com_Effect");
		//static_cast<CEffect*>(pComponent)->Operate_Effect();

		_vec3 vPos, vLook;
		m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &vPos);
		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectFanSpread", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Operate_Effect();

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_BloodSplater", L"Com_Transform");
		static_cast<CTransform*>(pComponent)->Set_Pos(vPos + vLook * 1.f);
		pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
		static_cast<CEffectPool*>(pGameObject)->Operate();

		//pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_Spark", L"Com_Transform");
		//static_cast<CTransform*>(pComponent)->Set_Pos(vPos + vLook * 2.f);
		//pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
		//static_cast<CEffectPool*>(pGameObject)->Operate();

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_MinigunShell", L"Com_Transform");
		pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
		static_cast<CEffectPool*>(pGameObject)->Operate();

	}
	
	if (m_WeaponState == SNIPER) {
		if (Engine::Key_Press(DIK_L))
		{
			m_Right_CurState = ZOOMIN;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_ZoomIn", false);
			bLeftHandUse = false;
		}
		if (Engine::Key_Release(DIK_L))
		{
			if (m_Right_CurState == ZOOMING || m_Right_CurState == ZOOMIN) {
				m_Right_CurState = ZOOMOUT;
				m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_ZoomOut", false);
				bLeftHandUse = false;
			}
		}
	}
}

void CPlayer::Mouse_Move(const _float& _fTimeDelta)
{
	_long dwMouseMove(0);

	if (Engine::Get_Stop())
		return;

	if (dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_Y))
	{
		m_pBody_TransformCom->Rotation(ROTATION::ROT_X, D3DXToRadian(dwMouseMove / 20.f));
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X))
	{
		m_pBody_TransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(dwMouseMove / 20.f));
	}
	if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_LB)) {

		_vec3 RayStart;
		_vec3 RayDir;
		m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &RayStart);
		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &RayDir);
		// ±Ôºó
		_vec3 vMuzzlePos{};
		m_pRight_TransformCom->Get_Info(INFO::INFO_POS, &vMuzzlePos);

		switch (m_WeaponState) {
		case PISTOL:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Shoot", false);

			// ±Ôºó
			vMuzzlePos.x -= 100.f;
			vMuzzlePos.y += 200.f;

			break;
		case RIFLE:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Shoot", false);

			// ±Ôºó
			vMuzzlePos.x += -120.f;
			vMuzzlePos.y += 100.f;
			break;
		case SHOTGUN:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Shotgun_Shoot", false);

			// ±Ôºó
			vMuzzlePos.x += -180.f;
			vMuzzlePos.y += 210.f;
			break;
		case SNIPER:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_Shoot", false);

			// ±Ôºó
			vMuzzlePos.x += -160.f;
			vMuzzlePos.y += 200.f;
			break;
		case KATANA:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Katana_Shoot", false);
			break;
		case MINIGUN:
			m_Right_CurState = SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"MiniGun_GunPoint_Shoot", false);
			Engine::Fire_Bullet(m_pGraphicDev, RayStart, RayDir, 5, CBulletManager::BULLET_PISTOL);
			break;
		}

		// ±Ôºó
		CComponent* pComponent(nullptr);
		CEffectMuzzleFlash* pMuzzleFlash(nullptr);
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectMuzzleFlash", L"Com_Effect");
		pMuzzleFlash = static_cast<CEffectMuzzleFlash*>(static_cast<CTransform*>(pComponent)->GetOwner());
		pMuzzleFlash->Set_InitPos(vMuzzlePos);
		static_cast<CEffect*>(pComponent)->Operate_Effect();

		Engine::Play_Sound(L"pew_01.wav", CHANNELID::SOUND_EFFECT, 0.1f);
		if (m_WeaponState != KATANA && m_WeaponState != MINIGUN) {
			Engine::RayCast2(RayStart, RayDir);
		}
	}

	if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB))
	{
		bLegUse = true;
		m_Leg_CurState = LEG_IDLE;
		m_pAnimator[LEG]->PlayAnimation(L"Leg_Idle", false);
		CComponent* pComponent(nullptr);
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectCircleLines", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Operate_Effect();
		m_pRight_TransformCom->Rotation(ROTATION::ROT_Z, 1.f);
		m_pRight_TransformCom->Set_Pos(WINCX / 3.f, 0.f, 2.f);
		m_pLeft_TransformCom->Rotation(ROTATION::ROT_Z, -1.f);
		m_pLeft_TransformCom->Set_Pos(WINCX / -3.f, 0.f, 2.f);
		m_fDashSpeed = fSpeed * 1.5f;
	}
	if (Engine::Mouse_Hold(MOUSEKEYSTATE::DIM_RB))
	{
		_vec3 vLook;
		if (0.f < m_fDashSpeed)
			m_fDashSpeed -= (_fTimeDelta * 10.f);
		else
			m_fDashSpeed = 0.f;

		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, m_fDashSpeed);
	}
	if (Engine::Mouse_Release(MOUSEKEYSTATE::DIM_RB))
	{
		CComponent* pComponent(nullptr);
		bLegUse = false;
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectCircleLines", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Stop_Effect();
		m_pRight_TransformCom->Rotation(ROTATION::ROT_Z, -1.f);
		m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
		m_pLeft_TransformCom->Rotation(ROTATION::ROT_Z, 1.f);
		m_pLeft_TransformCom->Set_Pos(WINCX / -3.f, WINCY / -3.f, 2.f);

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
	if (bJumpCheck) {
		fJumpPower -= 1.f;
		_vec3 vUp;
		_vec3 vPos;
		m_pBody_TransformCom->Get_Info(INFO::INFO_UP, &vUp);
		m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &vPos);
		if (vPos.y - 1.f + _fTimeDelta * fJumpPower <= fTilePos) {
			bJumpCheck = false;
			fJumpPower = 0;
			vPos.y = fTilePos;
			m_pBody_TransformCom->Set_Pos(vPos.x, vPos.y + 1.f, vPos.z);
		}
		else
			m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), _fTimeDelta, fJumpPower);

	}
}

void CPlayer::Picking_Terrain()
{
	_vec3 vPos;
	m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &vPos);
	//Beomseung
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(COMPONENTID::ID_STATIC, L"Layer_GameLogic", L"Terrain", L"Com_Buffer"));
	NULL_CHECK(pTerrainBufferCom);

	fTilePos = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	if (!bJumpCheck) {
		m_pBody_TransformCom->Set_Pos(vPos.x, fTilePos + 1.f, vPos.z);
	}
}

void CPlayer::SetAnimation()
{
	//Beomseung
	//Right
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
	m_pAnimator[RIGHT]->CreateAnimation(L"MiniGun_GunPoint_Idle", m_pRight_TextureCom[MINIGUN][IDLE], 8.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"MiniGun_GunPoint_Shoot", m_pRight_TextureCom[MINIGUN][SHOOT], 13.f);
	m_pAnimator[LEFT]->CreateAnimation(L"MiniGun_Body_Idle", m_pLeft_TextureCom[MINIGUN_BODY_IDLE], 13.f);
	m_pAnimator[LEFT]->CreateAnimation(L"MiniGun_Body_Change", m_pLeft_TextureCom[MINIGUN_BODY_CHANGE], 13.f);
	m_pAnimator[LEG]->CreateAnimation(L"MiniGun_Panel_Change", m_pLeg_TextureCom[MINIGUN_PANEL_CHANGE], 13.f);
	m_pAnimator[LEG]->CreateAnimation(L"MiniGun_Panel_Idle", m_pLeg_TextureCom[MINIGUN_PANEL_IDLE], 13.f);

	m_pAnimator[RIGHT]->CreateAnimation(L"Katana_Idle", m_pRight_TextureCom[KATANA][IDLE], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Katana_Shoot", m_pRight_TextureCom[KATANA][SHOOT], 18.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Katana_Change", m_pRight_TextureCom[KATANA][CHANGE], 18.f);

	//Left
	m_pAnimator[LEFT]->CreateAnimation(L"Left_Idle", m_pLeft_TextureCom[LEFT_IDLE], 8.f);
	m_pAnimator[LEFT]->CreateAnimation(L"Left_Drink", m_pLeft_TextureCom[DRINK], 8.f);

	//Leg
	m_pAnimator[LEG]->CreateAnimation(L"Leg_Idle", m_pLeg_TextureCom[LEG_IDLE], 8.f);
	m_pAnimator[LEG]->CreateAnimation(L"Leg_Kick", m_pLeg_TextureCom[KICK], 18.f);
	m_pAnimator[LEG]->CreateAnimation(L"Leg_Sliding", m_pLeg_TextureCom[SLIDING], 18.f);

	m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Change", false);
	m_pRight_TransformCom->Set_Scale(600.f, 600.f, 0.f);
	m_pRight_TransformCom->Set_Pos(vDefaultPos[RIGHT]);

	m_pAnimator[LEFT]->PlayAnimation(L"Left_Idle", true);
	m_pLeft_TransformCom->Set_Scale(vDefaultSize[LEFT]);
	m_pLeft_TransformCom->Set_Pos(vDefaultPos[LEFT]);

	m_pAnimator[LEG]->PlayAnimation(L"Leg_Idle", false);
	m_pLeg_TransformCom->Set_Scale(vDefaultSize[LEG]);
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
		else {
			m_Leg_CurState = LEG_IDLE;
			//bLegUse = false;
		}
	}

	if (m_pAnimator[RIGHT]->GetCurrAnim()->GetFinish())
	{
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
		bLeftHandUse = true;
		flinear = 0;
	}
	if (m_pAnimator[LEFT]->GetCurrAnim()->GetFinish())
	{
		if (m_WeaponState == MINIGUN) {
			m_Left_CurState = MINIGUN_BODY_IDLE;
			m_pAnimator[LEFT]->PlayAnimation(L"MiniGun_Body_Idle", false);
		}
		else {
			m_Left_CurState = LEFT_IDLE;
			m_pAnimator[LEFT]->PlayAnimation(L"Left_Idle", true);
		}
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
	case MINIGUN_PANEL_IDLE:
		m_pLeg_TransformCom->Set_Scale(vDefaultSize[LEG] * 1.2f);
		break;
	default:
		m_pLeg_TransformCom->Set_Pos(0, WINCY / -3.f, 2.f);
		break;
	}
	switch (m_WeaponState)
	{
	case PISTOL:
		m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT]);
		m_pRight_TransformCom->Set_Pos(vDefaultPos[RIGHT]);
		break;
	case RIFLE:
		switch (m_Right_CurState)
		{
		case IDLE:
		case SHOOT:
			m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT] / 2.f);
			m_pRight_TransformCom->Set_Pos(vDefaultPos[RIGHT]);
			break;
		case RELOAD:
		case CHANGE:
			m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT]);
			m_pRight_TransformCom->Set_Pos(0.f, -WINCY / 3.f, 2.f);
			break;
		}
		break;
	case SHOTGUN:
		switch (m_Right_CurState)
		{
		case IDLE:
		case SHOOT:
		case RELOAD:
			m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT]);
			m_pRight_TransformCom->Set_Pos(vDefaultPos[RIGHT]);
			break;
		case CHANGE:
			m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT] * 1.2f);
			if (m_pAnimator[RIGHT]->GetCurrAnim()->GetCurrFrame() >= 10)
			{
				_vec3 vRight;
				m_pRight_TransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);
				m_pRight_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), 1, 10.f);
			}
			else {
				m_pRight_TransformCom->Set_Pos(0.f, WINCY / -4.f, 2.f);
			}
		}
		break;
	case SNIPER:
		switch (m_Right_CurState)
		{
		case IDLE:
		case SHOOT:
		case ZOOMOUT:
			m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT]);
			m_pRight_TransformCom->Set_Pos(vDefaultPos[RIGHT]);
			m_pCComponentCamera->SetFov(D3DXToRadian(60.f));
			break;
		case RELOAD:
			m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT]);
			m_pRight_TransformCom->Set_Pos(0.f, WINCY / -3.f, 2.f);
			break;
		case ZOOMIN:
			m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT]);
			m_pRight_TransformCom->Set_Pos(vDefaultPos[RIGHT]);
			break;
		case ZOOMING:
			bLeftHandUse = false;
			m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT] * 2);
			m_pRight_TransformCom->Set_Pos(0.f, 0.f, 2.f);
			m_pCComponentCamera->SetFov(D3DXToRadian(10.f));
			break;
		}
		break;
	case KATANA:
		switch (m_Right_CurState)
		{
		case IDLE:
			m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT] * 1.1f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -4.f, 2.f);
			break;
		case SHOOT:
			m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT] * 1.4f);
			m_pRight_TransformCom->Set_Pos(0.f, 0.f, 2.f);
			break;
		case CHANGE:
			m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT] * 1.5f);
			m_pRight_TransformCom->Set_Pos(0.f, WINCY / -3.f, 2.f);
			break;
		}
		break;
	case MINIGUN:
		if (flinear >= 1.f) {
			flinear = 1.f;
		}
		flinear += 0.1f;
		vPos;
		vStart = { 0.f, WINCY / -4.f , 2.f };
		vEnd = { 0.f,-100.f,2.f };
		D3DXVec3Lerp(&vPos, &vStart, &vEnd, flinear);
		m_pRight_TransformCom->Set_Pos(vPos);
		m_pRight_TransformCom->Set_Scale(vDefaultSize[RIGHT] * 0.5f);
		break;
	}
	switch (m_Left_CurState)
	{
	case LEFT_IDLE:
	case DRINK:
		m_pLeft_TransformCom->Set_Scale(vDefaultSize[LEFT]);
		m_pLeft_TransformCom->Set_Pos(vDefaultPos[LEFT]);
		break;
	case MINIGUN_BODY_IDLE:
	case MINIGUN_BODY_CHANGE:
		if (flinear >= 1.f) {
			flinear = 1.f;
		}
		vStart = { 0.f, WINCY / -4.f , 2.f };
		vEnd = { 0.f,-150.f,2.f };
		D3DXVec3Lerp(&vPos, &vStart, &vEnd, flinear);
		m_pLeft_TransformCom->Set_Pos(vPos);
		m_pLeft_TransformCom->Set_Scale(vDefaultSize[LEFT] * 0.5f);
		break;
	}
}

void CPlayer::OnCollision(CCollider& _pOther)
{
	CMonster* pMonster = dynamic_cast<CMonster*>(_pOther.GetOwner());

	if (m_Leg_CurState == KICK && m_pAnimator[LEG]->GetCurrAnim()->GetCurrFrame() <= 1.f)
	{
		_vec3 vLook;
		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
		if (pMonster == nullptr) {
			return;
		}
		pMonster->AddForce(30.f, vLook);
	}
	if (m_WeaponState == KATANA && m_Right_CurState == SHOOT && m_pAnimator[RIGHT]->GetCurrAnim()->GetCurrFrame() <= 1.f)
	{
		if (pMonster == nullptr) {
			return;
		}
		pMonster->Damaged_By_Player(DAMAGED_STATE::DAMAGED_KATANA);

	}

}

void CPlayer::OnCollisionEnter(CCollider& _pOther)
{
	CMonster* pMonster = static_cast<CMonster*>(_pOther.GetOwner());
	if (nullptr != pMonster) //is it Monster == _pOther
	{
		if (bLegUse)
		{
			m_Leg_CurState = KICK;
			m_pAnimator[LEG]->PlayAnimation(L"Leg_Kick", false);
		}
		else
		{
			CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPlayerBlood", L"Com_Effect");
			static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);
		}
		return;
	}

	CBullet* pBullet = static_cast<CBullet*>(_pOther.GetOwner()); //is it Monster_Bullet == _pOther

	if (nullptr != pBullet)
	{
		CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPlayerBlood", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);
		return;
	}

	CItem* pItem = static_cast<CItem*>(_pOther.GetOwner()); //here you should check fcking Item
	if (nullptr != pItem)
		return;
	else
	{
		if (bLegUse)
		{
			m_Leg_CurState = KICK;
			m_pAnimator[LEG]->PlayAnimation(L"Leg_Kick", false);
		}
	}
	
}

void CPlayer::OnCollisionExit(CCollider& _pOther)
{

}

_vec3 CPlayer::Picking_OnTerrain()
{
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(COMPONENTID::ID_STATIC, L"Layer_GameLogic", L"Terrain", L"Com_Buffer"));
	NULL_CHECK_RETURN(pTerrainBufferCom, _vec3());

	CTransform* pTerrainTransCom = dynamic_cast<CTransform*>(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_GameLogic", L"Terrain", L"Com_Transform"));
	NULL_CHECK_RETURN(pTerrainTransCom, _vec3());

	return m_pCalculatorCom->Picking_OnTerrain(g_hWnd, pTerrainBufferCom, pTerrainTransCom);
}

void CPlayer::Free()
{
	Engine::CGameObject::Free();
}