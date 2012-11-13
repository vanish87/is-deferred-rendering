//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; 
	float  Shininess;
};
struct PointLight
{
    float3 positionView;
    float attenuationBegin;
    float4 color;
    float attenuationEnd;
};

StructuredBuffer<PointLight> gLight;

Texture2D position_tex;
Texture2D diffuse_tex;
Texture2D specular_tex;
Texture2D normal_tex;

cbuffer cbPerFrame
{
	float4 g_light_color;
	float3 g_light_position;
	float3 g_eye_pos;
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
};

struct VertexOut
{
	float4 pos			: SV_POSITION;
	float3 world_pos    : POSITION;
    float3 normal		: NORMAL;
};

VertexOut GbufferVS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.	
	float4x4 world_matrix = mul(g_model_matrix, g_world_matrix);
	float4x4 mvp_matrix = mul(world_matrix ,g_view_proj_matrix);
	vout.pos = mul(float4(vin.pos, 1.0f), mvp_matrix);
	vout.normal = normalize(mul(vin.normal, (float3x3)g_world_inv_transpose));

	vout.world_pos = mul(float4(vin.pos, 1.0f), world_matrix).xyz;
    
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
	float SpecularPower = 1;

	output.Normal = float4(pin.normal.xyz, 0.0f);
	output.DiffuseAlbedo = gMaterial.Diffuse;
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
	if(0)
	{
	int3 samplelndices = int3( pin.pos.xy, 0 );
	float3 world_pos = position_tex.Load( samplelndices ).xyz;
	return float4(world_pos,1.0f);
	}
	else{

	int3 samplelndices = int3( pin.pos.xy, 0 );
	float3 world_pos = position_tex.Load( samplelndices ).xyz;
	float3 normal = normal_tex.Load( samplelndices ).xyz;
	float4 diffuse_mat = diffuse_tex.Load( samplelndices );
	float4 spercular_mat = specular_tex.Load( samplelndices );

	float3 pos_eye = normalize(g_eye_pos - world_pos);
	
	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 litColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	uint lights_size, dummy;
    gLight.GetDimensions(lights_size, dummy);

	for(uint i = 0; i < lights_size; i++)
	{
		float4 light_color = gLight[i].color;
		float3 light_position = gLight[i].positionView;
		// The vector from the surface to the light.
		float3 pos_light = light_position - world_pos;
		pos_light = normalize(pos_light);



		ambient = light_color* 0;

		float diffuse_angle = dot(pos_light, normal);
		[flatten]
		if( diffuse_angle > 0.0f )
		{
			float3 refect_vec = reflect(-pos_light, normal);

			float spec_factor = pow(max(dot(refect_vec, pos_eye), 0.0f), spercular_mat.w);

			diffuse = diffuse_angle * diffuse_mat * light_color;
			spec    = spec_factor * float4(spercular_mat.xyz, 1.0f) * light_color;
		}
		
	litColor += (ambient + diffuse + spec);
	}

	litColor.a = diffuse_mat.w;
    return litColor;
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
