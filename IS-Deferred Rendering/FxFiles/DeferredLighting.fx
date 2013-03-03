#include "Rendering.fx"

//g-buffer
Texture2D diffuse_tex;
Texture2D normal_tex;
Texture2D depth_tex;

//Shadow map
Texture2D shadow_map_tex;

//for lighting buffer
Texture2D lighting_tex;

Texture2D mesh_diffuse;

SamplerState MeshTextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

SamplerState ShadowMapSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
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

	
	float4x4 g_shadow_transform; 
	float4x4 g_light_view_proj; 
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
	float2 tex_cood		: TEXCOORD0;
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
	//combines Mat with Tex color
	output.Diffuse = float4(mesh_diffuse.Sample(MeshTextureSampler, pin.tex_cood).rgb * gMaterial.Diffuse.rgb, gMaterial.Specular.x);	

	return output;
}
struct LightingVin
{
	float4 Position : POSITION;
};

struct LightingVout
{
	float4 pos		: SV_POSITION;
	float3 view_ray    : VIEWRAY;
};

LightingVout LightingVS(in LightingVin vin)
{
	LightingVout vout;
	vout.pos = vin.Position;
	float4 pos = mul(vin.Position, g_inv_proj_matrix);

	float3 positionVS = mul( vin.Position, g_inv_proj_matrix ).xyz;
	vout.view_ray = float3( positionVS.xy / positionVS.z, 1.0f );
	return vout;
}
float linstep(float min, float max, float v)
{
	return saturate((v - min) / (max - min));
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
	float3 view_ray_vec = pin.view_ray;
	float depth = depth_tex.Load( samplelndices ).r;

	float zf = 1000.0f;
	float zn = 1.0f;
	float q = zf/ (zf-zn);
	//view space Z
	float linear_depth = zn * q / (q - depth);

	//float viewZProj = dot(g_eye_z, view_ray_vec);
	//float3 positionWS = g_eye_pos + view_ray_vec * (linear_depth / viewZProj);
	float3 positionVS = view_ray_vec * linear_depth;

	//shadowing
	float4 world_pos = mul(float4(positionVS, 1.0f) , g_inv_view_matrix);
	//world_pos /= world_pos.w;
	//float2 shadow_tex_cood = mul(world_pos , g_shadow_transform);
	//float shadow_depth = shadow_map_tex.Sample(ShadowMapSampler, shadow_tex_cood).r;
	//shadow_depth = zn * q / (q - shadow_depth);

	
	float4 pos_light = mul(world_pos, g_light_view_proj);
	pos_light /= pos_light.w;
	pos_light.x = pos_light.x / 2 + 0.5f;
	pos_light.y = -pos_light.y / 2 + 0.5f;
	//float shadow_depth = shadow_map_tex.Load( samplelndices ).r;
	float shadow_depth  = shadow_map_tex.Sample(ShadowMapSampler, pos_light.xy).r;
	shadow_depth = zn * q / (q - shadow_depth);
	float pos_depth = pos_light.z;
	pos_depth = zn * q / (q - pos_depth);

	//pos_depth = length(mul(light.position.xyz,g_inv_view_matrix)  - world_pos);

	float min_variance = 0.3;
	float bleeding_reduce = 0.75;

	float2 moments = float2(shadow_depth, shadow_depth*shadow_depth);
	//float len = length(light.position.xyz - positionVS);
	float p = (pos_depth < moments.x );
	// Variance shadow mapping
	float variance = moments.y - moments.x * moments.x;
	variance = max(variance, min_variance);
	float m_d = moments.x - pos_depth;
	float p_max = variance / (variance + m_d * m_d);
	p_max = linstep(bleeding_reduce, 1, p_max);

	


	float shadow = max(p, p_max);
	//float shadow =1;
	//if(light.type == 1 && shadow_depth != zn && shadow_depth < pos_depth - 1   )
	//	shadow = 0;
	//if(depth==1.0f)world_pos=float3(0,0,0);
	//else{
		//float px = ((( 2.0f * pin.pos.x) / 1280)  - 1.0f);
		//float py = (((-2.0f * pin.pos.y) / 800) + 1.0f);
		//float4 vPositionCS = float4(px, py, 1.0f, 1.0f);
		//float4 vPositionPS = mul(vPositionCS, g_inv_proj_matrix);
		//float4 vPositionVS = float4(vPositionPS.xy/ vPositionPS.ww, linear_depth, 1.0f);
		//vPositionVS = mul(positionVS, g_inv_view_matrix);
		//world_pos = positionVS;
		//world_pos = positionVS;
	//}
	
	if(0)
	{
		shadow_depth /=1000.0f;
		return float4(shadow_depth,shadow_depth,shadow_depth,1.0f);
	}

	//Get Infor from g-buffer
	float4 normal_t = normal_tex.Load( samplelndices );
	float3 normal = normal_t.xyz;
	//set for those mesh that do not want to do lighting
	if(normal.x ==0 && normal.y ==0&& normal.z ==0)
		return float4(1,1,1,0);
	float shininess = normal_t.w;

	//float4 pre_color = lighting_tex.Load( samplelndices );

	//cal lighting
	return CalPreLighting( normal, positionVS, shininess, shadow);
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
	return float4(world_pos.xyz,1.0f);
	}
	else
	{
		
	//Get Infor from g-buffer
	int3 samplelndices = int3( pin.pos.xy, 0 );
	float4 lighting = lighting_tex.Load( samplelndices );
	float4 material = diffuse_tex.Load( samplelndices );
	float3 DiffuseAlbedo = material.rgb;

	if(0) return float4(DiffuseAlbedo,1.0f);
	//float4 DiffuseAlbedo = gMaterial.Diffuse;
	float3 diffuse = lighting.xyz * DiffuseAlbedo;
	float3 specular = lighting.w *  float3(material.w,material.w,material.w);
		
	//cal lighting
	return float4(diffuse + specular , 1.0f);
	//return float4(DiffuseAlbedo , 1.0f);

	}

	
}

BlendState lighing_acc
{
	BlendEnable[0] = TRUE;
	SrcBlend = One;
	DestBlend = One;
	BlendOp[0] = ADD;
	SrcBlendAlpha = One;
	DestBlendAlpha = Zero;
	BlendOpAlpha[0] = ADD;
	RenderTargetWriteMask[0] = 0x0f;

};

BlendState final
{
	BlendEnable[0] = TRUE;
	SrcBlend = One;
	DestBlend = Zero;
	BlendOp[0] = ADD;
	SrcBlendAlpha = One;
	DestBlendAlpha = Zero;
	BlendOpAlpha[0] = ADD;
	RenderTargetWriteMask[0] = 0x0f;

};


technique11 GbufferTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, GbufferVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, GbufferPS() ) );		
		SetBlendState(final, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
    }

	pass P1
	{
		SetVertexShader( CompileShader( vs_5_0, LightingVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, LightingPS() ) );
		SetBlendState(lighing_acc, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	}

	pass P2
	{
		SetVertexShader( CompileShader( vs_5_0, FinalVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, FinalPS() ) );		
		SetBlendState(final, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	}
};

//shadowing tech
struct ShadowVSOut
{
	float4 position_h : SV_POSITION;
	float2 tex  : TEXCOORD;
};

ShadowVSOut ShadowingVS(VertexIn vin)
{
	ShadowVSOut vout;

	float4x4 world_matrix = mul(g_model_matrix, g_world_matrix);
	float4x4 mvp_matrix = mul(world_matrix ,g_view_proj_matrix);
	vout.position_h = mul(float4(vin.pos, 1.0f), mvp_matrix);
	vout.tex  = vin.tex_cood;

	return vout;
}

technique11 Shadowing
{
	pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, ShadowingVS() ) );
		SetGeometryShader( NULL );
        SetPixelShader(NULL);		
    }
};

