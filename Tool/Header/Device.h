#pragma once
#include "Tool_Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9		Get_Device() { return m_pDevice; }
	LPD3DXSPRITE			Get_Sprite() { return m_pSprite; }
	LPD3DXFONT				Get_Font() { return m_pFont; }
public:
	HRESULT			Init_Device();
	void			Render_Begin();
	void			Render_End(HWND hWnd = nullptr);
	void			Release();

private:
	void			Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	LPDIRECT3D9				m_pSDK;		// 그래픽 카드의 성능을 조사하는 객체
	LPDIRECT3DDEVICE9		m_pDevice;	// 그리기 관련 객체
	LPD3DXSPRITE			m_pSprite;	// dx 상에서 2d 이미지 출력 담당 객체
	LPD3DXFONT				m_pFont;	// 폰트 객체
};