#include "ShaderBase.hpp"

vector gWorldLightPosition;
vector gWorldCameraPosition;

float3 gLightColor;

texture g_DiffuseMap;
texture g_NormalMap;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseMap;
	minfilter = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

sampler NormalSampler = sampler_state
{
	texture = g_NormalMap;
	minfilter = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

struct VS_IN_Default
{
	vector mPosition : POSITION;
	float3 mNormal : NORMAL;
	float3 mTangent : TANGENT;
	float3 mBinormal : BINORMAL;
	float2 mUV : TEXCOORD0;
};
struct VS_OUT_Default
{
	vector mPosition : POSITION;
	float2 m_UV : TEXCOORD0;
	float3 mLightDir : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

struct VS_IN_NotNormal
{
	vector mPosition : POSITION;
	float3 mNormal : NORMAL;
	float2 mUV : TEXCOORD0;
};

struct VS_OUT_NotNormal
{
	vector mPosition : POSITION;
	float2 m_UV : TEXCOORD0;
	float3 mDiffuse : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
	float3 mReflection : TEXCOORD3;
};

VS_OUT_Default VS_MAIN_Defalut(VS_IN_Default Input)
{
	VS_OUT_Default Output;

	Output.mPosition = mul(Input.mPosition, g_matWorld);
	Output.mPosition = mul(Output.mPosition, g_matView);
	Output.mPosition = mul(Output.mPosition, g_matProj);
	Output.m_UV = Input.mUV;

	vector worldPosition = mul(Input.mPosition, g_matWorld);
	float3 lightDir = worldPosition.xyz - gWorldLightPosition.xyz;
	Output.mLightDir = normalize(lightDir);

	float3 viewDir = normalize(worldPosition.xyz - gWorldCameraPosition.xyz);
	Output.mViewDir = viewDir;

	float3 worldNormal = mul(Input.mNormal, (float3x3)g_matWorld);
	Output.N = normalize(worldNormal);

	float3 worldTangent = mul(Input.mTangent, (float3x3)g_matWorld);
	Output.T = normalize(worldTangent);

	float3 worldBinormal = mul(Input.mBinormal, (float3x3)g_matWorld);
	Output.B = normalize(worldBinormal);

	return Output;
}

VS_OUT_Default VS_MAIN_Reverse(VS_IN_Default Input)
{
	VS_OUT_Default Output;

	Output.mPosition = mul(Input.mPosition, g_matWorld);
	Output.mPosition = mul(Output.mPosition, g_matView);
	Output.mPosition = mul(Output.mPosition, g_matProj);
	Output.m_UV = Input.mUV;

	vector worldPosition = mul(Input.mPosition, g_matWorld);
	float3 lightDir = gWorldLightPosition.xyz - worldPosition.xyz;
	Output.mLightDir = normalize(lightDir);

	float3 viewDir = normalize(worldPosition.xyz - gWorldCameraPosition.xyz);
	Output.mViewDir = viewDir;

	float3 worldNormal = mul(Input.mNormal, (float3x3)g_matWorld);
	Output.N = normalize(worldNormal);

	float3 worldTangent = mul(Input.mTangent, (float3x3)g_matWorld);
	Output.T = normalize(worldTangent);

	float3 worldBinormal = mul(Input.mBinormal, (float3x3)g_matWorld);
	Output.B = normalize(worldBinormal);

	return Output;
}

VS_OUT_NotNormal VS_MAIN_NotNormal(VS_IN_NotNormal Input)
{
	VS_OUT_NotNormal Output;

	Output.mPosition = mul(Input.mPosition, g_matWorld);
	Output.mPosition = mul(Output.mPosition, g_matView);
	Output.mPosition = mul(Output.mPosition, g_matProj);
	Output.m_UV = Input.mUV;

	vector worldPosition = mul(Input.mPosition, g_matWorld);
	float3 lightDir = gWorldLightPosition.xyz - worldPosition.xyz;

	float3 viewDir = normalize(worldPosition.xyz - gWorldCameraPosition.xyz);
	Output.mViewDir = viewDir;

	float3 worldNormal = mul(Input.mNormal, (float3x3)g_matWorld);
	worldNormal = normalize(worldNormal);

	Output.mDiffuse = dot(-lightDir, worldNormal);
	Output.mReflection = reflect(lightDir, worldNormal);

	return Output;
}

struct PS_IN_Default
{
	float2 mUV : TEXCOORD0;
	float3 mLightDir : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
	float3 T : TEXCOORD3;
	float3 B : TEXCOORD4;
	float3 N : TEXCOORD5;
};

struct PS_IN_NotNormal
{
	float2 mUV : TEXCOORD0;
	float3 mDiffuse : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
	float3 mReflection : TEXCOORD3;
};

float4 PS_MAIN_Default(PS_IN_Default Input) : COLOR
{
	float3 tangentNormal = tex2D(NormalSampler, Input.mUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

	float4 albedo = tex2D(DiffuseSampler, Input.mUV);
	float3 lightDir = normalize(Input.mLightDir);
	float3 diffuse = saturate(dot(worldNormal, -lightDir));
	diffuse = gLightColor * albedo.rgb * diffuse;

	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

	return float4(ambient + diffuse, 1);
}

float4 PS_MAIN_Alph(PS_IN_Default Input) : COLOR
{
	float3 tangentNormal = tex2D(NormalSampler, Input.mUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

	float4 albedo = tex2D(DiffuseSampler, Input.mUV);
	float3 lightDir = normalize(Input.mLightDir);
	float3 diffuse = saturate(dot(worldNormal, -lightDir));
	diffuse = gLightColor * albedo.rgb * diffuse;

	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

	return float4(ambient + diffuse, 0.4f);
}

float4 PS_MAIN_Specular(PS_IN_Default Input) : COLOR
{
	float3 tangentNormal = tex2D(NormalSampler, Input.mUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
	TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

	float4 albedo = tex2D(DiffuseSampler, Input.mUV);
	float3 lightDir = normalize(Input.mLightDir);
	float3 diffuse = saturate(dot(worldNormal, -lightDir));
	diffuse = gLightColor * albedo.rgb * diffuse;

	float3 specular = 0;
	if (diffuse.x > 0)
	{
		float3 reflection = reflect(lightDir, worldNormal);
		float3 viewDir = normalize(Input.mViewDir);

		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.f);
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

	return float4(ambient + diffuse + specular, 1);
}



float4 PS_MAIN_Alph_NotNormal(PS_IN_NotNormal Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mUV);
	float3 viewDir = normalize(Input.mViewDir);

	float3 reflection = normalize(Input.mReflection);
	float3 diffuse = gLightColor * albedo.rgb * saturate(Input.mDiffuse);

	float3 specular = 0;
	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.f);
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;

	return float4(ambient + diffuse + specular, 0.8);
}

technique ShaderPlayerMesh
{
	// Notmetal Armor
	pass noneAlph_noneSpecular
	{
		VertexShader = compile vs_3_0 VS_MAIN_Defalut();
	PixelShader = compile ps_3_0 PS_MAIN_Default();
	}

		// Upper2, eye
		pass Alph_noneSpecular
	{
		VertexShader = compile vs_3_0 VS_MAIN_Defalut();
	PixelShader = compile ps_3_0 PS_MAIN_Alph();
	}

		// metal Armor
		pass noneAlph_Specular
	{
		VertexShader = compile vs_3_0 VS_MAIN_Defalut();
	PixelShader = compile ps_3_0 PS_MAIN_Specular();
	}

		// Hair
		pass Alph_Specular
	{
		VertexShader = compile vs_3_0 VS_MAIN_NotNormal();
	PixelShader = compile ps_3_0 PS_MAIN_Alph_NotNormal();
	}

		// Skin
		pass Reverse_Specular
	{
		VertexShader = compile vs_3_0 VS_MAIN_Reverse();
	PixelShader = compile ps_3_0 PS_MAIN_Specular();
	}
};