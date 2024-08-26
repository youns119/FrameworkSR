#include "..\..\Header\ComponentManager.h"

IMPLEMENT_SINGLETON(CComponentManager)

CComponentManager::CComponentManager()
{
	m_mapProtoType.clear();
}

CComponentManager::~CComponentManager()
{
	Free();
}

HRESULT CComponentManager::Ready_Proto(const _tchar* _pComponentTag, CComponent* _pComponent)
{
	CComponent* pInstance = Find_Proto(_pComponentTag);

	if (pInstance != nullptr)
		return E_FAIL;

	m_mapProtoType.insert({ _pComponentTag, _pComponent });

	return S_OK;
}

CComponent* CComponentManager::Clone_Proto(const _tchar* _pComponentTag)
{
	CComponent* pInstance = Find_Proto(_pComponentTag);
	NULL_CHECK_RETURN(pInstance, nullptr);

	return pInstance->Clone();
}

CComponent* CComponentManager::Find_Proto(const _tchar* _pComponentTag)
{
	auto iter = find_if(m_mapProtoType.begin(), m_mapProtoType.end(), CTag_Finder(_pComponentTag));

	if (iter == m_mapProtoType.end())
		return nullptr;

	return iter->second;
}

void CComponentManager::Free()
{
	for_each(m_mapProtoType.begin(), m_mapProtoType.end(), CDeleteMap());
	m_mapProtoType.clear();
}