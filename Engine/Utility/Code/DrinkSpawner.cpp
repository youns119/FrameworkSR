#include "DrinkSpawner.h"

IMPLEMENT_SINGLETON(CDrinkSpawner)

CDrinkSpawner::CDrinkSpawner()
{
	m_vecDrinkObject.reserve(64);
}

CDrinkSpawner::~CDrinkSpawner()
{
	Free();
}

void CDrinkSpawner::Set_DrinkObject(CSoda* _pGameObject)
{
	m_vecDrinkObject.push_back(_pGameObject);
}

void CDrinkSpawner::Spawn_DrinkObject(_vec3 _vStartPos)
{
	for (auto& iter : m_vecDrinkObject)
	{
		if (!(iter->Get_IsRender()))
		{
			iter->Spawn_Soda(_vStartPos);
			iter->Set_IsRender(true);
			break;
		}
	}
}

void CDrinkSpawner::Free()
{
	m_vecDrinkObject.clear();
}
