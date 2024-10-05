#include "..\..\Header\UIUnit.h"

CUIUnit::CUIUnit(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
	, m_bRender(false)
	, m_fViewZ(0.f)
	, m_pOwnerUI(nullptr)
{
	for (_uint i = 0; i < (_uint)COMPONENTID::ID_END; i++)
		m_mapComponent[i].clear();

	m_pGraphicDev->AddRef();
}

CUIUnit::CUIUnit(const CUIUnit& _rhs)
	: m_pGraphicDev(_rhs.m_pGraphicDev)
	, m_bRender(_rhs.m_bRender)
	, m_fViewZ(_rhs.m_fViewZ)
	, m_pOwnerUI(nullptr)
{
	for (_uint i = 0; i < (_uint)COMPONENTID::ID_END; i++)
		m_mapComponent[i] = _rhs.m_mapComponent[i];

	m_pGraphicDev->AddRef();
}

CUIUnit::~CUIUnit()
{
}

CComponent* CUIUnit::Get_Component(COMPONENTID _eID, const _tchar* _pComponentTag)
{
	CComponent* pComponent = Find_Component(_eID, _pComponentTag);
	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

_int CUIUnit::Update_Unit(const _float& _fTimeDelta)
{
	for (auto& pComponent : m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC])
		pComponent.second->Update_Component(_fTimeDelta);

	return 0;
}

void CUIUnit::LateUpdate_Unit()
{
	for (auto& pComponent : m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

CComponent* CUIUnit::Find_Component(COMPONENTID _eID, const _tchar* _pComponentTag)
{
	auto iter = find_if(m_mapComponent[(_uint)_eID].begin(), m_mapComponent[(_uint)_eID].end(), CTag_Finder(_pComponentTag));

	if (iter == m_mapComponent[(_uint)_eID].end())
		return nullptr;

	return iter->second;
}

void CUIUnit::Free()
{
	for (_uint i = 0; i < (_uint)COMPONENTID::ID_END; i++)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}