#include "..\..\Header\Animator.h"
#include "Export_Utility.h"

CAnimator::CAnimator()
	: m_pCurrAnimation(nullptr)
	, m_bAnimRepeat(false)
	, m_bAnimPause(false)
{
	m_mapAnimation.clear();
}

CAnimator::CAnimator(LPDIRECT3DDEVICE9 _pGraphicDev)
	: CComponent(_pGraphicDev)
	, m_pCurrAnimation(nullptr)
	, m_bAnimRepeat(false)
	, m_bAnimPause(false)
{
	m_mapAnimation.clear();
}

CAnimator::CAnimator(const CAnimator& _rhs)
	: CComponent(_rhs)
	, m_pCurrAnimation(_rhs.m_pCurrAnimation)
	, m_bAnimRepeat(_rhs.m_bAnimRepeat)
	, m_bAnimPause(_rhs.m_bAnimPause)
{
	m_mapAnimation.clear();
}

CAnimator::~CAnimator()
{
}

CAnimator* CAnimator::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{
	CAnimator* pAnimator = new CAnimator(_pGraphicDev);

	if (FAILED(pAnimator->Ready_Animator()))
	{
		Safe_Release(pAnimator);
		return nullptr;
	}

	return pAnimator;
}

HRESULT CAnimator::Ready_Animator()
{
	return S_OK;
}

_int CAnimator::Update_Component(const _float& _fTimeDelta)
{
	if (m_bAnimPause)
		return 0;

	if (m_pCurrAnimation != nullptr)
	{
		m_pCurrAnimation->Update_Animation(_fTimeDelta);

		if (m_bAnimRepeat && m_pCurrAnimation->GetFinish())
			m_pCurrAnimation->ResetAnimFrame(0.f);
		else if (!m_bAnimRepeat && m_pCurrAnimation->GetFinish())
			m_pCurrAnimation->SetFinish(true);
	}

	return 0;
}

void CAnimator::Render_Animator()
{
	m_pCurrAnimation->GetTexture()->Set_Texture((_uint)m_pCurrAnimation->GetCurrFrame());
}

void CAnimator::CreateAnimation(const _tchar* _strName, CTexture* _pTexture, float _fAnimSpeed)
{
	CAnimation* pAnimation = FindAnimation(_strName);

	if (pAnimation == nullptr)
	{
		pAnimation = CAnimation::Create(m_pGraphicDev, _pTexture, _fAnimSpeed);
		m_mapAnimation.insert(make_pair(_strName, pAnimation));
	}
}

CAnimation* CAnimator::FindAnimation(const _tchar* _strName)
{
	auto iter = find_if(m_mapAnimation.begin(), m_mapAnimation.end(), CTag_Finder(_strName));

	if (iter == m_mapAnimation.end())
		return nullptr;

	return iter->second;
}

void CAnimator::PlayAnimation(const _tchar* _strName, bool _bAnimRepeat)
{
	m_pCurrAnimation = FindAnimation(_strName);
	m_bAnimRepeat = _bAnimRepeat;

	m_pCurrAnimation->ResetAnimFrame(0.f);
}

CComponent* CAnimator::Clone()
{
	return new CAnimator(*this);
}

void CAnimator::Free()
{
	for_each(m_mapAnimation.begin(), m_mapAnimation.end(), CDeleteMap());
	m_mapAnimation.clear();

	CComponent::Free();
}