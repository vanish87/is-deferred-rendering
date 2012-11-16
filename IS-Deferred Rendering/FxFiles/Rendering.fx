float3 Calctighting( in float3 normal, 
					 in float3 position,
					 in float3 diffuseAlbedo,
					 in float3 specularAlbedo,
					 in float specularPower )
{
		// Calculate the diffuse term
		float3 L = 0;
		float attenuation = 1.0f; 
		#if POINTLIGHT || SPOTLIGHT
		// Base the the light vector on the light position L = LightPos - position;
		11.2 Classic Deferred Rendering 505
		}
		// Calculate attenuation based on distance from the light source float dist = length( L );
		attenuation = max( 0, 1.0f - ( dist / LightRange.x ) );
		L /= dist; #elif DIRECTIONALLIGHT
		// Light direction is explicit for directional lights
		L = -LightDirection; #endif
		#if SPOTLIGHT
		// Also add in the spotlight attenuation factor float3 L2 = LightDirection;
		float rho = dot( -L, L2 );
		attenuation *= saturate( ( rho - SpotlightAngles.y )
		/ ( SpotlightAngles.x - SpotlightAngles.y ) );
		#endif
		float nDotL = saturate( dot( normal, L ) );
		float3 diffuse = nDotL * LightColor * diffuseAlbedo;
		// Calculate the specular term
		float3 V = CameraPos - position;
		float3 H = normalize( L + V );
		float3 specular = pow( saturate( dot( normal, H ) ), specular-Power )
		* LightColor * specularAlbedo.xyz * nDotL; // Final value is the sum of the albedo and diffuse with attenuation applied
return ( diffuse + specular ) * attenuation;