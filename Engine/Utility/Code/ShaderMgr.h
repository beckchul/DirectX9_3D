/*!
 * \file ShaderMgr.h
 * \date 2017/06/15 16:54
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef ShaderMgr_h__
#define ShaderMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CShader;

class ENGINE_DLL CShaderMgr
{
public:
	DECLARE_SINGLETON(CShaderMgr)

private:
	CShaderMgr(void);
	~CShaderMgr(void);

public:
	HRESULT AddShaderFile(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pShaderKey
		, const TCHAR* pFilePath);
	CShader* CloneShader(const TCHAR* pShaderKey);

private:
	CShader* FindShader(const TCHAR* pShaderKey);

public:
	void Release(void);

private:
	typedef map<const TCHAR*, CShader*>		MAPSHADER;
	MAPSHADER		m_mapShader;
};

END

#endif // ShaderMgr_h__