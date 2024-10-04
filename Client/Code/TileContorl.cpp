#include "pch.h"

#include "../Header/TileContorl.h"

CTileContorl::CTileContorl(LPDIRECT3DDEVICE9 _pGraphicDev)
    : Engine::CGameObject(_pGraphicDev)
    , m_pTransformCom(nullptr)
    , m_pTextureCom(nullptr)
    , m_vecPos({0.f, 0.f, 0.f})
    , m_iNumber(0)
    , m_iNumber_Type(0)
{
}

CTileContorl::~CTileContorl()
{
}


void CTileContorl::Free()
{
    Engine::CGameObject::Free();
}
