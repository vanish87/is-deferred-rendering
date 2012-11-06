//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float  Reflect;
};

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

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	
	float4x4 world_matrix = mul(g_model_matrix, g_world_matrix);
	float4x4 mvp_matrix = mul(world_matrix ,g_view_proj_matrix);
	vout.pos = mul(float4(vin.pos, 1.0f), mvp_matrix);
	vout.normal = mul(vin.normal, (float3x3)g_world_inv_transpose);


	vout.world_pos = mul(float4(vin.pos, 1.0f), world_matrix).xyz;

	
	

	// Just pass vertex color into the pixel shader.
   // vout.Color = gMaterial.Ambient;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	pin.normal = normalize(pin.normal);

	float3 pos_eye = normalize(g_eye_pos - pin.world_pos);
	
	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 pos_light = g_light_position - pin.world_pos;
	pos_light = normalize(pos_light);

	ambient = gMaterial.Ambient * 0.1;

	float diffuse_angle = dot(pos_light, pin.normal);
	[flatten]
	if( diffuse_angle > 0.0f )
	{
		float3 refect_vec = reflect(-pos_light, pin.normal);

		float spec_factor = pow(max(dot(refect_vec, pos_eye), 0.0f), gMaterial.Reflect);

		diffuse = diffuse_angle * gMaterial.Diffuse * g_light_color;
		spec    = spec_factor * gMaterial.Specular * g_light_color;
	}
	float4 litColor = ambient + diffuse + spec;

	litColor.a = gMaterial.Diffuse.a;
    return litColor;
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
