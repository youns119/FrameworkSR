#pragma once

#include "../../Base/Base.h"
#include "Engine_Define.h"
#include "GraphicDev.h"
#include "Define.h"
#include "Export_System.h"

class CMainApp 
	: public CBase
{
private :
	explicit CMainApp();
	virtual ~CMainApp();

public :
	static CMainApp* Create();

public :
	HRESULT	Ready_MainApp();
	int	Update_MainApp(const float& fTimeDelta);
	void LateUpdate_MainApp();
	void Render_MainApp();

private :
	virtual void Free();

private :
	bool SetUp();

private :
	CGraphicDev* m_pDeviceClass;
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	IDirect3DVertexBuffer9* m_pVB;
	IDirect3DIndexBuffer9* m_pIB;
};

struct Vertex
{
	Vertex() : m_fX(0), m_fY(0), m_fZ(0) {};
	Vertex(float _fX, float _fY, float _fZ)
	{
		m_fX = _fX;
		m_fY = _fY;
		m_fZ = _fZ;
	}

	float m_fX, m_fY, m_fZ;
	static const DWORD m_FVF;
};