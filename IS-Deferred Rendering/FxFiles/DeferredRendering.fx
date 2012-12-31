#include "Rendering.fx"

//g-buffer
Texture2D position_tex;
Texture2D diffuse_tex;
Texture2D specular_tex;
Texture2D normal_tex;

Texture2D mesh_diffuse;

SamplerState MeshTextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};


cbuffer cbPerObject
{
	float4x4 g_world_matrix;
	float4x4 g_model_matrix;
	float4x4 g_view_proj_matrix;
	float4x4 g_world_inv_transpose;
	Material gMaterial;
};

struct VertexIn
{
	float3 pos  : POSITION;
    float3 normal : NORMAL;	
	float2 tex_cood		: TEXCOORD;
};

struct VertexOut
{
	float4 pos			: SV_POSITION;
	float3 world_pos    : POSITION;
    float3 normal		: NORMAL;
	float2 tex_cood		: TEXCOORD;
};

VertexOut GbufferVS(VertexIn vin)
{
	VertexOut vout;
	
	float4x4 world_matrix = mul(g_model_matrix, g_world_matrix);
	float4x4 mvp_matrix = mul(world_matrix ,g_view_proj_matrix);
	vout.pos = mul(float4(vin.pos, 1.0f), mvp_matrix);
	vout.normal = normalize(mul(vin.normal, (float3x3)g_world_inv_transpose));

	vout.world_pos = mul(float4(vin.pos, 1.0f), world_matrix).xyz;

	vout.tex_cood = vin.tex_cood;    
    return vout;
}


struct GbufferPSOutput
{
	float4 Normal			: SV_Target0;
	float4 DiffuseAlbedo	: SV_Target1;
	float4 SpecularAlbedo	: SV_Target2;
	float4 Position			: SV_Target3;
};
GbufferPSOutput GbufferPS(VertexOut pin)
{
	GbufferPSOutput output;
	float3 diffuseAlbedo = float3(0, 0, 0);
	float3 SpecularAlbedo = float3(0, 0, 0);

	output.Normal = float4(pin.normal.xyz, 0.0f);
	output.DiffuseAlbedo = mesh_diffuse.Sample(MeshTextureSampler, pin.tex_cood);
	output.SpecularAlbedo = float4( gMaterial.Specular.xyz, gMaterial.Shininess );
	output.Position = float4( pin.world_pos, 1.0f );

	return output;
}
struct LightingVin
{
	float4 Position : POSITION;
};

struct LightingVout
{
	float4 pos		: SV_POSITION;
};

LightingVout LightingVS(in LightingVin vin)
{
	LightingVout vout;
	vout.pos = vin.Position;
	return vout;
}

float4 LightingPS( in LightingVout pin): SV_Target
{
	if(0)//for debugging
	{
	int3 samplelndices = int3( pin.pos.xy, 0 );
	float3 world_pos = normal_tex.Load( samplelndices ).xyz;
	return float4(world_pos,1.0f);
	}
	else{
		
	//Get Infor from g-buffer
	int3 samplelndices = int3( pin.pos.xy, 0 );
	float3 world_pos = position_tex.Load( samplelndices ).xyz;
	float3 normal = normal_tex.Load( samplelndices ).xyz;
	float4 diffuse_mat = diffuse_tex.Load( samplelndices );
	float4 spercular_mat = specular_tex.Load( samplelndices );

	//cal lighting
	return CalLighting( normal, world_pos, diffuse_mat , spercular_mat.xyz, spercular_mat.w);
	}
}

technique11 GbufferTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, GbufferVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, GbufferPS() ) );
    }

	pass P1
	{
		SetVertexShader( CompileShader( vs_5_0, LightingVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, LightingPS() ) );
	}
}
