#pragma once
#include "Soda.h"

BEGIN(Engine)

class ENGINE_DLL CDrinkSpawner :
    public CBase
{
    DECLARE_SINGLETON(CDrinkSpawner)
private:
    explicit CDrinkSpawner();
    virtual ~CDrinkSpawner();

public:
    void Set_DrinkObject(CSoda* _pGameObject);
    void Spawn_DrinkObject(_vec3 _vStartPos);

private:
    vector<CSoda*> m_vecDrinkObject;

public:
    virtual void Free();

};

END