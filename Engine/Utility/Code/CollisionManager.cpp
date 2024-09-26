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

vector<CGameObject*> CCollisionManager::RayCast(_vec3 vRayStart, _vec3 vRayDir)
{
	auto Objects = CManagement::GetInstance()->Get_CurrScene()->Get_LayerObjects(L"Layer_GameLogic");
	vector<CGameObject*> pHitObject;

	for (auto pair : *Objects) {
		CGameObject* pTargetObject = pair.second;
		CComponent* pTargetComponent = pTargetObject->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Collider");
		CCollider* pTargetCollider = dynamic_cast<CCollider*>(pTargetComponent);
		if (pTargetCollider == nullptr)
			continue;

		_float fRadius = pTargetCollider->GetRadius();
		_vec3 vCenter = pTargetCollider->GetFinalPos();
		_vec3 vLength = vRayStart - vCenter;



		// 구 와 직선의 충돌은 결과값이 0 보다 크거나같아야 충돌한거로 판정.
		// 레이의 방정식을 사용하여 구와의 충돌 여부를 확인
		// P(t) = P0 + t(내적점)vRayDir
		// 
		// 구와의 거리를 제곱으로 계산하여 최소값을 찾습니다
		// 거리 제곱 공식은 다음과 같습니다
		// (P0 + t(내적점)vRayDir−C)(내적점)(P0 + t(내적점)vRayDir−C) = r2
		// 
		//기하학적 변환 : 위 공식을 정리하여 t에 대한 이차 방정식 형태로 변환합니다
		//At^2 + Bt + C = 0
		_float A = D3DXVec3Dot(&vRayDir, &vRayDir);
		_float B = 2.0f * D3DXVec3Dot(&vLength, &vRayDir);
		_float C = D3DXVec3Dot(&vLength, &vLength) - (fRadius * fRadius);

		//판별식을 계산하여 실근이 존재하는지 확인
		//B^2 - 4ac
		_float fDiscriminant = B * B - 4 * A * C;

		if (fDiscriminant >= 0)
		{
			pHitObject.push_back(pTargetObject);
		}
	}
	return pHitObject;
}

_bool CCollisionManager::RayCast2(_vec3 vRayStart, _vec3 vRayDir)
{
	auto Objects = CManagement::GetInstance()->Get_CurrScene()->Get_LayerObjects(L"Layer_Monster");
	for (auto pair : *Objects) {

		CGameObject* pTargetObject = pair.second;
		CComponent* pTargetComponent = pTargetObject->Get_Component(COMPONENTID::ID_STATIC, L"Com_HitBufferCom");
		if (pTargetComponent == nullptr)
			continue;
		CRcCol* pTargetCol = dynamic_cast<CRcCol*>(pTargetComponent);
		CTransform* TargetTransfrom = dynamic_cast<CTransform*>(pTargetComponent->GetOwner()->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform"));
		const _matrix* TargetWorld = TargetTransfrom->Get_WorldMatrix();

		_vec3 v0 = *pTargetCol->VertexPos(0);
		_vec3 v1 = *pTargetCol->VertexPos(1);
		_vec3 v2 = *pTargetCol->VertexPos(2);
		_vec3 v3 = *pTargetCol->VertexPos(3);

		D3DXVec3TransformCoord(&v0, &v0, TargetWorld);
		D3DXVec3TransformCoord(&v1, &v1, TargetWorld);
		D3DXVec3TransformCoord(&v2, &v2, TargetWorld);
		D3DXVec3TransformCoord(&v3, &v3, TargetWorld);

		float u, v, dist;
		_bool intersected = D3DXIntersectTri(&v0, &v1, &v2, &vRayStart, &vRayDir, &u, &v, &dist);
		_bool intersected2 = D3DXIntersectTri(&v0, &v2, &v3, &vRayStart, &vRayDir, &u, &v, &dist);
		if (intersected || intersected2) {
			dynamic_cast<CCharacter*>(pTargetObject)->Damaged();
			return true;
		}
	}
	return false;
}

void CCollisionManager::Free()
{
	Reset();
}