#include "../../Header/Trigger.h"

IMPLEMENT_SINGLETON(CTrigger)

CTrigger::CTrigger()
	: m_iCurTriggerNumber(1)
	, m_iPreTriggerNumber(0)
	, m_iPrePreTriggerNumber(0)
{
	for (_int i = 0; i < TRIGGERCOUNT; ++i)
		m_vecTrigger[i].reserve(256);
}

CTrigger::~CTrigger()
{
	Free();
}


void CTrigger::Set_Trigger(_int _iTrigger, CGameObject* _pGameObject)
{
	m_vecTrigger[_iTrigger].push_back(_pGameObject);
}

void CTrigger::Calculate_Trigger()
{
	for (_int i = 0; i < TRIGGERCOUNT; ++i)
		for (auto& iter : m_vecTrigger[i])
			iter->Set_IsRender(false);

	for (auto& iter : m_vecTrigger[m_iPrePreTriggerNumber])
		iter->Set_IsRender(true);

	for (auto& iter : m_vecTrigger[m_iPreTriggerNumber])
		iter->Set_IsRender(true);

	for (auto& iter : m_vecTrigger[m_iCurTriggerNumber])
		iter->Set_IsRender(true);
}

void CTrigger::Collision_With_Trigger(const _int& _iTriggerNumber)
{
	if (0 != _iTriggerNumber)
		m_iPrePreTriggerNumber = _iTriggerNumber - 1;

	m_iPreTriggerNumber = _iTriggerNumber;
	m_iCurTriggerNumber = _iTriggerNumber + 1;
}

void CTrigger::Free()
{
	for (_int i = 0; i < TRIGGERCOUNT; ++i)
		m_vecTrigger[i].clear();
}
