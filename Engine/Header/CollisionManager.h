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
	_bool Get_ColliderRender() const { return m_bRender; }

public:
	void Update_Collision();
	void Render_Collider();

private:
	void CollisionGroupUpdate(const _tchar* _pLeft, const _tchar* _pRight);
	_bool isCollision(CCollider* _pLCol, CCollider* _pRCol);
	_bool isAABB(CCollider* _pLCol, CCollider* _pRCol);

public:
	void Toggle_Collider() { m_bRender = !m_bRender; }
	void Add_Collider(CCollider* _pCollider);
	void CheckGroup(const _tchar* _pLeft, const _tchar* _pRight);
	void Clear_Collider();
	void Reset();

public:
	vector<CGameObject*> RayCast(_vec3 vRayStart, _vec3 vRayDir);
	_bool RayCast2(_vec3 vRayStart, _vec3 vRayDir);
	_bool FireRayCast(_vec3 _vRayStart, _vec3 _vRayDir, _vec3& _vOut);
	_float FloorRayCast(_vec3 vRayStart);

public:
	virtual void Free();

private:
	map<ULONGLONG, _bool> m_mapColInfo;
	vector<pair<const _tchar*, const _tchar*>> m_vecCheck;

	list<CCollider*> m_listRender;

	_bool m_bRender;
};

END