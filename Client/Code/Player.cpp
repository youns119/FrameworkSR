#include "pch.h"
#include "..\Header\Player.h"
#include "Export_System.h"
#include "Export_Utility.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CGameObject(_pGraphicDev)
	, m_pRight_BufferCom(nullptr)
	, m_pRight_TransformCom(nullptr)
	, m_pBody_TransformCom(nullptr)
	, m_pCComponentCamera(nullptr)
	, m_pLeft_BufferCom(nullptr)
	, m_pLeft_TransformCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, bJumpCheck(false)
	, bLegUse(false)
	, fJumpPower(0.f)
	, fTilePos(0.f)
	, m_Right_CurState(PISTOL_IDLE)
	, m_Right_PreState(RIGHT_STATE_END)
	, m_Left_CurState(LEFT_IDLE)
	, m_Left_PreState(LEFT_STATE_END)
	, m_Leg_CurState(LEG_IDLE)
	, m_Leg_PreState(LEG_STATE_END)
	, m_WeaponState(PISTOL)
	//Beomseung
{
	ZeroMemory(&m_fFrameStart, sizeof(m_fFrameStart));
	ZeroMemory(&m_fFrameEnd, sizeof(m_fFrameEnd));
	ZeroMemory(&m_fFrameSpeed, sizeof(m_fFrameSpeed));
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

	//m_pLeft_TransformCom->Set_Scale(1, 1, 1);
	//m_pRight_TransformCom->Set_Scale(1, 1, 1);

	m_pRight_TransformCom->Set_Pos(2.f, 1.f, 2.f);
	m_pLeft_TransformCom->Set_Pos(-2.f, 1.f, 2.f);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& _fTimeDelta)
{
	Move_Frame(_fTimeDelta);
	Update_Pos();
	Motion_Change();
	Picking_Terrain();
	Key_Input(_fTimeDelta);
	Jump(_fTimeDelta);
	Mouse_Move();
	//Mouse_Fix();

	Add_RenderGroup(RENDERID::RENDER_ORTHOGONAL, this);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);

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
		m_pLeg_TextureCom[m_Leg_CurState]->Set_Texture(m_fFrameStart[LEG]);
		m_pLeg_BufferCom->Render_Buffer();
	}

	_matrix mat = *m_pRight_TransformCom->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);

	m_pRight_TextureCom[m_Right_CurState]->Set_Texture(m_fFrameStart[RIGHT]);
	m_pRight_BufferCom->Render_Buffer();

	mat = *m_pLeft_TransformCom->Get_WorldMatrix();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pLeft_TextureCom[m_Left_CurState]->Set_Texture(m_fFrameStart[LEFT]);
	m_pLeft_BufferCom->Render_Buffer();

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
	//Left
	pComponent = m_pLeft_TextureCom[LEFT_IDLE] = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LeftArmTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture2", pComponent });

	pComponent = m_pLeft_TextureCom[DRINK] = (dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LeftDrink")));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_LeftDrink", pComponent });

	//Leg
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

	//Camera
	pComponent = m_pCComponentCamera = dynamic_cast<CComponentCamera*>(Engine::Clone_Proto(L"Proto_ComponentCamera"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_ComponentCamera", pComponent });
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
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, 10.f);
	}
	if (Engine::Key_Hold(DIK_S)) {
		//Beomseung   
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, -10.f);

	}
	if (Engine::Key_Hold(DIK_A)) {
		//Beomseung    
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), _fTimeDelta, -10.f);

	}
	if (Engine::Key_Hold(DIK_D)) {
		//Beomseung    
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), _fTimeDelta, 10.f);
	}
	if (Engine::Key_Press(DIK_SPACE)) {
		bJumpCheck = true;
		fJumpPower = 20.0f;
	}

	if (Engine::Key_Hold(DIK_R)) {
		switch (m_WeaponState) {
		case PISTOL:
			m_Right_CurState = PISTOL_RELOAD;
			break;
		case RIFLE:
			m_Right_CurState = RIFLE_RELOAD;
			break;
		case SHOTGUN:
			m_Right_CurState = SHOTGUN_RELOAD;
			break;
		}
		m_fFrameStart[RIGHT] = 0;
	}

	if (Engine::Key_Hold(DIK_1)) {
		m_WeaponState = PISTOL;
		m_Right_CurState = PISTOL_IDLE;
		m_fFrameStart[RIGHT] = 0;
	}

	if (Engine::Key_Hold(DIK_2)) {
		m_WeaponState = RIFLE;
		m_Right_CurState = RIFLE_IDLE;
		m_fFrameStart[RIGHT] = 0;
	}

	if (Engine::Key_Hold(DIK_3)) {
		m_WeaponState = SHOTGUN;
		m_Right_CurState = SHOTGUN_IDLE;
		m_fFrameStart[RIGHT] = 0;
	}


	if (Engine::Key_Hold(DIK_P)) {
		//Beomseung   
		bLegUse = true;
		m_Leg_CurState = KICK;
		m_fFrameStart[LEG] = 0;
	}

	if (Engine::Key_Hold(DIK_O)) {
		//Beomseung   
		bLegUse = true;
		m_Leg_CurState = SLIDING;
		m_fFrameStart[LEG] = 0;
	}


	// Kyubin
	if (Engine::Key_Press(DIK_X))
	{
		CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectPlayerBlood", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);
	}
	if (Engine::Key_Press(DIK_Z))
	{
		CComponent* pComponent(nullptr);
		//CComponent* pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectMuzzleFlash", L"Com_Effect");
		//static_cast<CEffect*>(pComponent)->Set_Visibility(TRUE);
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectBloodSplater", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Operate_Effect();

		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectFanSpread", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Operate_Effect();
	}
	if (Engine::Key_Press(DIK_LSHIFT))
	{
		CComponent* pComponent(nullptr);
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectCircleLines", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Operate_Effect();

	}
	if (Engine::Key_Press(DIK_C))
	{
		// HOW TO KNOW WHEN KEY UP? I DO NOT KNOW 
		// MY PLAN WAS TO MAKE THIS CODE WORK WHEN IT WAS SHIFT KEY-UP
		CComponent* pComponent(nullptr);
		pComponent = Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_Effect", L"EffectCircleLines", L"Com_Effect");
		static_cast<CEffect*>(pComponent)->Stop_Effect();
	}

	// ¿¬¿í
	if (Engine::Key_Press(DIK_T))
	{
		Engine::CTimerManager::GetInstance()->OnOff_Timer();
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
		switch (m_WeaponState) {
		case PISTOL:
			m_Right_CurState = PISTOL_SHOOT;
			break;
		case RIFLE:
			m_Right_CurState = RIFLE_SHOOT;
			break;
		case SHOTGUN:
			m_Right_CurState = SHOTGUN_SHOOT;
			break;
		}
		m_fFrameStart[RIGHT] = 0;
	}

	if (Engine::Mouse_Press(MOUSEKEYSTATE::DIM_RB)) {
		m_Left_CurState = DRINK;
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

void CPlayer::Motion_Change()
{
	if (m_Right_PreState != m_Right_CurState)
	{
		switch (m_Right_CurState)
		{
		case PISTOL_IDLE:
			m_fFrameStart[RIGHT] = 0;
			m_fFrameEnd[RIGHT] = 4;
			m_fFrameSpeed[RIGHT] = 8.f;
			m_pRight_TransformCom->Set_Scale(500, 500, 0.f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
			break;
		case PISTOL_SHOOT:
			m_fFrameStart[RIGHT] = 0;
			m_fFrameEnd[RIGHT] = 6;
			m_fFrameSpeed[RIGHT] = 13.f;
			m_pRight_TransformCom->Set_Scale(500, 500, 0.f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
			break;
		case PISTOL_RELOAD:
			m_fFrameStart[RIGHT] = 0;
			m_fFrameEnd[RIGHT] = 16;
			m_fFrameSpeed[RIGHT] = 13.f;
			m_pRight_TransformCom->Set_Scale(500, 500, 0.f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
			break;
		case RIFLE_IDLE:
			m_fFrameStart[RIGHT] = 0;
			m_fFrameEnd[RIGHT] = 4;
			m_fFrameSpeed[RIGHT] = 8.f;
			m_pRight_TransformCom->Set_Scale(500, 500, 0.f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
			break;
		case RIFLE_SHOOT:
			m_fFrameStart[RIGHT] = 0;
			m_fFrameEnd[RIGHT] = 9;
			m_fFrameSpeed[RIGHT] = 13.f;
			m_pRight_TransformCom->Set_Scale(500, 500, 0.f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
			break;
		case RIFLE_RELOAD:
			m_fFrameStart[RIGHT] = 0;
			m_fFrameEnd[RIGHT] = 9;
			m_fFrameSpeed[RIGHT] = 13.f;
			m_pRight_TransformCom->Set_Scale(500, 500, 0.f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
			break;
		case SHOTGUN_IDLE:
			m_fFrameStart[RIGHT] = 0;
			m_fFrameEnd[RIGHT] = 4;
			m_fFrameSpeed[RIGHT] = 8.f;
			m_pRight_TransformCom->Set_Scale(500, 500, 0.f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
			break;
		case SHOTGUN_SHOOT:
			m_fFrameStart[RIGHT] = 0;
			m_fFrameEnd[RIGHT] = 14;
			m_fFrameSpeed[RIGHT] = 13.f;
			m_pRight_TransformCom->Set_Scale(500, 500, 0.f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
			break;
		case SHOTGUN_RELOAD:
			m_fFrameStart[RIGHT] = 0;
			m_fFrameEnd[RIGHT] = 10;
			m_fFrameSpeed[RIGHT] = 13.f;
			m_pRight_TransformCom->Set_Scale(500, 500, 0.f);
			m_pRight_TransformCom->Set_Pos(WINCX / 3.f, WINCY / -3.f, 2.f);
			break;
		}
		m_Right_PreState = m_Right_CurState;
	}

	if (m_Left_PreState != m_Left_CurState)
	{
		switch (m_Left_CurState)
		{
		case LEFT_IDLE:
			m_fFrameStart[LEFT] = 0;
			m_fFrameEnd[LEFT] = 16;
			m_fFrameSpeed[LEFT] = 8.f;
			m_pLeft_TransformCom->Set_Scale(300, 300, 0.f);
			m_pLeft_TransformCom->Set_Pos(WINCX / -3.f, -WINCY / 3.f, 2.f);
			break;
		case DRINK:
			m_fFrameStart[LEFT] = 0;
			m_fFrameEnd[LEFT] = 6;
			m_fFrameSpeed[LEFT] = 8.f;
			m_pLeft_TransformCom->Set_Scale(600, 600, 0.f);
			m_pLeft_TransformCom->Set_Pos(0.f, 0.f, 2.f);
			break;

		}
		m_Left_PreState = m_Left_CurState;

	}

	if (m_Leg_PreState != m_Leg_CurState)
	{
		switch (m_Leg_CurState)
		{
		case KICK:
			m_fFrameStart[LEG] = 0;
			m_fFrameEnd[LEG] = 9;
			m_fFrameSpeed[LEG] = 8.f;
			m_pLeg_TransformCom->Set_Scale(500, 500, 0.f);
			m_pLeg_TransformCom->Set_Pos(0.f, -WINCY / 3.f, 2.f);
			break;
		case SLIDING:
			m_fFrameStart[LEG] = 0;
			m_fFrameEnd[LEG] = 4;
			m_fFrameSpeed[LEG] = 8.f;
			m_pLeg_TransformCom->Set_Scale(500, 500, 0.f);
			m_pLeg_TransformCom->Set_Pos(0.f, -WINCY / 3.f, 2.f);
			break;
		}
		m_Leg_PreState = m_Leg_CurState;

	}
}

void CPlayer::Move_Frame(const _float& _fTimeDelta)
{
	m_fFrameStart[RIGHT] += m_fFrameSpeed[RIGHT] * _fTimeDelta;
	if (m_fFrameStart[RIGHT] > m_fFrameEnd[RIGHT]) {
		switch (m_WeaponState) {
		case PISTOL:
			m_Right_CurState = PISTOL_IDLE;
			break;
		case RIFLE:
			m_Right_CurState = RIFLE_IDLE;
			break;
		case SHOTGUN:
			m_Right_CurState = SHOTGUN_IDLE;
			break;
		}
		m_fFrameStart[RIGHT] = 0;
	}

	m_fFrameStart[LEFT] += m_fFrameSpeed[LEFT] * _fTimeDelta;
	if (m_fFrameStart[LEFT] > m_fFrameEnd[LEFT]) {
		m_fFrameStart[LEFT] = 0;
		m_Left_CurState = LEFT_IDLE;
	}
	if (bLegUse) {
		m_fFrameStart[LEG] += m_fFrameSpeed[LEG] * _fTimeDelta;
		if (m_fFrameStart[LEG] > m_fFrameEnd[LEG]) {
			bLegUse = false;
			m_fFrameStart[LEG] = 0;
			m_pLeg_TransformCom->Set_Pos(0.f, -WINCY / 3.f, 2.f);
			m_Leg_CurState = KICK;
		}
	}
}

void CPlayer::Update_Pos()
{
	if (m_Leg_CurState == KICK && m_fFrameStart[LEG] >= 5)
	{
		m_pLeg_TransformCom->Set_Pos(0.f, -400.f, 2.f);
	}

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