#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagVertexColor
	{
		_vec3 vPosition;			
		_ulong dwColor;
	} VTXCOL;

	const _ulong FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexTexture
	{
		_vec3 vPosition;
		_vec3 vNormal;
		_vec2 vTexUV;
	} VTXTEX;

	const _ulong FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTexture
	{
		_vec3 vPosition;
		_vec3 vTexUV;
	} VTXCUBE;

	const _ulong FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagIndex16
	{
		_ushort _0;
		_ushort _1;
		_ushort _2;
	} INDEX16;

	typedef struct tagIndex32
	{
		_ulong _0;
		_ulong _1;
		_ulong _2;
	} INDEX32;	
}

#endif // Engine_Struct_h__