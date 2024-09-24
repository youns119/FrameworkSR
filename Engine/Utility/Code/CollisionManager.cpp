#include "..\..\Header\CollisionManager.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CCollisionManager)

CCollisionManager::CCollisionManager()
	: m_bRender(false)
{
	m_mapColInfo.clear();
	m_vecCheck.clear();
	m_listRender.clear();
}

CCollisionManager::~CCollisionManager()
{
	Free();
}

void CCollisionManager::Update_Collision()
{
	for (_uint i = 0; i < m_vecCheck.size(); i++)
		CollisionGroupUpdate(m_vecCheck[i].first, m_vecCheck[i].second);
}

void CCollisionManager::Render_Collider()
{
	for (auto& Collider : m_listRender)
		Collider->Render_Collider();
}

void CCollisionManager::CollisionGroupUpdate(const _tchar* _pLeft, const _tchar* _pRight)
{
	CScene* pCurrScene = Engine::Get_CurrScene();

	multimap<const _tchar*, CGameObject*>* mapLeft = pCurrScene->Get_LayerObjects(_pLeft);
	multimap<const _tchar*, CGameObject*>* mapRight = pCurrScene->Get_LayerObjects(_pRight);

	map<ULONGLONG, bool>::iterator iter;

	for (auto iterLeft = mapLeft->begin(); iterLeft != mapLeft->end(); iterLeft++)
	{
		if (iterLeft->second == nullptr ||
			iterLeft->second->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Collider") == nullptr)
			continue;

		for (auto iterRight = mapRight->begin(); iterRight != mapRight->end(); iterRight++)
		{
			if (iterRight->second == nullptr ||
				iterRight->second->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Collider") == nullptr ||
				iterLeft->second == iterRight->second)
				continue;

			CCollider* pLeftCol = dynamic_cast<CCollider*>(iterLeft->second->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Collider"));
			CCollider* pRightCol = dynamic_cast<CCollider*>(iterRight->second->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Collider"));

			if (!pLeftCol->GetActive() || !pRightCol->GetActive())
				continue;

			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCol->GetID();
			ID.iRight_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ullID);

			if (iter == m_mapColInfo.end())
			{
				m_mapColInfo.insert(make_pair(ID.ullID, false));
				iter = m_mapColInfo.find(ID.ullID);
			}

			if (isCollision(pLeftCol, pRightCol))
			{
				if (iter->second)
				{
					pLeftCol->OnCollision(*pRightCol);
					pRightCol->OnCollision(*pLeftCol);
				}
				else
				{
					pLeftCol->OnCollisionEnter(*pRightCol);
					pRightCol->OnCollisionEnter(*pLeftCol);
					iter->second = true;
				}
			}
			else
			{
				if (iter->second)
				{
					pLeftCol->OnCollisionExit(*pRightCol);
					pRightCol->OnCollisionExit(*pLeftCol);
					iter->second = false;
				}
			}
		}
	}
}

bool CCollisionManager::isCollision(CCollider* _pLCol, CCollider* _pRCol)
{
	_float fX = abs(_pLCol->GetFinalPos().x - _pRCol->GetFinalPos().x);
	_float fY = abs(_pLCol->GetFinalPos().y - _pRCol->GetFinalPos().y);
	_float fZ = abs(_pLCol->GetFinalPos().z - _pRCol->GetFinalPos().z);

	_float fDiagonal = sqrt(fX * fX + fY * fY + fZ * fZ);

	_float fRadius = _pLCol->GetRadius() + _pRCol->GetRadius();

	return fRadius >= fDiagonal;
}

void CCollisionManager::Add_Collider(CCollider* _pCollider)
{
	m_listRender.push_back(_pCollider);
	_pCollider->AddRef();
}

void CCollisionManager::CheckGroup(const _tchar* _pLeft, const _tchar* _pRight)
{
	pair<const _tchar*, const _tchar*> pairGroup(_pLeft, _pRight);

	if (lstrcmp(_pLeft, _pRight) > 0)
	{
		pairGroup.first = _pRight;
		pairGroup.second = _pLeft;
	}

	auto iter = find(m_vecCheck.begin(), m_vecCheck.end(), pairGroup);

	if (iter == m_vecCheck.end())
		m_vecCheck.push_back(pairGroup);
}

void CCollisionManager::Clear_Collider()
{
	for_each(m_listRender.begin(), m_listRender.end(), CDeleteObj());
	m_listRender.clear();
}

void CCollisionManager::Reset()
{
	m_mapColInfo.clear();
	m_vecCheck.clear();

	Clear_Collider();
}

void CCollisionManager::Free()
{
	Reset();
}