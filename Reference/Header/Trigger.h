#pragma once
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CTrigger :
    public CBase
{
	DECLARE_SINGLETON(CTrigger)
private:
	explicit CTrigger();
	virtual ~CTrigger();

public:
	void Set_Trigger(_int _iTrigger, CGameObject* _pGameObject); //처음에 게임 Setting할때 Tile과 Monster의 트리거 숫자를 기반으로 Vector에다가 넣어주는 함수
	void Calculate_Trigger(); // Render 확인해주는 녀석
	void Collision_With_Trigger(const _int& _iTriggerNumber); // TriggerNumber를 건드려주는 함수,

private:
	vector<CGameObject*> m_vecTrigger[TRIGGERCOUNT];
	_int m_iCurTriggerNumber;
	_int m_iPreTriggerNumber;
	_int m_iPrePreTriggerNumber;
public:
	virtual void Free();
};

END