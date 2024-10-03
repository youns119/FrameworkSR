#include "..\..\Header\Layer.h"

CLayer::CLayer()
{
	m_mapObject.clear();
}

CLayer::~CLayer()
{
}

CGameObject* CLayer::Get_GameObject(const _tchar* _pObjTag)
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(_pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second;
}

CGameObject* CLayer::Get_GameObject(CGameObject* _pGameObject)
{
	for (auto iter = m_mapObject.begin(); iter != m_mapObject.end(); ++iter)
	{
		if (iter->second == _pGameObject)
			return iter->second;
		else continue;
	}

	return nullptr;
}

CComponent* CLayer::Get_Component(COMPONENTID _eID, const _tchar* _pObjTag, const _tchar* _pComponentTag)
{
	auto iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(_pObjTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(_eID, _pComponentTag);
}

CLayer* CLayer::Create()
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer()))
	{
		Safe_Release(pLayer);
		MSG_BOX("layer create failed");
		return nullptr;
	}

	return pLayer;
}

HRESULT CLayer::Add_GameObject(const _tchar* _pObjTag, CGameObject* _pGameObject)
{
	NULL_CHECK_RETURN(_pGameObject, E_FAIL);

	m_mapObject.insert({ _pObjTag, _pGameObject });

	return S_OK;
}

HRESULT CLayer::Ready_Layer()
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float& _fTimeDelta)
{
	_int iResult(0);

	for (auto& pObj : m_mapObject)
	{
		iResult = pObj.second->Update_GameObject(_fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CLayer::LateUpdate_Layer()
{
	for (auto& pObj : m_mapObject)
		pObj.second->LateUpdate_GameObject();
}

void CLayer::Free()
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());
	m_mapObject.clear();
}