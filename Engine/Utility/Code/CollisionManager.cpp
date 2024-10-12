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

		CCollider* pLeftCol = static_cast<CCollider*>(iterLeft->second->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Collider"));

		if (!pLeftCol->GetActive()) continue;

		for (auto iterRight = mapRight->begin(); iterRight != mapRight->end(); iterRight++)
		{
			if (iterRight->second == nullptr ||
				iterRight->second->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Collider") == nullptr ||
				iterLeft->second == iterRight->second)
				continue;

			_bool bIsCollide = false;

			CCollider* pRightCol = static_cast<CCollider*>(iterRight->second->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Collider"));

			if (!pRightCol->GetActive()) continue;

			COLLIDER_ID ID;
			ID.iLeft_id = pLeftCol->GetID();
			ID.iRight_id = pRightCol->GetID();

			iter = m_mapColInfo.find(ID.ullID);

			if (iter == m_mapColInfo.end())
			{
				m_mapColInfo.insert(make_pair(ID.ullID, false));
				iter = m_mapColInfo.find(ID.ullID);
			}

			if (lstrcmp(_pLeft, L"Layer_Wall") == 0 || lstrcmp(_pRight, L"Layer_Wall") == 0)
				bIsCollide = isAABB(pLeftCol, pRightCol);
			else
				bIsCollide = isCollision(pLeftCol, pRightCol);

			if (bIsCollide)
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

_bool CCollisionManager::isAABB(CCollider* _pLCol, CCollider* _pRCol)
{
	if (isCollision(_pLCol, _pRCol))
	{
		CCollider::AABB* pLAABB = _pLCol->GetAABB();
		CCollider::AABB* pRAABB = _pRCol->GetAABB();

		return ((pLAABB->vMin.x < pRAABB->vMax.x && pLAABB->vMax.x > pRAABB->vMin.x) &&
			(pLAABB->vMin.y < pRAABB->vMax.y && pLAABB->vMax.y > pRAABB->vMin.y) &&
			(pLAABB->vMin.z < pRAABB->vMax.z && pLAABB->vMax.z > pRAABB->vMin.z));
	}
	else
		return false;
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

_bool CCollisionManager::RayCast(_vec3 vRayStart, _vec3 vRayDir)
{
	auto Objects = CManagement::GetInstance()->Get_CurrScene()->Get_LayerObjects(L"Layer_Player");
	vector<CGameObject*> pHitObject;

	for (auto pair : *Objects) {
		CGameObject* pTargetObject = pair.second;
		CCharacter* pCharacter = dynamic_cast<CCharacter*>(pTargetObject);
		//CComponent* pTargetComponent = pTargetObject->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Body_Transform");
		//dynamic_cast<CCollider*>(pTargetComponent);
		CCollider* pTargetCollider = static_cast<CCollider*>(pTargetObject->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Collider"));
		if (pTargetCollider == nullptr)
			continue;

		_float fRadius = pTargetCollider->GetRadius();
		_vec3 vCenter = pTargetCollider->GetFinalPos();
		_vec3 vLength = vRayStart - vCenter;

		_float A = D3DXVec3Dot(&vRayDir, &vRayDir);
		_float B = 2.0f * D3DXVec3Dot(&vLength, &vRayDir);
		_float C = D3DXVec3Dot(&vLength, &vLength) - (fRadius * fRadius);

		//판별식을 계산하여 실근이 존재하는지 확인
		//B^2 - 4ac
		_float fDiscriminant = B * B - 4 * A * C;

		if (fDiscriminant >= 0)
		{
			pCharacter->Damaged();
			return true;
		}
	}
	return false;
}

_bool CCollisionManager::RayCast2(_vec3 vRayStart, _vec3 vRayDir)
{
	auto Objects = CManagement::GetInstance()->Get_CurrScene()->Get_LayerObjects(L"Layer_Monster");
	for (auto pair : *Objects) {

		CGameObject* pTargetObject = pair.second;
		CCharacter* pCharacter = dynamic_cast<CCharacter*>(pTargetObject);
		if (nullptr != pCharacter && pCharacter->Get_IsDead())
			continue;
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
			//헤드피격
			pTargetComponent = pTargetObject->Get_Component(COMPONENTID::ID_STATIC, L"Com_HeadHit");
			pTargetCol = dynamic_cast<CRcCol*>(pTargetComponent);
			if (pTargetComponent == nullptr) {
				//몸체피격만있을때
				dynamic_cast<CCharacter*>(pTargetObject)->Damaged(DAMAGED_STATE::DAMAGED_BODYSHOT);
				return true;
			}
			v0 = *pTargetCol->VertexPos(0);
			v1 = *pTargetCol->VertexPos(1);
			v2 = *pTargetCol->VertexPos(2);
			v3 = *pTargetCol->VertexPos(3);

			D3DXVec3TransformCoord(&v0, &v0, TargetWorld);
			D3DXVec3TransformCoord(&v1, &v1, TargetWorld);
			D3DXVec3TransformCoord(&v2, &v2, TargetWorld);
			D3DXVec3TransformCoord(&v3, &v3, TargetWorld);

			intersected = D3DXIntersectTri(&v0, &v1, &v2, &vRayStart, &vRayDir, &u, &v, &dist);
			intersected2 = D3DXIntersectTri(&v0, &v2, &v3, &vRayStart, &vRayDir, &u, &v, &dist);
			if (intersected || intersected2) {
				dynamic_cast<CCharacter*>(pTargetObject)->Damaged(DAMAGED_STATE::DAMAGED_HEADSHOT);
				return true;
			}
			//급소피격
			pTargetComponent = pTargetObject->Get_Component(COMPONENTID::ID_STATIC, L"Com_CriticalHit");
			pTargetCol = dynamic_cast<CRcCol*>(pTargetComponent);

			v0 = *pTargetCol->VertexPos(0);
			v1 = *pTargetCol->VertexPos(1);
			v2 = *pTargetCol->VertexPos(2);
			v3 = *pTargetCol->VertexPos(3);

			D3DXVec3TransformCoord(&v0, &v0, TargetWorld);
			D3DXVec3TransformCoord(&v1, &v1, TargetWorld);
			D3DXVec3TransformCoord(&v2, &v2, TargetWorld);
			D3DXVec3TransformCoord(&v3, &v3, TargetWorld);

			intersected = D3DXIntersectTri(&v0, &v1, &v2, &vRayStart, &vRayDir, &u, &v, &dist);
			intersected2 = D3DXIntersectTri(&v0, &v2, &v3, &vRayStart, &vRayDir, &u, &v, &dist);
			if (intersected || intersected2) {
				dynamic_cast<CCharacter*>(pTargetObject)->Damaged(DAMAGED_STATE::DAMAGED_BULLSHOT);
				return true;
			}
			//몸체피격
			dynamic_cast<CCharacter*>(pTargetObject)->Damaged(DAMAGED_STATE::DAMAGED_BODYSHOT);
			return true;
		}
	}
	return false;
}

_bool CCollisionManager::FireRayCast(_vec3 _vRayStart, _vec3 _vRayDir, _vec3& _vOut, const _float& _fDamage)
{
	auto Objects = CManagement::GetInstance()->Get_CurrScene()->Get_LayerObjects(L"Layer_Monster");
	for (auto pair : *Objects) {

		CGameObject* pTargetObject = pair.second;
		CCharacter* pCharacter = dynamic_cast<CCharacter*>(pTargetObject);
		if (nullptr != pCharacter && pCharacter->Get_IsDead())
			continue;
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
		_bool intersected = D3DXIntersectTri(&v0, &v1, &v2, &_vRayStart, &_vRayDir, &u, &v, &dist);
		_bool intersected2 = D3DXIntersectTri(&v0, &v2, &v3, &_vRayStart, &_vRayDir, &u, &v, &dist);
		if (intersected || intersected2) {
			//헤드피격
			pTargetComponent = pTargetObject->Get_Component(COMPONENTID::ID_STATIC, L"Com_HeadHit");
			if (pTargetComponent == nullptr)
			{
				dynamic_cast<CCharacter*>(pTargetObject)->Damaged(DAMAGED_STATE::DAMAGED_BODYSHOT, 1.f);
				return true;
			}
			pTargetCol = dynamic_cast<CRcCol*>(pTargetComponent);
			v0 = *pTargetCol->VertexPos(0);
			v1 = *pTargetCol->VertexPos(1);
			v2 = *pTargetCol->VertexPos(2);
			v3 = *pTargetCol->VertexPos(3);

			D3DXVec3TransformCoord(&v0, &v0, TargetWorld);
			D3DXVec3TransformCoord(&v1, &v1, TargetWorld);
			D3DXVec3TransformCoord(&v2, &v2, TargetWorld);
			D3DXVec3TransformCoord(&v3, &v3, TargetWorld);

			intersected = D3DXIntersectTri(&v0, &v1, &v2, &_vRayStart, &_vRayDir, &u, &v, &dist);
			intersected2 = D3DXIntersectTri(&v0, &v2, &v3, &_vRayStart, &_vRayDir, &u, &v, &dist);
			if (intersected || intersected2) {
				dynamic_cast<CCharacter*>(pTargetObject)->Damaged(DAMAGED_STATE::DAMAGED_HEADSHOT);
				// V1 + U(V2 - V1) + V(V3 - V1)
				//if (intersected)
				//{
				//	//_vOut = v0 + (u * (v1 - v0)) + (v * (v2 - v0));
				//	_int iTemp = rand() % 100;
				//	_vOut = _vRayStart + (dist * _vRayDir *(1 - (iTemp * 0.0001f)));
				//}
				//else
				//{
				//	//_vOut = v0 + (u * (v2 - v1)) + (v * (v3 - v0));
				//	_int iTemp = rand() % 100;
				//	_vOut = _vRayStart + (dist * _vRayDir * (1 - (iTemp * 0.0001f)));
				//}

				_int iTemp = rand() % 100;
				_vOut = _vRayStart + (dist * _vRayDir * (1 - (iTemp * 0.001f)));

				return true;
			}
			//급소피격
			pTargetComponent = pTargetObject->Get_Component(COMPONENTID::ID_STATIC, L"Com_CriticalHit");
			pTargetCol = dynamic_cast<CRcCol*>(pTargetComponent);

			v0 = *pTargetCol->VertexPos(0);
			v1 = *pTargetCol->VertexPos(1);
			v2 = *pTargetCol->VertexPos(2);
			v3 = *pTargetCol->VertexPos(3);

			D3DXVec3TransformCoord(&v0, &v0, TargetWorld);
			D3DXVec3TransformCoord(&v1, &v1, TargetWorld);
			D3DXVec3TransformCoord(&v2, &v2, TargetWorld);
			D3DXVec3TransformCoord(&v3, &v3, TargetWorld);

			intersected = D3DXIntersectTri(&v0, &v1, &v2, &_vRayStart, &_vRayDir, &u, &v, &dist);
			intersected2 = D3DXIntersectTri(&v0, &v2, &v3, &_vRayStart, &_vRayDir, &u, &v, &dist);
			if (intersected || intersected2) {
				dynamic_cast<CCharacter*>(pTargetObject)->Damaged(DAMAGED_STATE::DAMAGED_BULLSHOT);
				// V1 + U(V2 - V1) + V(V3 - V1)
				//if (intersected)
				//{
				//	//_vOut = v0 + (u * (v1 - v0)) + (v * (v2 - v0));
				//	_int iTemp = rand() % 100;
				//	_vOut = _vRayStart + (dist * _vRayDir * (1 - (iTemp * 0.0001f)));
				//}
				//else
				//{
				//	//_vOut = v0 + (u * (v2 - v1)) + (v * (v3 - v0));
				//	_int iTemp = rand() % 100;
				//	_vOut = _vRayStart + (dist * _vRayDir * (1 - (iTemp * 0.0001f)));
				//}

				_int iTemp = rand() % 100;
				_vOut = _vRayStart + (dist * _vRayDir * (1 - (iTemp * 0.001f)));
				return true;
			}
			//몸체피격
			dynamic_cast<CCharacter*>(pTargetObject)->Damaged(DAMAGED_STATE::DAMAGED_BODYSHOT, _fDamage);
			// V1 + U(V2 - V1) + V(V3 - V1)
			//if (intersected)
			//{
			//	//_vOut = v0 + (u * (v1 - v0)) + (v * (v2 - v0));
			//	_vOut = _vRayStart + (dist * _vRayDir);
			//}
			//else
			//{
			//	//_vOut = v0 + (u * (v2 - v1)) + (v * (v3 - v0));
			//	_vOut = _vRayStart + (dist * _vRayDir);
			//}

			_int iTemp = rand() % 100;
			_vOut = _vRayStart + (dist * _vRayDir * (1 - (iTemp * 0.001f)));

			return true;
		}
	}
	return false;
}

_float CCollisionManager::FloorRayCast(_vec3 vRayStart)
{
	auto mapFloor = CManagement::GetInstance()->Get_CurrScene()->Get_LayerObjects(L"Layer_Floor");

	for (auto Object : *mapFloor)
	{
		CGameObject* pGameObject = Object.second;
		CComponent* pComponent = pGameObject->Get_Component(COMPONENTID::ID_STATIC, L"Com_Buffer");
		CFloorTex* pFloorTex = static_cast<CFloorTex*>(pComponent);

		pComponent = pGameObject->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform");
		CTransform* pFloorTransform = static_cast<CTransform*>(pComponent);
		const _matrix* pFloorWorld = pFloorTransform->Get_WorldMatrix();

		_vec3 vVertex0 = *pFloorTex->Get_VertexPos(0);
		_vec3 vVertex1 = *pFloorTex->Get_VertexPos(1);
		_vec3 vVertex2 = *pFloorTex->Get_VertexPos(2);
		_vec3 vVertex3 = *pFloorTex->Get_VertexPos(3);

		D3DXVec3TransformCoord(&vVertex0, &vVertex0, pFloorWorld);
		D3DXVec3TransformCoord(&vVertex1, &vVertex1, pFloorWorld);
		D3DXVec3TransformCoord(&vVertex2, &vVertex2, pFloorWorld);
		D3DXVec3TransformCoord(&vVertex3, &vVertex3, pFloorWorld);

		float fU, fV, fDist1(0.f), fDist2(0.f);
		_vec3 vRayDir = { 0.f, -1.f, 0.f };
		_bool bIntersected1 = D3DXIntersectTri(&vVertex0, &vVertex1, &vVertex2, &vRayStart, &vRayDir, &fU, &fV, &fDist1);
		_bool bIntersected2 = D3DXIntersectTri(&vVertex0, &vVertex2, &vVertex3, &vRayStart, &vRayDir, &fU, &fV, &fDist2);

		if (bIntersected1)
			return fDist1;
		else if (bIntersected2)
			return fDist2;
		else
			continue;
	}

	return 0.f;
}

CGameObject* CCollisionManager::FloorRayCast2(_vec3 vRayStart)
{
	auto mapFloor = CManagement::GetInstance()->Get_CurrScene()->Get_LayerObjects(L"Layer_Floor");
	CGameObject* pGameObject;

	for (auto Object : *mapFloor)
	{
		pGameObject = Object.second;
		CComponent* pComponent = pGameObject->Get_Component(COMPONENTID::ID_STATIC, L"Com_Buffer");
		CFloorTex* pFloorTex = static_cast<CFloorTex*>(pComponent);

		pComponent = pGameObject->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform");
		CTransform* pFloorTransform = static_cast<CTransform*>(pComponent);
		const _matrix* pFloorWorld = pFloorTransform->Get_WorldMatrix();

		_vec3 vVertex0 = *pFloorTex->Get_VertexPos(0);
		_vec3 vVertex1 = *pFloorTex->Get_VertexPos(1);
		_vec3 vVertex2 = *pFloorTex->Get_VertexPos(2);
		_vec3 vVertex3 = *pFloorTex->Get_VertexPos(3);

		D3DXVec3TransformCoord(&vVertex0, &vVertex0, pFloorWorld);
		D3DXVec3TransformCoord(&vVertex1, &vVertex1, pFloorWorld);
		D3DXVec3TransformCoord(&vVertex2, &vVertex2, pFloorWorld);
		D3DXVec3TransformCoord(&vVertex3, &vVertex3, pFloorWorld);

		float fU, fV, fDist1(0.f), fDist2(0.f);
		_vec3 vRayDir = { 0.f, -1.f, 0.f };
		_bool bIntersected1 = D3DXIntersectTri(&vVertex0, &vVertex1, &vVertex2, &vRayStart, &vRayDir, &fU, &fV, &fDist1);
		_bool bIntersected2 = D3DXIntersectTri(&vVertex0, &vVertex2, &vVertex3, &vRayStart, &vRayDir, &fU, &fV, &fDist2);

		if (bIntersected1)
			return pGameObject;
		else if (bIntersected2)
			return pGameObject;
		else
			continue;
	}
}

CGameObject* CCollisionManager::RayCastWall(_vec3 vRayStart, _vec3 vRayDir, _vec3* _vPos)
{
	// wall 
	CGameObject* pReturn(nullptr);
	auto Objects = CManagement::GetInstance()->Get_CurrScene()->Get_LayerObjects(L"Layer_Wall");
	vector<CGameObject*> pHitObject;
	_float fMinDist(1001.f);

	for (auto pair : *Objects)
	{
		CGameObject* pTargetObject = pair.second; // wall game object
		if (!pTargetObject->Get_IsRender())
			continue;

		CWallTex* pWallbuffer(nullptr);
		CWallTBTex* pWallTBBuffer(nullptr);
		CTransform* pTargetTransform = static_cast<CTransform*>(pTargetObject->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform"));
		const _matrix* pTargetWolrd = pTargetTransform->Get_WorldMatrix();

		_vec3 vVertexPos[4];
		pWallbuffer = dynamic_cast<CWallTex*>(pTargetObject->Get_Component(COMPONENTID::ID_STATIC, L"Com_Buffer"));
		if (pWallbuffer)
		{
			//for (_uint i=0; i<4; ++i)
			//	vVertexPos[i] = pWallbuffer->Get_VtxPos()[i];
			vVertexPos[0] = { 0.f, 2.f, 0.f };
			vVertexPos[1] = { 0.f, 2.f, 2.f };
			vVertexPos[2] = { 0.f, 0.f, 2.f };
			vVertexPos[3] = { 0.f, 0.f, 0.f };
		}
		else
		{
			pWallTBBuffer = static_cast<CWallTBTex*>(pTargetObject->Get_Component(COMPONENTID::ID_STATIC, L"Com_Buffer"));
			//for (_uint i=0; i<4; ++i)
			//	vVertexPos[i] = pWallTBBuffer->Get_VtxPos()[i];

			vVertexPos[0] = { 0.f, 2.f, 0.f };
			vVertexPos[1] = { 2.f, 2.f, 0.f };
			vVertexPos[2] = { 2.f, 0.f, 0.f };
			vVertexPos[3] = { 0.f, 0.f, 0.f };
		}

		for (_uint i = 0; i < 4; ++i)
			D3DXVec3TransformCoord(&vVertexPos[i], &vVertexPos[i], pTargetWolrd);

		float fU, fV, fDist(0.f);
		_bool bIntersected(FALSE);
		if (D3DXIntersectTri(&vVertexPos[0], &vVertexPos[1], &vVertexPos[2], &vRayStart, &vRayDir, &fU, &fV, &fDist))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pReturn = pTargetObject;

				*_vPos = vVertexPos[0] + (fU * (vVertexPos[1] - vVertexPos[0])) + (fV * (vVertexPos[2] - vVertexPos[0]));
			}
		}

		else if (D3DXIntersectTri(&vVertexPos[0], &vVertexPos[2], &vVertexPos[3], &vRayStart, &vRayDir, &fU, &fV, &fDist))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pReturn = pTargetObject;

				*_vPos = vVertexPos[0] + (fU * (vVertexPos[2] - vVertexPos[1])) + (fV * (vVertexPos[3] - vVertexPos[0]));
			}

		}
		else
			continue;

	}


	if (fMinDist < 1000.f)
	{
		*_vPos = fMinDist * vRayDir + vRayStart;
		return pReturn;
	}


	// floor ray casting
	auto pFloor = CManagement::GetInstance()->Get_CurrScene()->Get_LayerObjects(L"Layer_Floor");

	for (auto pObject : *pFloor)
	{
		CGameObject* pGameObject = pObject.second;
		//if (!pGameObject->Get_IsRender())
		//	continue;

		CComponent* pComponent = pGameObject->Get_Component(COMPONENTID::ID_STATIC, L"Com_Buffer");
		CFloorTex* pFloorTex = static_cast<CFloorTex*>(pComponent);

		pComponent = pGameObject->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Transform");
		CTransform* pFloorTransform = static_cast<CTransform*>(pComponent);
		const _matrix* pFloorWorld = pFloorTransform->Get_WorldMatrix();

		//_vec3 vVertex0 = *pFloorTex->Get_VertexPos(0);
		//_vec3 vVertex1 = *pFloorTex->Get_VertexPos(1);
		//_vec3 vVertex2 = *pFloorTex->Get_VertexPos(2);
		//_vec3 vVertex3 = *pFloorTex->Get_VertexPos(3);

		_vec3 vVertex0 = { 0.f, 0.f, 1.f };
		_vec3 vVertex1 = { 1.f, 0.f, 1.f };
		_vec3 vVertex2 = { 1.f, 0.f, 0.f };
		_vec3 vVertex3 = { 0.f, 0.f, 0.f };



		D3DXVec3TransformCoord(&vVertex0, &vVertex0, pFloorWorld);
		D3DXVec3TransformCoord(&vVertex1, &vVertex1, pFloorWorld);
		D3DXVec3TransformCoord(&vVertex2, &vVertex2, pFloorWorld);
		D3DXVec3TransformCoord(&vVertex3, &vVertex3, pFloorWorld);

		float fU, fV, fDist(0.f);
		if (D3DXIntersectTri(&vVertex0, &vVertex1, &vVertex2, &vRayStart, &vRayDir, &fU, &fV, &fDist))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pReturn = pGameObject;
			}
		}

		else if (D3DXIntersectTri(&vVertex0, &vVertex2, &vVertex3, &vRayStart, &vRayDir, &fU, &fV, &fDist))
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pReturn = pGameObject;
			}
		}

	}

	if (fMinDist < 1000.f)
	{
		*_vPos = fMinDist * vRayDir + vRayStart;
		return pReturn;
	}

	return nullptr;
}


void CCollisionManager::Free()
{
	Reset();
}