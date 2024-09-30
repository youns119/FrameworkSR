#include "../../Header/Character.h"

CCharacter::CCharacter(LPDIRECT3DDEVICE9 _pGraphicDev)
	:CGameObject(_pGraphicDev)
	, m_bIsDead(false)
{
}

CCharacter::CCharacter(const CCharacter& _rhs)
	:CGameObject(_rhs.m_pGraphicDev)
{
}

CCharacter::~CCharacter()
{
}

HRESULT CCharacter::Ready_GameObject()
{
	return S_OK;
}

_int CCharacter::Update_GameObject(const _float& _fTimeDelta)
{
	for (auto& pComponent : m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC])
		pComponent.second->Update_Component(_fTimeDelta);

	return 0;
}

void CCharacter::LateUpdate_GameObject()
{
	for (auto& pComponent : m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

void CCharacter::Free()
{
	CGameObject::Free();
}
