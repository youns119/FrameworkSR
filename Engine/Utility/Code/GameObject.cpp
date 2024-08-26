#include "..\..\Header\GameObject.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject& _rhs)
	: m_pGraphicDev(_rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}

CComponent* CGameObject::Get_Component(COMPONENTID _eID, const _tchar* _pComponentTag)
{
	CComponent* pComponent = Find_Component(_eID, _pComponentTag);
	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

HRESULT CGameObject::Ready_GameObject()
{
	return S_OK;
}

_int CGameObject::Update_GameObject(const _float& _fTimeDelta)
{
	for (auto& pComponent : m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC])
		pComponent.second->Update_Component(_fTimeDelta);

	return 0;
}

void CGameObject::LateUpdate_GameObject()
{
	for (auto& pComponent : m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

CComponent* CGameObject::Find_Component(COMPONENTID _eID, const _tchar* _pComponentTag)
{
	auto iter = find_if(m_mapComponent[(_uint)_eID].begin(), m_mapComponent[(_uint)_eID].end(), CTag_Finder(_pComponentTag));

	if (iter == m_mapComponent[(_uint)_eID].end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (_uint i = 0; i < (_uint)COMPONENTID::ID_END; i++)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}