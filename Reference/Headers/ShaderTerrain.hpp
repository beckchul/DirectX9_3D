#include "ShaderBase.hpp"

vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
float		g_fPower = 20.f;

vector		g_vCamPos;
float		g_fDetail = 1.f;

vector		g_vBrushPos;
float		g_fBrushSize;


//texture		g_BaseTexture;
texture		g_SrcTexture;
sampler SrcSampler = sampler_state
{
	texture = g_SrcTexture;
minfilter = linear;
MAGFILTER = LINEAR;
MIPFILTER = LINEAR;
};

texture		g_DestTexture;
sampler DestSampler = sampler_state
{
	texture = g_DestTexture;
minfilter = linear;
MAGFILTER = LINEAR;
MIPFILTER = LINEAR;
};

texture		g_MultiTexture;
sampler MultiSampler = sampler_state
{
	texture = g_MultiTexture;
minfilter = linear;
MAGFILTER = LINEAR;
MIPFILTER = LINEAR;

};

texture		g_BrushTexture;
sampler BrushSampler = sampler_state
{
	texture = g_BrushTexture;
};

struct VS_IN
{
	vector		vPos : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPos : POSITION;
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	float2		vTexUV : TEXCOORD0;
	vector		vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);
	Out.vPos = mul(vector(In.vPos.xyz, 1.f), matWVP);

	Out.vTexUV = In.vTexUV;

	vector		vLightDirInv = normalize(g_vLightDir) * -1.f;
	vector		vWorldNormal = mul(vector(In.vNormal.xyz, 0.f), g_matWorld);

	Out.vShade = max(dot(vLightDirInv, vWorldNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	vector		vReflect = reflect(normalize(g_vLightDir), vWorldNormal);

	vector		vWorldPos = mul(vector(In.vPos.xyz, 1.f), g_matWorld);
	Out.vWorldPos = vWorldPos;
	vector		vAt = vWorldPos - g_vCamPos;

	Out.vSpecular = pow(max(dot(normalize(vAt * -1.f), normalize(vReflect)), 0.f), g_fPower) * (g_vLightSpecular * g_vMtrlSpecular);
	return Out;
}

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
	vector		vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	//Out.vColor = tex2D(BaseSampler, In.vTexUV * g_fDetail);
	vector		vColor[4];
	vColor[0] = tex2D(SrcSampler, In.vTexUV * g_fDetail);
	vColor[1] = tex2D(DestSampler, In.vTexUV * g_fDetail);
	vColor[2] = tex2D(MultiSampler, In.vTexUV);
	//vColor[3] = tex2D(BrushSampler, In.vTexUV);
	vColor[3] = (vector)0.f;

	//	30-10 < 30	|		|30 < 30 + 10
	if (g_vBrushPos.x - g_fBrushSize < In.vWorldPos.x
		&& In.vWorldPos.x < g_vBrushPos.x + g_fBrushSize
		&& g_vBrushPos.z - g_fBrushSize < In.vWorldPos.z
		&& In.vWorldPos.z < g_vBrushPos.z + g_fBrushSize)
	{
		float2		vNewUV = (float2)0.f;

		//0.5 = (30 - (30 - 10)) / (10 * 2);
		vNewUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fBrushSize)) / (g_fBrushSize * 2.f);
		vNewUV.y = ((g_vBrushPos.z + g_fBrushSize) - In.vWorldPos.z) / (g_fBrushSize * 2.f);

		vColor[3] = tex2D(BrushSampler, vNewUV);
	}

	vector		vResultColor = (vector)0.f;
	vResultColor = vColor[0] * vColor[2] + vColor[1] * (1.f - vColor[2]);

	Out.vColor = vResultColor * In.vShade + In.vSpecular + vColor[3];
	//Out.vColor = Out.vColor * In.vShade + In.vSpecular;
	return Out;
}

technique Default_Technique
{
	pass Default
{
	/*LPDIRECT3DDEVICE9		pDevice;
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);*/

	/*ALPHABLENDENABLE = TRUE;
	DESTBLEND = INVSRCALPHA;
	SRCBLEND = SRCALPHA;
	*/

	VertexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN();
}

/*pass asdf
{
VetexShader = compile vs_3_0 VS_MAIN();
PixelShader = compile ps_3_0 PS_MAIN();
}*/
};

/*
technique Default_Technique
{
pass Default
{
VetexShader = compile vs_2_0 VS_MAIN();
PixelShader = compile ps_2_0 PS_MAIN();
}
};
*/