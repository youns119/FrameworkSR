#include "Soda.h"

CSoda::CSoda(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
{
}

CSoda::~CSoda()
{
}

void CSoda::Free()
{
	CGameObject::Free();
}
