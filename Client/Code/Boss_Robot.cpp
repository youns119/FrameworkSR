#include "pch.h"
#include "../Header/Boss_Robot.h"
#include "Export_System.h"
#include "Export_Utility.h"

CBoss_Robot::CBoss_Robot(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CMonster(_pGraphicDev)
	, m_eCurState(CBoss_Robot::BOSS_SPAWN)
	, m_ePreState(CBoss_Robot::BOSS_SPAWN)
{
	for (_int i = 0; i < CBoss_Robot::BOSS_END; ++i)
		m_pTextureCom[i] = nullptr;
}

CBoss_Robot::~CBoss_Robot()
{
}

CBoss_Robot* CBoss_Robot::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CBoss_Robot* pGameObject = new CBoss_Robot(_pGraphicDev);

	if (FAILED(pGameObject->Ready_GameObject()))
	{
		Safe_Release(pGameObject);
		MSG_BOX("Boss_Robot Create Failed");
		return nullptr;
	}

	return pGameObject;
}

HRESULT CBoss_Robot::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Pos(10.f, 0.f, 25.f);

	return S_OK;
}

void CBoss_Robot::LateUpdate_GameObject()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO::INFO_POS, &vPos);

	//CTerrainTex* pTerrainBufferCom = dynamic_cast<CTerrainTex*>(Engine::Get_Component(COMPONENTID::ID_STATIC, L"Layer_GameLogic", L"Terrain", L"Com_Buffer"));
	//NULL_CHECK(pTerrainBufferCom);
	//
	//_float fY = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainBufferCom->Get_VtxPos(), VTXCNTX, VTXCNTZ); //this is Instance code to check Monster can ride the tile

	m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);

	CGameObject::Compute_ViewZ(&vPos);

	Change_State(); //this is Instance code for check Animation of Monster

	State_Check();

	Engine::CGameObject::LateUpdate_GameObject();
}

void CBoss_Robot::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	//Jonghan Monster Change Start

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pTextureCom[m_eCurState]->Set_Texture((_uint)m_fFrame); //Jonghan Change


	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CBoss_Robot::Change_State()
{
}

HRESULT CBoss_Robot::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	return S_OK;
}

void CBoss_Robot::State_Check()
{
}

void CBoss_Robot::Attack()
{
}

void CBoss_Robot::Free()
{
	CMonster::Free();
}
