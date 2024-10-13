#include "BulletManager.h"

IMPLEMENT_SINGLETON(CBulletManager)

CBulletManager::CBulletManager()
{
	m_vecBullet.reserve(100);
	m_vecMissile.reserve(100);
	m_vecLaser.reserve(100);
	m_vecMiniGun.reserve(256);
	m_vecHead.reserve(16);
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

HRESULT CBulletManager::Add_Laser(CBullet* _pLaser)
{
	NULL_CHECK_RETURN(_pLaser, E_FAIL);

	m_vecLaser.push_back(_pLaser);

	return S_OK;
}

HRESULT CBulletManager::Add_MiniGun(CBullet* _pMiniGun)
{
	NULL_CHECK_RETURN(_pMiniGun, E_FAIL);

	m_vecMiniGun.push_back(_pMiniGun);

	return S_OK;
}

HRESULT CBulletManager::Add_HumanoidHead(CBullet* _pHead)
{
	NULL_CHECK_RETURN(_pHead, E_FAIL);

	m_vecHead.push_back(_pHead);

	return S_OK;
}

HRESULT CBulletManager::Add_Boss_Humanoid_Laser(CBullet* _pBossLaser)
{
	NULL_CHECK_RETURN(_pBossLaser, E_FAIL);
	m_vecBoss_Humanoid_Laser.push_back(_pBossLaser);
	return S_OK;
}

HRESULT CBulletManager::Fire_Bullet(LPDIRECT3DDEVICE9 _pGraphicDev, const _vec3& _vStartPos, const _vec3& _vDir, const _float& _fAttackDamage, CBulletManager::BULLETTYPE _eBulletType, const _bool& _bIsBoss, const _vec3& vCurvePos)
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
				iter->Fire_Bullet(_pGraphicDev, _vStartPos, _vDir, _fAttackDamage, _bIsBoss);
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
				iter->Fire_Bullet(_pGraphicDev, _vStartPos, _vDir + vTemp, _fAttackDamage, _bIsBoss);
				iTemp++;
			}
			if (2 < iTemp)
				return S_OK;
		}
		break;
	case Engine::CBulletManager::BULLET_LASER:
		for (auto& iter : m_vecLaser)
		{
			if (!(iter->Get_IsRender()))
			{
				iter->Fire_Laser(_pGraphicDev, _vStartPos, _vDir, _fAttackDamage);
				return S_OK;
				break;
			}
		}
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
		break;
	case Engine::CBulletManager::BULLET_MINIGUN:
		for (auto& iter : m_vecMiniGun)
		{
			if (!(iter->Get_IsRender()))
			{
				iter->Fire_MiniGun(_pGraphicDev, _vStartPos, _vDir, _fAttackDamage);
				return S_OK;
				break;
			}
		}
		break;
	case Engine::CBulletManager::BULLET_HEAD:
		for (auto& iter : m_vecHead)
		{
			if (!(iter->Get_IsRender()))
			{
				iter->Fire_Head(_pGraphicDev, _vStartPos, _vDir, _fAttackDamage, vCurvePos);
				return S_OK;
				break;
			}
		}
		break;
	case BULLET_BOSS_HUMANOID_LASER:
		for (auto& iter : m_vecBoss_Humanoid_Laser)
		{
			if (!(iter->Get_IsRender()))
			{
				iter->Boss_Sniper_Laser(_pGraphicDev, _vStartPos, _vDir);
				return S_OK;
				break;
			}
		}
		break;
    }
	return E_FAIL;
}


_float CBulletManager::Get_Bullet_Linear(CBulletManager::BULLETTYPE _eBulletType)
{
	switch (_eBulletType) {
	case BULLET_LASER:
		for (auto& iter : m_vecLaser)
		{
			if ((iter->Get_IsRender()))
			{
				return iter->Get_Linear();
				break;
			}
		}
		break;
	case BULLET_MISSILE:
		for (auto& iter : m_vecMissile)
		{
			if ((iter->Get_IsRender()))
			{
				return iter->Get_Linear();
				break;
			}
		}
		break;
	case BULLET_BOSS_HUMANOID_LASER:
		for (auto& iter : m_vecBoss_Humanoid_Laser)
		{
			if ((iter->Get_IsRender()))
			{
				return iter->Get_Linear();
				break;
			}
		}
		break;
	}
	return 0.f;
}


_int CBulletManager::Update_Bullet(const _float& _fTimeDelta)
{
	for (auto& iter : m_vecBullet)
		iter->Update_GameObject(_fTimeDelta);
	for (auto& iter : m_vecMissile)
		iter->Update_GameObject(_fTimeDelta);
	for (auto& iter : m_vecLaser)
		iter->Update_GameObject(_fTimeDelta);
	for (auto& iter : m_vecMiniGun)
		iter->Update_GameObject(_fTimeDelta);
	for (auto& iter : m_vecHead)
		iter->Update_GameObject(_fTimeDelta);
	for (auto& iter : m_vecBoss_Humanoid_Laser)
		iter->Update_GameObject(_fTimeDelta);
	return 0;
}

void CBulletManager::LateUpdate_Bullet()
{
	for (auto& iter : m_vecBullet)
		iter->LateUpdate_GameObject();
	for (auto& iter : m_vecMissile)
		iter->LateUpdate_GameObject();
	for (auto& iter : m_vecLaser)
		iter->LateUpdate_GameObject();
	for (auto& iter : m_vecMiniGun)
		iter->LateUpdate_GameObject();
	for (auto& iter : m_vecHead)
		iter->LateUpdate_GameObject();
	for (auto& iter : m_vecBoss_Humanoid_Laser)
		iter->LateUpdate_GameObject();
}


void CBulletManager::Free()
{
	//for_each(m_vecBullet.begin(), m_vecBullet.end(), CDeleteObj());
	m_vecBullet.clear();
	m_vecMissile.clear();
	m_vecLaser.clear();
	m_vecMiniGun.clear();
	m_vecHead.clear();
	m_vecBoss_Humanoid_Laser.clear();
}
