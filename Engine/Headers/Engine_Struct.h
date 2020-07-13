/*!
 * \file Engine_Struct.h
 * \date 2015/04/04 23:03
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
#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagMeshContainer_Derived : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*			ppTexture;
		LPD3DXMESH					pOriMesh;
		D3DXMATRIX*					pFrameOffsetMatrix;
		D3DXMATRIX**				ppFrameCombinedMatrix;
		D3DXMATRIX*					pResultMatrix;
		vector<WCHAR*>				vecTagName;
		//LPD3DXBUFFER				pBoneCombinationBuf;	// 셰이더에게 행렬 팔레트를 넘겨주기 위한 본 컴비네이션 테이블 변수
	}DERIVED_MESHCONTAINER;

	//struct D3DXBONECOMBINATION {
	//	DWORD	AttribId;
	//	DWORD	FaceStart;
	//	DWORD	FaceCount;
	//	DWORD	VertexStart;
	//	DWORD	VertexCount;
	//	DWORD*	BoneId;
	//};

	typedef struct tagFrame_Derived : public D3DXFRAME
	{
		D3DXMATRIX		CombinedMatrix;
	}DERIVED_FRAME;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3				vPos;
		D3DXVECTOR3				vNormal;
		D3DXVECTOR2				vTexUV;
	}VTXTEX;
	const DWORD VTXFVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCube
	{
		D3DXVECTOR3			vPos;
		D3DXVECTOR3			vTex;
	}VTXCUBE;
	const DWORD VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEXCOORDSIZE3(0) | D3DFVF_TEX1;

	typedef struct tagVertexColor
	{
		D3DXVECTOR3		vPos;
		DWORD			dwColor;
	}VTXCOL;
	const DWORD VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagIndex16
	{
		WORD			_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		DWORD			_1, _2, _3;
	}INDEX32;


	typedef struct tagAnimationInfo
	{
		UINT	iAniIndex;
		float	fAniSpeedFront;
		float	fAniSpeedEnd;
		float	fAniWeightFront;
		float	fAniWeightEnd;
		double	dMagicNumber;
		double	dEndAniCount;
	}ANI_INFO;
}

#endif // Engine_Struct_h__