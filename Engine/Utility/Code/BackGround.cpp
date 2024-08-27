#include "pch.h"
#include "..\Header\BackGround.h"
#include "Export_Utility.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 _pGraphicDev)
	: Engine::CGameObject(_pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CBackGround::~CBackGround()
{
}

CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CBackGround* pBackGround = new CBackGround(_pGraphicDev);

	if (FAILED(pBackGround->Ready_GameObject()))
	{
		Safe_Release(pBackGround);
		MSG_BOX("pBackGround Create Failed");
		return nullptr;
	}

	return pBackGround;
}

HRESULT CBackGround::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(1.f, 1.f, 1.f);

	return S_OK;
}

_int CBackGround::Update_GameObject(const _float& _fTimeDelta)
{
	Key_Input(_fTimeDelta);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CBackGround::LateUpdate_GameObject()
{
	Engine::CGameObject::LateUpdate_GameObject();
}

void CBackGround::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//D3DXMATRIX matWorld;
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, D3DXMatrixIdentity(&matWorld));

	m_pBufferCom->Render_Buffer();

	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBackGround::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcCol*>(Engine::Clone_Proto(L"Proto_RcCol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CBackGround::Key_Input(const _float& _fTimeDelta)
{
	_vec3 vUp;

	m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);

	if (GetAsyncKeyState('W'))
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), _fTimeDelta, 10.f);
	if (GetAsyncKeyState('S'))
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), _fTimeDelta, -10.f);
	if (GetAsyncKeyState('A'))
		m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(180.f * _fTimeDelta));
	if (GetAsyncKeyState('D'))
		m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(-180.f * _fTimeDelta));
}

void CBackGround::Free()
{
	Engine::CGameObject::Free();
}