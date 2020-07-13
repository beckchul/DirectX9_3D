#include "ShaderBase.hpp"

vector gWorldLightPosition;
vector gWorldCameraPosition;

float3 gLightColor;
float g_fDetail = 1.f;
float g_fBlack = 1.f;
float g_fColor = 1.f;

texture g_DiffuseMap;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseMap;
	minfilter = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
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

VS_OUT_NotNormal VS_MAIN_Defalut(VS_IN_NotNormal Input)
{
	VS_OUT_NotNormal Output;

	Output.mPosition = mul(Input.mPosition, g_matWorld);
	Output.mPosition = mul(Output.mPosition, g_matView);
	Output.mPosition = mul(Output.mPosition, g_matProj);
	Output.m_UV = Input.mUV;

	vector worldPosition = mul(Input.mPosition, g_matWorld);
	float3 lightDir = worldPosition.xyz - gWorldLightPosition.xyz;

	float3 viewDir = normalize(worldPosition.xyz - gWorldCameraPosition.xyz);
	Output.mViewDir = viewDir;

	float3 worldNormal = mul(Input.mNormal, (float3x3)g_matWorld);
	worldNormal = normalize(worldNormal);

	Output.mDiffuse = dot(-lightDir, worldNormal);
	Output.mReflection = reflect(lightDir, worldNormal);

	return Output;
}

VS_OUT_NotNormal VS_MAIN_Reverse(VS_IN_NotNormal Input)
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

struct PS_IN_NotNormal
{
	float2 mUV : TEXCOORD0;
	float3 mDiffuse : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
	float3 mReflection : TEXCOORD3;
};

float4 PS_MAIN_Default(PS_IN_NotNormal Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mUV);
	float3 viewDir = normalize(Input.mViewDir);

	float3 reflection = normalize(Input.mReflection);
	float3 diffuse = gLightColor * albedo.rgb * saturate(Input.mDiffuse);

	float3 specular = 0;
	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 3.f);
	}

	float3 ambient = float3(1.f, 1.f, 1.f) * albedo;

	return float4(ambient + diffuse + specular, 1);
}

float4 PS_MAIN_Alph(PS_IN_NotNormal Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mUV);

	float3 ambient = float3(1.f, 1.f, 1.f) * albedo;

	return float4(ambient, 0.5f);
}

float4 PS_MAIN_NotDiffuse(PS_IN_NotNormal Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mUV);
	float3 viewDir = normalize(Input.mViewDir);

	float3 reflection = normalize(Input.mReflection);
	float3 diffuse = gLightColor * albedo.rgb * saturate(Input.mDiffuse);

	float3 specular = 0;
	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 30.f);
	}

	float3 ambient = float3(1.f, 1.f, 1.f) * albedo;

	return float4(ambient + (diffuse * 0.2f) + (specular * 0.5f), 1);
}

float4 PS_MAIN_Skin(PS_IN_NotNormal Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mUV);
	float3 viewDir = normalize(Input.mViewDir);

	float3 reflection = normalize(Input.mReflection);
	float3 diffuse = gLightColor * albedo.rgb * saturate(Input.mDiffuse);

	float3 specular = 0;
	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 40.f);
	}

	float3 ambient = float3(1.f, 1.f, 1.f) * albedo;

	return float4(ambient + (diffuse * 0.2f) + (specular * 0.8f), 1);
}

float4 PS_MAIN_Hair(PS_IN_NotNormal Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mUV);
	float3 viewDir = normalize(Input.mViewDir);

	float3 reflection = normalize(Input.mReflection);
	float3 diffuse = gLightColor * albedo.rgb * saturate(Input.mDiffuse);

	float3 specular = 0;
	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 10.f);
	}

	float3 ambient = float3(0.9f, 0.9f, 0.9f) * albedo;

	return float4(ambient + diffuse + specular, 0.8);
}


float4 PS_MAIN_Monster(PS_IN_NotNormal Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mUV);
	float3 viewDir = normalize(Input.mViewDir);

	float3 reflection = normalize(Input.mReflection);
	float3 diffuse = gLightColor * albedo.rgb * saturate(Input.mDiffuse);

	float3 specular = 0;
	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 30.f);
	}

	float3 ambient = float3(1.f, 1.f, 1.f) * albedo;

	if (g_fDetail < 1.f)
	{
		ambient.yz = 0.f;
		diffuse.yz = 0.f;
		specular.yz = 0.f;
	}

	if (g_fBlack == 0.f)
	{
		ambient.x *= g_fColor;
		diffuse.x *= g_fColor;
		specular.x *= g_fColor;

		ambient.y *= g_fColor;
		diffuse.y *= g_fColor;
		specular.y *= g_fColor;

		ambient.z *= g_fColor;
		diffuse.z *= g_fColor;
		specular.z *= g_fColor;
	}

	return float4(ambient + (diffuse * 0.2f) + (specular * 0.5f), 1);
}

technique ShaderPlayerMesh
{
	// Notmetal Armor, eye
	pass noneAlph_noneSpecular
	{
		VertexShader = compile vs_3_0 VS_MAIN_Defalut();
		PixelShader = compile ps_3_0 PS_MAIN_NotDiffuse();
	}

	// Upper2
	pass Alph_noneSpecular
	{
		VertexShader = compile vs_3_0 VS_MAIN_Defalut();
		PixelShader = compile ps_3_0 PS_MAIN_Alph();
	}

	// metal Armor
	pass noneAlph_Specular
	{
		VertexShader = compile vs_3_0 VS_MAIN_Defalut();
		PixelShader = compile ps_3_0 PS_MAIN_Default();
	}

	// Hair
	pass Alph_Specular
	{
		VertexShader = compile vs_3_0 VS_MAIN_Defalut();
		PixelShader = compile ps_3_0 PS_MAIN_Hair();
	}

	// Skin
	pass Reverse_Specular
	{
		VertexShader = compile vs_3_0 VS_MAIN_Defalut();
		PixelShader = compile ps_3_0 PS_MAIN_Skin();
	}

	// Monster
	pass Reverse_Specular
	{
		VertexShader = compile vs_3_0 VS_MAIN_Defalut();
		PixelShader = compile ps_3_0 PS_MAIN_Monster();
	}
};