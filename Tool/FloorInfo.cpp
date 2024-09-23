#include "pch.h"
#include "FloorInfo.h"
CFloorInfo::CFloorInfo(LPDIRECT3DDEVICE9 _pGraphicDev)
    : Engine::CGameObject(_pGraphicDev)
    , m_pBufferCom(nullptr)
    , m_pTransformCom(nullptr)
    , m_pTextureCom(nullptr)
{
}

CFloorInfo::~CFloorInfo()
{
}
