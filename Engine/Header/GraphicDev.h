#pragma once

#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev 
	: public CBase
{
	DECLARE_SINGLETON(CGraphicDev)

private :
	explicit CGraphicDev();
	virtual ~CGraphicDev();

public :
	LPDIRECT3DDEVICE9 Get_GraphicDev() { return m_pGraphicDev; }

public :
	HRESULT Ready_GraphicDev
	(
		HWND _hWnd,
		WINMODE _eMode,
		const _uint& _iSizeX,
		const _uint& _iSizeY,
		CGraphicDev** _ppGraphicDev
	);

	void Render_Begin(D3DXCOLOR _tColor);
	void Render_End();

public :
	virtual void Free();

private :
	LPDIRECT3D9 m_pSDK;
	LPDIRECT3DDEVICE9 m_pGraphicDev;
};

END