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
	float4 litColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	uint lights_size, dummy;
    gLight.GetDimensions(lights_size, dummy);

	for(uint i = 0; i < lights_size; i++)
	{
		float4 light_color = gLight[i].color;
		float3 light_position = gLight[i].positionView;
		// The vector from the surface to the light.
		float3 pos_light = light_position - pin.world_pos;
		pos_light = normalize(pos_light);



		ambient = gMaterial.Ambient * light_color* 0.2;

		float diffuse_angle = dot(pos_light, pin.normal);
		//[flatten]
		if( diffuse_angle > 0.0f )
		{
			float3 refect_vec = reflect(-pos_light, pin.normal);

			float spec_factor = pow(max(dot(refect_vec, pos_eye), 0.0f), gMaterial.Shininess);

			diffuse = diffuse_angle * gMaterial.Diffuse * light_color;
			spec    = spec_factor * gMaterial.Specular * light_color;
		}
		
	litColor += (ambient + diffuse + spec);
	}

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
