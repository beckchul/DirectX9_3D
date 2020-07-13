/*!
 * \file Shader.h
 * \date 2017/06/15 16:36
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

#ifndef Shader_h__
#define Shader_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader
	: public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pDevice);
	explicit CShader(const CShader& rhs);

public:
	virtual ~CShader(void);

public:
	LPD3DXEFFECT GetEffectHandle(void) {return m_pEffect;}

public:
	CShader* CloneShader(void);

public:
	HRESULT AddShaderFiles(const TCHAR* pFilePath);
	void BeginShader(const int& iPassIdx = 0);
	void EndShader(void);

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pDevice, const TCHAR* pFilePath);

public:
	virtual DWORD Release(void);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	LPD3DXEFFECT			m_pEffect;
	LPD3DXBUFFER			m_pErrBuffer;


};

END

#endif // Shader_h__