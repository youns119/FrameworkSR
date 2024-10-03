#include "BulletManager.h"

IMPLEMENT_SINGLETON(CBulletManager)

CBulletManager::CBulletManager()
{
	m_vecBullet.reserve(100);
}

CBulletManager::~CBulletManager()
{
	Free();
}


HRESULT CBulletManager::Add_Bullet(CBullet* _pBullet)
{
	NULL_CHECK_RETURN(_pBullet, E_FAIL);

	m_vecBullet.push_back(_pBullet);

	return S_OK;
}

HRESULT CBulletManager::Add_Missile(CBullet* _pMissile)
{
	NULL_CHECK_RETURN(_pMissile, E_FAIL);

	m_vecMissile.push_back(_pMissile);

	return S_OK;
}
HRESULT CBulletManager::Fire_Bullet(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vDir, const _float& _fAttackDamage, CBulletManager::BULLETTYPE _eBulletType, const _vec3& vCurvePos)
{
	_int iTemp(0);
	_int iSour(0);
	switch (_eBulletType)
	{
	case Engine::CBulletManager::BULLET_PISTOL:
		for (auto& iter : m_vecBullet)
		{
			if (!(iter->Get_IsRender()))
			{
				iter->Fire_Bullet(_pGraphicDev, _vStartPos, _vDir, _fAttackDamage);
				return S_OK;
			}
		}
		break;
	case Engine::CBulletManager::BULLET_SHOTGUN:
		for (auto& iter : m_vecBullet)
		{
			if (!(iter->Get_IsRender()))
			{
				iSour = _int(rand() % 4);
				_vec3 vTemp;
				switch (iSour)
				{
				case 0 :
					vTemp = { _float(rand() % 2) * (-0.1f), _float(rand() % 2) * 0.1f, 0.f };
					break;
				case 1:
					vTemp = { _float(rand() % 2) * 0.1f, _float(rand() % 2) * (-0.1f), 0.f};
					break;
				case 2:
					vTemp = { _float(rand() % 2) * (-0.1f), _float(rand() % 2) * (-0.1f), 0.f};
					break;
				case 3: 
					vTemp = { _float(rand() % 2) * 0.1f, _float(rand() % 2) * 0.1f, 0.f };
					break;
				}
				iter->Fire_Bullet(_pGraphicDev, _vStartPos, _vDir + vTemp, _fAttackDamage);
				iTemp++;
			}
			if (5 < iTemp)
				return S_OK;
		}
		break;
	case Engine::CBulletManager::BULLET_LASER:
		break;
	case Engine::CBulletManager::BULLET_MISSILE:
		for (auto& iter : m_vecMissile)
		{
			if (!(iter->Get_IsRender()))
			{
				iter->Fire_Missile(_pGraphicDev, _vStartPos, _vDir, _fAttackDamage, vCurvePos);
				return S_OK;
				break;
			}
		}
    }
	return E_FAIL;
}

_int CBulletManager::Update_Bullet(const _float& _fTimeDelta)
{
	for (auto& iter : m_vecBullet)
		iter->Update_GameObject(_fTimeDelta);
	for (auto& iter : m_vecMissile)
		iter->Update_GameObject(_fTimeDelta);
	return 0;
}

void CBulletManager::LateUpdate_Bullet()
{
	for (auto& iter : m_vecBullet)
		iter->LateUpdate_GameObject();
	for (auto& iter : m_vecMissile)
		iter->LateUpdate_GameObject();
}


void CBulletManager::Free()
{
	//for_each(m_vecBullet.begin(), m_vecBullet.end(), CDeleteObj());
	m_vecBullet.clear();
	m_vecMissile.clear();
}
