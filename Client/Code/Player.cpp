#include "pch.h"
#include "..\Header\Player.h"
#include "Export_System.h"
#include "Export_Utility.h"
// ±Ôºó
#include "../Header/EffectPool.h"
#include "../Header/EffectMuzzleFlash.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CCharacter(_pGraphicDev)
	, m_pRight_BufferCom(nullptr)
	, m_pRight_TransformCom(nullptr)
	, m_pBody_TransformCom(nullptr)
	, m_pCComponentCamera(nullptr)
	, m_pLeft_BufferCom(nullptr)
	, m_pLeft_TransformCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pColliderCom(nullptr)
	, bJumpCheck(false)
	, bLeftHandUse(true)
	, bLegUse(false)
	, fJumpPower(0.f)
	, fTilePos(0.f)
	, fSpeed(0.f)
	, m_Right_CurState(PISTOL_CHANGE)
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
		Key_Input(_fTimeDelta);
		Mouse_Move();
		Animation_End_Check();
	}

	if (!Engine::Get_UILayerRender(UITYPE::UI_UPGRADE))
		Mouse_Fix();

	Jump(_fTimeDelta);

	Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);
	Engine::Add_Collider(m_pColliderCom);

	_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);
	Animation_Pos();
	m_pLeg_TransformCom->Update_Component(_fTimeDelta);
	m_pRight_TransformCom->Update_Component(_fTimeDelta);
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

	if (bLegUse) {
		_matrix mat = *m_pLeg_TransformCom->Get_WorldMatrix();
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
		m_pAnimator[LEG]->Render_Animator();
		m_pLeg_BufferCom->Render_Buffer();
	}

	_matrix mat = *m_pRight_TransformCom->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);

	m_pAnimator[RIGHT]->Render_Animator();
	m_pRight_BufferCom->Render_Buffer();

	if (bLeftHandUse) {
		mat = *m_pLeft_TransformCom->Get_WorldMatrix();
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
		m_pAnimator[LEFT]->Render_Animator();
		m_pLeft_BufferCom->Render_Buffer();
	}
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component()
{
	CComponent* pComponent = NULL;

	//Beomseung
	//Buffer

	pComponent = m_pRight_BufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RightArmBuffer"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pLeft_BufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_LeftArmBuffer"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer2", pComponent });

	pComponent = m_pLeg_BufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_Leg_Buffer"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer3", pComponent });

	//Texture

	//Right
	//Pistol
	pComponent = m_pRight_TextureCom[PISTOL_IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Pistol_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Pistol_IDLE", pComponent });

	pComponent = m_pRight_TextureCom[PISTOL_SHOOT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Pistol_Shoot")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Pistol_Shoot", pComponent });

	pComponent = m_pRight_TextureCom[PISTOL_RELOAD] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Pistol_Reload")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Pistol_Reload", pComponent });

	pComponent = m_pRight_TextureCom[PISTOL_CHANGE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Pistol_Change")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Pistol_Change", pComponent });

	//Rifle
	pComponent = m_pRight_TextureCom[RIFLE_IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Rifle_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Rifle_IDLE", pComponent });

	pComponent = m_pRight_TextureCom[RIFLE_SHOOT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Rifle_Shoot")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Rifle_Shoot", pComponent });

	pComponent = m_pRight_TextureCom[RIFLE_RELOAD] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Rifle_Reload")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Rifle_Reload", pComponent });

	//Shotgun
	pComponent = m_pRight_TextureCom[SHOTGUN_IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Shotgun_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Shotgun_IDLE", pComponent });

	pComponent = m_pRight_TextureCom[SHOTGUN_SHOOT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Shotgun_Shoot")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Shotgun_Shoot", pComponent });

	pComponent = m_pRight_TextureCom[SHOTGUN_RELOAD] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Shotgun_Reload")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Shotgun_Reload", pComponent });

	//Sniper
	pComponent = m_pRight_TextureCom[SNIPER_IDLE] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_IDLE")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_IDLE", pComponent });

	pComponent = m_pRight_TextureCom[SNIPER_SHOOT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_Shoot")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_Shoot", pComponent });

	pComponent = m_pRight_TextureCom[SNIPER_RELOAD] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_Reload")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_Reload", pComponent });

	pComponent = m_pRight_TextureCom[SNIPER_ZOOMIN] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_ZoomIn")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_ZoomIn", pComponent });

	pComponent = m_pRight_TextureCom[SNIPER_ZOOMING] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_ZoomIng")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_ZoomIng", pComponent });

	pComponent = m_pRight_TextureCom[SNIPER_ZOOMOUT] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Sniper_ZoomOut")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Sniper_ZoomOut", pComponent });

	//Left
	pComponent = m_pLeft_TextureCom[LEFT_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LeftArmTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture2", pComponent });

	pComponent = m_pLeft_TextureCom[DRINK] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LeftDrink")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_LeftDrink", pComponent });

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
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, fSpeed);
	}
	if (Engine::Key_Hold(DIK_S)) {
		//Beomseung   
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, -fSpeed);

	}
	if (Engine::Key_Hold(DIK_A)) {
		//Beomseung    
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), _fTimeDelta, -fSpeed);

	}
	if (Engine::Key_Hold(DIK_D)) {
		//Beomseung    
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), _fTimeDelta, fSpeed);
	}
	if (Engine::Key_Press(DIK_SPACE)) {
		bJumpCheck = true;
		fJumpPower = 20.0f;
	}

	if (Engine::Key_Hold(DIK_R)) {
		switch (m_WeaponState) {
		case PISTOL:
			m_Right_CurState = PISTOL_RELOAD;
			m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Reload", false);
			break;
		case RIFLE:
			m_Right_CurState = RIFLE_RELOAD;
			m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Reload", false);
			bLeftHandUse = false;
			break;
		case SHOTGUN:
			m_Right_CurState = SHOTGUN_RELOAD;
			m_pAnimator[RIGHT]->PlayAnimation(L"Shotgun_Reload", false);
			break;
		case SNIPER:
			m_Right_CurState = SNIPER_RELOAD;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_Reload", false);
			bLeftHandUse = false;
			break;
		}
	}

	if (Engine::Key_Hold(DIK_1)) {
		m_WeaponState = PISTOL;
		m_Right_CurState = PISTOL_CHANGE;
		m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Change", false);
	}

	if (Engine::Key_Hold(DIK_2)) {
		m_WeaponState = RIFLE;
		m_Right_CurState = RIFLE_IDLE;
		m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Idle", true);
	}

	if (Engine::Key_Hold(DIK_3)) {
		m_WeaponState = SHOTGUN;
		m_Right_CurState = SHOTGUN_IDLE;
		m_pAnimator[RIGHT]->PlayAnimation(L"Shotgun_Idle", true);
	}

	if (Engine::Key_Hold(DIK_4)) {
		m_WeaponState = SNIPER;
		m_Right_CurState = SNIPER_IDLE;
		m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_Idle", true);
	}

	if (Engine::Key_Hold(DIK_P)) {
		//Beomseung   
		bLegUse = true;
		m_Leg_CurState = KICK;
		m_pAnimator[LEG]->PlayAnimation(L"Leg_Kick", false);
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

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Operate_Effect();
	}
	if (Engine::Key_Release(DIK_X))
	{
		CComponent* pComponent(nullptr);
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBossRobotBooster", L"Com_Effect");
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

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPool_Spark", L"Com_Transform");
		static_cast<CTransform*>(pComponent)->Set_Pos(vPos + vLook * 2.f);
		pGameObject = static_cast<CTransform*>(pComponent)->GetOwner();
		static_cast<CEffectPool*>(pGameObject)->Operate();
	}
	if (Engine::Key_Press(DIK_LSHIFT))
	{
		CComponent* pComponent(nullptr);
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectCircleLines", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Operate_Effect();
		m_pRight_TransformCom->Rotation(ROTATION::ROT_Z, 1.f);
		m_pRight_TransformCom->Set_Pos(WINCX / 3.f, 0.f, 2.f);
		m_pLeft_TransformCom->Rotation(ROTATION::ROT_Z, -1.f);
		m_pLeft_TransformCom->Set_Pos(WINCX / -3.f, 0.f, 2.f);
		fSpeed += 100;

	}
	if (Engine::Key_Release(DIK_LSHIFT))
	{
		CComponent* pComponent(nullptr);
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectCircleLines", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Stop_Effect();
		m_pRight_TransformCom->Rotation(ROTATION::ROT_Z, -1.f);
		m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
		m_pLeft_TransformCom->Rotation(ROTATION::ROT_Z, 1.f);
		m_pLeft_TransformCom->Set_Pos(WINCX / -3.f, WINCY / -3.f, 2.f);
		fSpeed -= 100;
	}
	if (m_WeaponState == SNIPER) {
		if (Engine::Key_Press(DIK_L))
		{
			m_Right_CurState = SNIPER_ZOOMIN;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_ZoomIn", false);
			bLeftHandUse = false;
		}
		if (Engine::Key_Release(DIK_L))
		{
			m_Right_CurState = SNIPER_ZOOMOUT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_ZoomOut", false);
			bLeftHandUse = false;
		}
	}
}

void CPlayer::Mouse_Move()
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

		// ±Ôºó
		_vec3 vMuzzlePos{};
		m_pRight_TransformCom->Get_Info(INFO::INFO_POS, &vMuzzlePos);

		switch (m_WeaponState) {
		case PISTOL:
			m_Right_CurState = PISTOL_SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Shoot", false);

			// ±Ôºó
			vMuzzlePos.x -= 100.f;
			vMuzzlePos.y += 200.f;

			break;
		case RIFLE:
			m_Right_CurState = RIFLE_SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Shoot", false);

			// ±Ôºó
			vMuzzlePos.x += -120.f;
			vMuzzlePos.y += 100.f;
			break;
		case SHOTGUN:
			m_Right_CurState = SHOTGUN_SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Shotgun_Shoot", false);

			// ±Ôºó
			vMuzzlePos.x += -180.f;
			vMuzzlePos.y += 210.f;
			break;
		case SNIPER:
			m_Right_CurState = SNIPER_SHOOT;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_Shoot", false);

			// ±Ôºó
			vMuzzlePos.x += -160.f;
			vMuzzlePos.y += 200.f;
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
		_vec3 RayStart;
		_vec3 RayDir;
		m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &RayStart);
		m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &RayDir);
		Engine::RayCast2(RayStart, RayDir);
	}

	if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB)) {
		m_Left_CurState = DRINK;
		m_pAnimator[LEFT]->PlayAnimation(L"Left_Drink", false);

		// ±Ôºó
		CComponent* pComponent(nullptr);
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectHeal", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Operate_Effect();
	}

	if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB)) {
		m_Left_CurState = DRINK;
		m_pAnimator[LEFT]->PlayAnimation(L"Left_Drink", false);
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
	m_pAnimator[RIGHT]->CreateAnimation(L"Pistol_Idle", m_pRight_TextureCom[PISTOL_IDLE], 8.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Pistol_Shoot", m_pRight_TextureCom[PISTOL_SHOOT], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Pistol_Reload", m_pRight_TextureCom[PISTOL_RELOAD], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Pistol_Change", m_pRight_TextureCom[PISTOL_CHANGE], 13.f);

	m_pAnimator[RIGHT]->CreateAnimation(L"Rifle_Idle", m_pRight_TextureCom[RIFLE_IDLE], 8.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Rifle_Shoot", m_pRight_TextureCom[RIFLE_SHOOT], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Rifle_Reload", m_pRight_TextureCom[RIFLE_RELOAD], 10.f);

	m_pAnimator[RIGHT]->CreateAnimation(L"Shotgun_Idle", m_pRight_TextureCom[SHOTGUN_IDLE], 8.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Shotgun_Shoot", m_pRight_TextureCom[SHOTGUN_SHOOT], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Shotgun_Reload", m_pRight_TextureCom[SHOTGUN_RELOAD], 13.f);

	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_Idle", m_pRight_TextureCom[SNIPER_IDLE], 8.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_Shoot", m_pRight_TextureCom[SNIPER_SHOOT], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_Reload", m_pRight_TextureCom[SNIPER_RELOAD], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_ZoomIn", m_pRight_TextureCom[SNIPER_ZOOMIN], 13.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_ZoomIng", m_pRight_TextureCom[SNIPER_ZOOMING], 5.f);
	m_pAnimator[RIGHT]->CreateAnimation(L"Sniper_ZoomOut", m_pRight_TextureCom[SNIPER_ZOOMOUT], 13.f);

	//Left
	m_pAnimator[LEFT]->CreateAnimation(L"Left_Idle", m_pLeft_TextureCom[LEFT_IDLE], 8.f);
	m_pAnimator[LEFT]->CreateAnimation(L"Left_Drink", m_pLeft_TextureCom[DRINK], 8.f);

	//Leg
	m_pAnimator[LEG]->CreateAnimation(L"Leg_Idle", m_pLeg_TextureCom[LEG_IDLE], 8.f);
	m_pAnimator[LEG]->CreateAnimation(L"Leg_Kick", m_pLeg_TextureCom[KICK], 13.f);
	m_pAnimator[LEG]->CreateAnimation(L"Leg_Sliding", m_pLeg_TextureCom[SLIDING], 13.f);

	m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Change", false);
	m_pRight_TransformCom->Set_Scale(600.f, 600.f, 0.f);
	m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);

	m_pAnimator[LEFT]->PlayAnimation(L"Left_Idle", true);
	m_pLeft_TransformCom->Set_Scale(300.f, 300.f, 0.f);
	m_pLeft_TransformCom->Set_Pos(WINCX / -3.f, WINCY / -3.f, 2.f);

	m_pAnimator[LEG]->PlayAnimation(L"Leg_Idle", false);
	m_pLeg_TransformCom->Set_Scale(500.f, 500.f, 0.f);
	m_pLeg_TransformCom->Set_Pos(0, WINCY / -3.f, 2.f);
}

void CPlayer::Animation_End_Check()
{
	if (m_pAnimator[LEG]->GetCurrAnim()->GetFinish())
	{
		m_Leg_CurState = LEG_IDLE;
		bLegUse = false;
	}

	if (m_pAnimator[RIGHT]->GetCurrAnim()->GetFinish())
	{
		switch (m_WeaponState) {
		case PISTOL:
			m_Right_CurState = PISTOL_IDLE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Pistol_Idle", true);
			bLeftHandUse = true;
			break;
		case RIFLE:
			m_Right_CurState = RIFLE_IDLE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Rifle_Idle", true);
			bLeftHandUse = true;
			break;
		case SHOTGUN:
			m_Right_CurState = SHOTGUN_IDLE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Shotgun_Idle", true);
			bLeftHandUse = true;
			break;
		case SNIPER:
			if (m_Right_CurState == SNIPER_ZOOMIN)
			{
				m_Right_CurState = SNIPER_ZOOMING;
				m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_ZoomIng", true);
				break;
			}
			m_Right_CurState = SNIPER_IDLE;
			m_pAnimator[RIGHT]->PlayAnimation(L"Sniper_Idle", true);
			bLeftHandUse = true;
			break;
		}
	}
	if (m_pAnimator[LEFT]->GetCurrAnim()->GetFinish())
	{
		m_Left_CurState = LEFT_IDLE;
		m_pAnimator[LEFT]->PlayAnimation(L"Left_Idle", true);
	}
}

void CPlayer::Animation_Pos()
{
	if ((m_Leg_CurState == KICK) && m_pAnimator[LEG]->GetCurrAnim()->GetCurrFrame() >= 5)
	{
		m_pLeg_TransformCom->Set_Pos(0, -400, 2.f);
	}
	else {
		m_pLeg_TransformCom->Set_Pos(0, WINCY / -3.f, 2.f);
	}

	if (m_WeaponState == RIFLE) {
		m_pRight_TransformCom->Set_Scale(250.f, 250.f, 0.f);
		if (m_Right_CurState == RIFLE_RELOAD) {
			m_pRight_TransformCom->Set_Scale(500.f, 500.f, 0.f);
			m_pRight_TransformCom->Set_Pos(0.f, -WINCY / 3.f, 2.f);
		}
		else {
			m_pRight_TransformCom->Set_Scale(250.f, 250.f, 0.f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
		}
	}
	else if (m_Right_CurState == SNIPER_RELOAD)
	{
		m_pRight_TransformCom->Set_Scale(500.f, 500.f, 0.f);
		m_pRight_TransformCom->Set_Pos(0.f, WINCY / -4.f, 2.f);
	}

	else if (m_Right_CurState == SNIPER_ZOOMIN)
	{
		m_pRight_TransformCom->Set_Scale(500.f, 500.f, 0.f);
		m_pRight_TransformCom->Set_Pos(0.f, WINCY / -4.f, 2.f);
	}
	else if (m_Right_CurState == SNIPER_ZOOMING)
	{
		m_pRight_TransformCom->Set_Scale(1000.f, 1000.f, 0.f);
		m_pRight_TransformCom->Set_Pos(0.f, 0.f, 2.f);
	}
	else {
		m_pRight_TransformCom->Set_Scale(500.f, 500.f, 0.f);
		m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
	}

}

void CPlayer::OnCollision(CCollider& _pOther)
{


}

void CPlayer::OnCollisionEnter(CCollider& _pOther)
{
	CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPlayerBlood", L"Com_Effect");
	static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);

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