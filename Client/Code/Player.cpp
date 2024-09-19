#include "pch.h"
#include "..\Header\Player.h"
#include "Export_System.h"
#include "Export_Utility.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CGameObject(_pGraphicDev)
	, m_pRight_BufferCom(nullptr)
	, m_pRight_TransformCom(nullptr)
	, m_pRight_TextureCom(nullptr)
	, m_pLeft_BufferCom(nullptr)
	, m_pLeft_TransformCom(nullptr)
	, m_pLeft_TextureCom(nullptr)
	, m_pCalculatorCom(nullptr)
	//Beomseung
{
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

	m_pLeft_TransformCom->Set_Scale(1, 1, 1);
	m_pRight_TransformCom->Set_Scale(1, 1, 1);

	m_pRight_TransformCom->Set_Pos(2.f, 1.f, 2.f);
	m_pLeft_TransformCom->Set_Pos(-2.f, 1.f, 2.f);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& _fTimeDelta)
{
	Key_Input(_fTimeDelta);
	Mouse_Move();
	Mouse_Fix();

	m_pRight_TransformCom->Set_Pos(2.f, 0, 2.f);
	m_pLeft_TransformCom->Set_Pos(-2.f, 0, 2.f);

	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);

}

void CPlayer::LateUpdate_GameObject()
{
	_vec3 vPos;
	m_pBody_TransformCom->Get_Info(INFO::INFO_POS, &vPos);
	//Beomseung
	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(COMPONENTID::ID_STATIC, L"Layer_GameLogic", L"Terrain", L"Com_Buffer"));
	NULL_CHECK(pTerrainBufferCom);

	_float fY = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pBody_TransformCom->Set_Pos(vPos.x, fY + 1.f, vPos.z);

	Engine::CGameObject::LateUpdate_GameObject();
}

void CPlayer::Render_GameObject()
{
	//Beomseung Fix

	_matrix mat = *m_pRight_TransformCom->Get_WorldMatrix() * *m_pBody_TransformCom->Get_WorldMatrix();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pRight_TextureCom->Set_Texture();
	m_pRight_BufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	mat = *m_pLeft_TransformCom->Get_WorldMatrix() * *m_pBody_TransformCom->Get_WorldMatrix();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pLeft_TextureCom->Set_Texture();
	m_pLeft_BufferCom->Render_Buffer();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CPlayer::Add_Component()
{
	CComponent* pComponent = NULL;

	//Beomseung
	pComponent = m_pBody_TransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Body_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Body_Transform", pComponent });

	pComponent = m_pRight_BufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RightArmBuffer"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pLeft_BufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_LeftArmBuffer"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer2", pComponent });

	pComponent = m_pRight_TextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_RightArmTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pLeft_TextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_LeftArmTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture2", pComponent });

	pComponent = m_pRight_TransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Right_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Right_Transform", pComponent });

	pComponent = m_pLeft_TransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Left_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Left_Transform", pComponent });

	pComponent = m_pCalculatorCom = dynamic_cast<CCalculator*>(Engine::Clone_Proto(L"Proto_Calculator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Calculator", pComponent });

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

	m_pBody_TransformCom->Get_Info(INFO::INFO_LOOK, &vLook);
	m_pBody_TransformCom->Get_Info(INFO::INFO_RIGHT, &vRight);

	if (Engine::Get_DIKeyState(DIK_W) & 0x80) {
		//Beomseung
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, 10.f);
	}
	if (Engine::Get_DIKeyState(DIK_S) & 0x80) {
		//Beomseung   
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), _fTimeDelta, -10.f);

	}
	if (Engine::Get_DIKeyState(DIK_A) & 0x80) {
		//Beomseung    
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), _fTimeDelta, -10.f);

	}
	if (Engine::Get_DIKeyState(DIK_D) & 0x80) {
		//Beomseung    
		m_pBody_TransformCom->Move_Pos(D3DXVec3Normalize(&vRight, &vRight), _fTimeDelta, 10.f);

	}
}

void CPlayer::Mouse_Move()
{

	_long dwMouseMove(0);

	if (dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_Y))
	{
		m_pBody_TransformCom->Rotation(ROTATION::ROT_X, D3DXToRadian(dwMouseMove / 20.f));
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X))
	{
		m_pBody_TransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(dwMouseMove / 20.f));
	}
}

void CPlayer::Mouse_Fix()
{
	POINT ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
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