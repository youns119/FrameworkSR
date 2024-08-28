#include "pch.h"
#include "..\Header\Player.h"
#include "Export_Utility.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 _pGraphicDev)
    : Engine::CGameObject(_pGraphicDev)
    , m_pBufferCom(nullptr)
    , m_pTransformCom(nullptr)
    , m_pTextureCom(nullptr)
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

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& _fTimeDelta)
{
	Key_Input(_fTimeDelta);

	return Engine::CGameObject::Update_GameObject(_fTimeDelta);
}

void CPlayer::LateUpdate_GameObject()
{
	Engine::CGameObject::LateUpdate_GameObject();
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Buffer", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_PlayerTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_STATIC].insert({ L"Com_Texture", pComponent });

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CPlayer::Key_Input(const _float& _fTimeDelta)
{
	_vec3 vUp;

	m_pTransformCom->Get_Info(INFO::INFO_UP, &vUp);

	if (GetAsyncKeyState(VK_UP))
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), _fTimeDelta, 10.f);
	if (GetAsyncKeyState(VK_DOWN))
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), _fTimeDelta, -10.f);
	if (GetAsyncKeyState('Q'))
		m_pTransformCom->Rotation(ROTATION::ROT_X, D3DXToRadian(180.f * _fTimeDelta));
	if (GetAsyncKeyState('A'))
		m_pTransformCom->Rotation(ROTATION::ROT_X, D3DXToRadian(-180.f * _fTimeDelta));
	if (GetAsyncKeyState('W'))
		m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(180.f * _fTimeDelta));
	if (GetAsyncKeyState('S'))
		m_pTransformCom->Rotation(ROTATION::ROT_Y, D3DXToRadian(-180.f * _fTimeDelta));
	if (GetAsyncKeyState('E'))
		m_pTransformCom->Rotation(ROTATION::ROT_Z, D3DXToRadian(180.f * _fTimeDelta));
	if (GetAsyncKeyState('D'))
		m_pTransformCom->Rotation(ROTATION::ROT_Z, D3DXToRadian(-180.f * _fTimeDelta));
}

void CPlayer::Free()
{
	Engine::CGameObject::Free();
}