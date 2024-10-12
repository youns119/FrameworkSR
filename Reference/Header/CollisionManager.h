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
	_bool RayCast(_vec3 vRayStart, _vec3 vRayDir);
	_bool RayCast2(_vec3 vRayStart, _vec3 vRayDir);
	_bool FireRayCast(_vec3 _vRayStart, _vec3 _vRayDir, _vec3& _vOut, const _float& _fDamage);
	_float FloorRayCast(_vec3 vRayStart);

	CGameObject* FloorRayCast2(_vec3 vRayStart);//산성, 용암 바닥 타일 정보 가져오기 위해 만듬

	CGameObject* RayCastWall(_vec3 vRayStart, _vec3 vRayDir, _vec3* _vPos); // 벽 스파크 튀기는거

public:
	virtual void Free();

private:
	map<ULONGLONG, _bool> m_mapColInfo;
	vector<pair<const _tchar*, const _tchar*>> m_vecCheck;

	list<CCollider*> m_listRender;

	_bool m_bRender;
};

END