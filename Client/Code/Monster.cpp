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
	, m_eCurState(MONSTERSTATE::MONSTER_ATTACK) //Jonghan Change
	, m_ePreState(MONSTERSTATE::MONSTER_ATTACK) //Jonghan Change
	//, m_pTextureCom(nullptr)// Jonghan Change
{
	for (_int i = 0; i < MONSTERSTATE::MONSTER_END; ++i)
		m_pTextureCom[i] = nullptr;
}

CMonster::~CMonster()
{
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CMonster* pMonster = new CMonster(_pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("pMonster Create Failed");
		return nullptr;
	}

	return pMonster;
}

HRESULT CMonster::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//Jonghan Monster Change Start

	//m_pTransformCom->Set_Pos(10.f, 0.f, 10.f); 

	_matrix matWorld;

	m_pTransformCom->Get_WorldMatrix(&matWorld);

	matWorld._41 = 10.f;
	matWorld._43 = 10.f;

	m_pTransformCom->Set_WorldMatrix(&matWorld);

	//Jonghan Monster Change End
	return S_OK;
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

void CMonster::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//Jonghan Monster Change Start

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom[m_eCurState]->Set_Texture((_uint)m_fFrame); //Jonghan Change


	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//Jonghan Monster Change End
}

void CMonster::Change_State() // Jonghan Change
{
	//When Player Picking Monster_Collider (Like Head Body Bull), it'll be Dealing Monster Hp and Setting the State from Hp
	if (Engine::Get_DIMouseState(MOUSEKEYSTATE::DIM_RB) & 0x80)
	{
		switch (m_eCurState)
		{
		case CMonster::MONSTER_ATTACK:
			m_eCurState = MONSTER_HEADSHOT;
			break;
		case CMonster::MONSTER_HEADSHOT:
			m_eCurState = MONSTER_PUSH_ONE;
			break;
		case CMonster::MONSTER_PUSH_ONE:
			m_eCurState = MONSTER_PUSH_TWO;
			break;
		case CMonster::MONSTER_PUSH_TWO:
			m_eCurState = MONSTER_BULLSHOT;
			break;
		case CMonster::MONSTER_BULLSHOT:
			m_eCurState = MONSTER_SHOT_ONE;
			break;
		case CMonster::MONSTER_SHOT_ONE:
			m_eCurState = MONSTER_SHOT_TWO;
			break;
		case CMonster::MONSTER_SHOT_TWO:
			m_eCurState = MONSTER_ATTACK;
			break;
		}
	}

}

HRESULT CMonster::Add_Component()
{

	return S_OK;
}

void CMonster::State_Check() //Jonghan Change
{
	
}

void CMonster::Free()
{
	Engine::CGameObject::Free();
}