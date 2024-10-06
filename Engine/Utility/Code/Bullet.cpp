#include "../../Header/Bullet.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CGameObject(_pGraphicDev)
	, m_vDir({0.f, 0.f, 0.f})
	, m_fAttackDamage(0.f)
{
	m_bIsRender = false;

}

CBullet::CBullet(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vDir, const _float& _fAttackDamage)
	: CGameObject(_pGraphicDev)
	, m_vDir(_vDir)
	, m_fAttackDamage(_fAttackDamage)
{
}

CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_GameObject()
{
	return S_OK;
}

_int CBullet::Update_GameObject(const _float& _fTimeDelta)
{
	CGameObject::Update_GameObject(_fTimeDelta);
	return 0;
}

void CBullet::LateUpdate_GameObject()
{
	CGameObject::LateUpdate_GameObject();
}

void CBullet::Render_GameObject()
{

}

void CBullet::Fire_Bullet(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vDir, const _float& _fAttackDamage)
{
	
}

void CBullet::Fire_Missile(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vEndPos, const _float& _fAttackDamage, const _vec3& vCurvePos)
{
}

void CBullet::Free()
{
	CGameObject::Free();
}
