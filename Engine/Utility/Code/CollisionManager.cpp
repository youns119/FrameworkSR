#include "..\..\Header\CollisionManager.h"

IMPLEMENT_SINGLETON(CCollisionManager)

CCollisionManager::CCollisionManager()
{
}

CCollisionManager::~CCollisionManager()
{
	Free();
}

void CCollisionManager::Update_Collision()
{
}

void CCollisionManager::CollisionGroupUpdate()
{
}

bool CCollisionManager::isCollision(CCollider* _pLCol, CCollider* _pRCol)
{
	_float fX = abs(_pLCol->GetFinalPos().x - _pRCol->GetFinalPos().x);
	_float fY = abs(_pLCol->GetFinalPos().y - _pRCol->GetFinalPos().y);
	_float fZ = abs(_pLCol->GetFinalPos().z - _pRCol->GetFinalPos().z);

	_float fDiagonal = sqrt(fX * fX + fY * fY + fZ * fZ);

	_float fRadius = (_pLCol->GetRadius() + _pRCol->GetRadius()) * 0.5f;

	return fRadius >= fDiagonal;
}

void CCollisionManager::CheckGroup()
{
}

void CCollisionManager::Reset()
{
}

void CCollisionManager::Free()
{
}