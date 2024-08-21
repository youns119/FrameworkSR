#ifndef Engine_Typedef_h__
#define Engine_Typedef_h__

namespace Engine
{
	typedef	bool			_bool;

	typedef	signed char		_byte;
	typedef	unsigned char	_ubyte;

	typedef	wchar_t			_tchar;
	
	typedef	signed short	_short;
	typedef	unsigned short	_ushort;

	typedef	signed int		_int;
	typedef	unsigned int	_uint;

	typedef	signed long		_long;
	typedef	unsigned long	_ulong;

	typedef	float			_float;
	typedef	double			_double;

	typedef	D3DXVECTOR2		_vec2;
	typedef	D3DXVECTOR3		_vec3;
	typedef	D3DXVECTOR4		_vec4;

	typedef	D3DXMATRIX		_matrix;

	/*typedef	 struct tagMyVec3 : public D3DXVECTOR3
	{
		float	Get_Dot(const d3dxvector3& vTemp);
	
	};	*/
}

#endif // Engine_Typedef_h__