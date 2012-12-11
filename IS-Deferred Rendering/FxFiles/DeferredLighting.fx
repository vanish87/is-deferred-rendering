//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************
#include "Rendering.fx"

//g-buffer
Texture2D diffuse_tex;
Texture2D normal_tex;
Texture2D depth_tex;

//for lighting buffer
Texture2D lighting_tex;

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
	float4x4 g_view_matrix;
	float4x4 g_view_proj_matrix;
	float4x4 g_world_inv_transpose;
	float4x4 g_inv_proj_matrix;
	float4x4 g_inv_view_matrix;
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
	vout.tex_cood = vin.tex_cood;    
    return vout;
}


struct GbufferPSOutput
{
	float4 Normal			: SV_Target0;
	float4 Diffuse			: SV_Target1;
};
GbufferPSOutput GbufferPS(VertexOut pin)
{
	GbufferPSOutput output;

	output.Normal = float4(pin.normal, gMaterial.Shininess);	
	output.Diffuse = float4(mesh_diffuse.Sample(MeshTextureSampler, pin.tex_cood).rgb, gMaterial.Specular.x);	

	return output;
}
struct LightingVin
{
	float4 Position : POSITION;
};

struct LightingVout
{
	float4 pos		: SV_POSITION;
	float3 posVS    : POSITION;
};

LightingVout LightingVS(in LightingVin vin)
{
	LightingVout vout;
	vout.pos = vin.Position;
	vout.posVS = mul(vin.Position, g_inv_proj_matrix).xyz ;
	return vout;
}

float4 LightingPS( in LightingVout pin): SV_Target
{
	if(0)//for debugging
	{
	int3 samplelndices = int3( pin.pos.xy, 0 );
	float3 world_pos = normal_tex.Load( samplelndices ).xyz;
	return float4(world_pos.xyz,1.0f);
	}
	else{
		
	
	int3 samplelndices = int3( pin.pos.xy, 0 );
	float3 view_ray_vec = normalize(pin.posVS);
	float depth = depth_tex.Load( samplelndices ).r;
	float3 world_pos;
	//if(depth==1.0f)world_pos=float3(0,0,0);
	//else{
		float px = ((( 2.0f * pin.pos.x) / 1280)  - 1.0f);
		float py = (((-2.0f * pin.pos.y) / 800) + 1.0f);
		float4 vPositionCS = float4(px, py, depth, 1.0f);
		float4 vPositionPS = mul(vPositionCS, g_inv_proj_matrix);
		vPositionPS = mul(vPositionPS, g_inv_view_matrix);
		world_pos = vPositionPS.xyz/ vPositionPS.www;
	//}
	
	if(0) return float4(world_pos,1.0f);

	//Get Infor from g-buffer
	float4 normal_t = normal_tex.Load( samplelndices );
	float3 normal = normal_t.xyz;
	float shininess = normal_t.w;

	//cal lighting
	return CalPreLighting( normal, world_pos, shininess);
	}
}

struct FinalVin
{
	float4 Position : POSITION;		
};

struct FinalVout
{
	float4 pos		: SV_POSITION;
};

FinalVout FinalVS(in FinalVin vin)
{
	FinalVout vout;
	vout.pos = vin.Position;
	return vout;
}

float4 FinalPS( in FinalVout pin): SV_Target
{
	if(0)//for debugging
	{
	int3 samplelndices = int3( pin.pos.xy, 0 );
	float4 world_pos = lighting_tex.Load( samplelndices );
	return float4(world_pos.www,1.0f);
	}
	else{
		
	//Get Infor from g-buffer
	int3 samplelndices = int3( pin.pos.xy, 0 );
	float4 lighting = lighting_tex.Load( samplelndices );
	float4 material = diffuse_tex.Load( samplelndices );
	float3 DiffuseAlbedo = material.rgb;
	//float4 DiffuseAlbedo = gMaterial.Diffuse;

	float3 diffuse = lighting.xyz * DiffuseAlbedo ;
	float3 specular = lighting.w *  float3(material.w,material.w,material.w);

	//cal lighting
	return float4(diffuse + specular , 1.0f);
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

	pass P2
	{
		SetVertexShader( CompileShader( vs_5_0, FinalVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, FinalPS() ) );
	}
}