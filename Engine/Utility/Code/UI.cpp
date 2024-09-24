#include "..\..\Header\UI.h"

CUI::CUI(LPDIRECT3DDEVICE9 _pGraphicDev)
	: m_pGraphicDev(_pGraphicDev)
	, m_eUIType(UITYPE::UI_END)
	, m_bActive(false)
	, m_fViewZ(0.f)
{
	for (_uint i = 0; i < (_uint)COMPONENTID::ID_END; i++)
		m_mapComponent[i].clear();

	m_pGraphicDev->AddRef();
}

CUI::CUI(const CUI& _rhs)
	: m_pGraphicDev(_rhs.m_pGraphicDev)
	, m_eUIType(UITYPE::UI_END)
	, m_bActive(_rhs.m_bActive)
{
	m_pGraphicDev->AddRef();
}

CUI::~CUI()
{
}

CComponent* CUI::Get_Component(COMPONENTID _eID, const _tchar* _pComponentTag)
{
	CComponent* pComponent = Find_Component(_eID, _pComponentTag);
	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

_int CUI::Update_UI(const _float& _fTimeDelta)
{
	for (auto& pComponent : m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC])
		pComponent.second->Update_Component(_fTimeDelta);

	return 0;
}

void CUI::LateUpdate_UI()
{
	for (auto& pComponent : m_mapComponent[(_uint)COMPONENTID::ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

CComponent* CUI::Find_Component(COMPONENTID _eID, const _tchar* _pComponentTag)
{
	auto iter = find_if(m_mapComponent[(_uint)_eID].begin(), m_mapComponent[(_uint)_eID].end(), CTag_Finder(_pComponentTag));

	if (iter == m_mapComponent[(_uint)_eID].end())
		return nullptr;

	return iter->second;
}

void CUI::Free()
{
	for (_uint i = 0; i < (_uint)COMPONENTID::ID_END; i++)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	Safe_Release(m_pGraphicDev);
}