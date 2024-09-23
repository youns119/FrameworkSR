#include "pch.h"
#include "WallInfo.h"

CWallInfo::CWallInfo(LPDIRECT3DDEVICE9 _pGraphicDev)
    : Engine::CGameObject(_pGraphicDev)
    , m_pBufferCom(nullptr)
    , m_pTransformCom(nullptr)
    , m_pTextureCom(nullptr)
{
}

CWallInfo::~CWallInfo()
{
}
