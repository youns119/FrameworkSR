 #include "pch.h"
#include "..\Header\Monster.h"
#include "Export_Utility.h"
#include "Export_System.h" // Jonghan Change

CMonster::CMonster(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CGameObject(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_fFrame(0.f)
	, m_fMaxFrame(14.f) //Jonghan Change
{
}

CMonster::~CMonster()
{
}

_int CMonster::Update_GameObject(const _float& _fTimeDelta)
{
	//Jonghan Monster Change Start

	m_fFrame += 13.f * _fTimeDelta;

	if (m_fMaxFrame < m_fFrame) // Jonghan Change
		m_fFrame = 0.f;

	_int iExit = Engine::CGameObject::Update_GameObject(_fTimeDelta);

	//Engine::CTransform* pPlayerTransform = dynamic_cast<Engine::CTransform*>
	//	(Engine::Get_Component(COMPONENTID::ID_DYNAMIC, L"Layer_GameLogic", L"Player", L"Com_Transform"));
	//NULL_CHECK_RETURN(pPlayerTransform, -1);
	//
	//_vec3 vPlayerPos;
	//pPlayerTransform->Get_Info(INFO::INFO_POS, &vPlayerPos);
	//
	//m_pTransformCom->Chase_Target(&vPlayerPos, 5.f * _fTimeDelta);

	//Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);
	_matrix		matWorld, matView, matBill, matResult;
	m_pTransformCom->Get_WorldMatrix(&matWorld);

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	matResult = matBill * matWorld;

	m_pTransformCom->Set_WorldMatrix(&(matResult));

	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	//Jonghan Monster Change End

	return iExit;
}

void CMonster::LateUpdate_GameObject()
{
	//Jonghan Monster Change Start
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(COMPONENTID::ID_STATIC, L"Layer_GameLogic", L"Terrain", L"Com_Buffer"));
	NULL_CHECK(pTerrainBufferCom);

	_float fY = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fY + 1.f, vPos.z);

	CGameObject::Compute_ViewZ(&vPos);

	Change_State();

	State_Check();

	//Jonghan Monster Change End
	Engine::CGameObject::LateUpdate_GameObject();
}

void CMonster::Free()
{
	Engine::CGameObject::Free();
}