#pragma once

#include "Base.h"
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionManager
	: public CBase
{
	DECLARE_SINGLETON(CCollisionManager)

private:
	explicit CCollisionManager();
	virtual ~CCollisionManager();

public:
	void Update_Collision();

private:
	void CollisionGroupUpdate();
	bool isCollision(CCollider* _pLCol, CCollider* _pRCol);

public:
	void CheckGroup();
	void Reset();

public:
	virtual void Free();

private:
	map <ULONGLONG, bool> m_mapColInfo;
};

END