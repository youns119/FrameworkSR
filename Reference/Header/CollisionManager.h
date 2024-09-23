#pragma once

#include "Base.h"
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionManager
	: public CBase
{
	DECLARE_SINGLETON(CCollisionManager)

public:
	union COLLIDER_ID
	{
		struct
		{
			UINT iLeft_id;
			UINT iRight_id;
		};

		ULONGLONG ullID;
	};

private:
	explicit CCollisionManager();
	virtual ~CCollisionManager();

public:
	void Update_Collision();

private:
	void CollisionGroupUpdate(const _tchar* _pLeft, const _tchar* _pRight);
	_bool isCollision(CCollider* _pLCol, CCollider* _pRCol);

public:
	void CheckGroup(const _tchar* _pLeft, const _tchar* _pRight);
	void Reset();

public:
	virtual void Free();

private:
	map<ULONGLONG, _bool> m_mapColInfo;
	vector<pair<const _tchar*, const _tchar*>> m_vecCheck;
};

END