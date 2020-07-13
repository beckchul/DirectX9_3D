
#include "ShaderBase.hpp"

texture		g_BaseTexture;

float		g_fAlpha = 1.f;
float		g_fDetail = 1.f;

sampler BaseSampler = sampler_state
{
	/*
	LPDIRECT3DDEVICE9		pDevice;
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	*/
	texture = g_BaseTexture;
minfilter = linear;
MAGFILTER = LINEAR;
MIPFILTER = LINEAR;
};

struct VS_IN
{
	vector		vPos : POSITION;
	//vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector		vPos : POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	Matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPos = mul(vector(In.vPos.xyz, 1.f), matWVP);

	Out.vTexUV = In.vTexUV;
	return Out;
}

struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	vector		vColor[2];
	vColor[0] = tex2D(BaseSampler, In.vTexUV * g_fDetail);
	vColor[1] = tex2D(BaseSampler, In.vTexUV * g_fDetail);
	//Out.vColor = tex2D(BaseSampler, In.vTexUV * g_fDetail);

	if (In.vTexUV.x * g_fDetail <= 1.f)
		vColor[0].gb = 0.f;
	else if (In.vTexUV.x * g_fDetail <= 2.f)
		vColor[0].rb = 0.f;
	else
		vColor[0].rg = 0.f;

	if (In.vTexUV.y * g_fDetail <= 1.f)
		vColor[1].gb = 0.f;
	else if (In.vTexUV.y * g_fDetail <= 2.f)
		vColor[1].rb = 0.f;
	else
		vColor[1].rg = 0.f;

	Out.vColor = vColor[0] + vColor[1];

	Out.vColor.a = g_fAlpha;

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

	ALPHABLENDENABLE = TRUE;
DESTBLEND = INVSRCALPHA;
SRCBLEND = SRCALPHA;

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