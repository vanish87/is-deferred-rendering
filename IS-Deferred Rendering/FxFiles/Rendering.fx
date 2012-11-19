struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; //it will be only float
	float  Shininess;
};

struct PointLight
{
    float3 position;
    float4 color;
};
StructuredBuffer<PointLight> gLight;

cbuffer cbPerFrame
{
	float3 g_eye_pos;
};

float4 CalLighting( in float3 normal, 
					 in float3 position, //world_pos
					 in float4 diffuseAlbedo,
					 in float3 specularAlbedo,
					 in float specularPower )
{
	float3 pos_eye = normalize(g_eye_pos - position);
	
	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 litColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	uint lights_size, dummy;
    gLight.GetDimensions(lights_size, dummy);

	for(uint i = 0; i < lights_size; i++)
	{
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 light_color = gLight[i].color;
		float3 light_position = gLight[i].position;//world_pos
		// The vector from the surface to the light.
		float3 pos_light = light_position - position;
		pos_light = normalize(pos_light);

		float diffuse_angle = dot(pos_light, normal);//N * Lc
		[flatten]
		if( diffuse_angle > 0.0f )
		{
			float3 refect_vec = reflect(-pos_light, normal);

			float spec_factor = pow(max(dot(refect_vec, pos_eye), 0.0f), specularPower);

			//Cdiff * Clight * (N * Lc)
			diffuse =  diffuseAlbedo * light_color * diffuse_angle;
			//pow(R*V, alpha) * Cspec * Clight * (N * Lc)
			spec    = spec_factor * float4(specularAlbedo, 1.0f) * light_color * diffuse_angle;
		}
		
		float4 acc_color = (ambient + diffuse + spec);
		litColor = litColor + acc_color;
	}

	litColor.a = diffuseAlbedo.w;
    return litColor;
}

float4 CalPreLighting(	 in float3 normal, 
						 in float3 position, //world_pos
						 in float  specularPower )
{
	float3 pos_eye = normalize(g_eye_pos - position);//V
	
	// Start with a sum of zero. 
	float4 litColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	uint lights_size, dummy;
    gLight.GetDimensions(lights_size, dummy);

	for(uint i = 0; i < lights_size; i++)
	{
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float spec    = 0.0f;
		float4 light_color = gLight[i].color;
		float3 light_position = gLight[i].position;//world_pos
		// The vector from the surface to the light.
		float3 pos_light = light_position - position;//Lc
		pos_light = normalize(pos_light);

		float diffuse_angle = dot(pos_light, normal);//N * Lc (light vector = L)
		[flatten]
		if( diffuse_angle > 0.0f )
		{
			float3 H = normalize(pos_light + pos_eye);

			float spec_factor = pow(max(dot(normal, H), 0.0f), specularPower);
			
			//Clight * (N * Lc)
			diffuse = light_color * diffuse_angle;
			//pow(N*H, alpha) * Clight * (N * Lc)
			spec    = spec_factor * light_color.r * diffuse_angle;//only one value(specular intensity) for spec
		}
		
		float4 acc_color = float4(diffuse.rgb , spec);
		litColor = litColor + acc_color;
	}	
    return litColor;
}
