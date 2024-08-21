#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

namespace Engine
{
	template<typename T>
	void Safe_Delete(T& Pointer)
	{
		if (Pointer != nullptr)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& Pointer)
	{
		if (Pointer != nullptr)
		{
			delete[] Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long dwRefCnt = 0;

		if (pInstance != nullptr)
		{
			dwRefCnt = pInstance->Release();

			if (dwRefCnt == 0)
				pInstance = NULL;
		}

		return dwRefCnt;
	}

	class CTag_Finder
	{
	public :
		explicit CTag_Finder(const _tchar* pTag) : m_pTargetTag(pTag) {}
		~CTag_Finder(void) {}

	public :
		template<typename T> 
		_bool operator () (const T& pair)
		{
			if (lstrcmpW(m_pTargetTag, pair.first) == 0)
				return true;
			
			return false;
		}

	private :
		const _tchar* m_pTargetTag = nullptr;
	};

	class CDeleteObj
	{
	public :
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}

	public :
		template <typename T>
		void operator () (T& pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = pInstance->Release();

			if (dwRefCnt == 0)
				pInstance = nullptr;
		}
	};

	class CDeleteMap
	{
	public :
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}

	public :	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (dwRefCnt == 0)
				Pair.second = NULL;
		}
	};
}

#endif // Engine_Function_h__