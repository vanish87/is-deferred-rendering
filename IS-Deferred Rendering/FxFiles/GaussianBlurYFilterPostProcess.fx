cbuffer cbPerObject
{
	float4x4 g_world_matrix;
	float4x4 g_model_matrix;
	float4x4 g_view_matrix;
	float4x4 g_view_proj_matrix;
	float4x4 g_world_inv_transpose;
	float4x4 g_inv_proj_matrix;
	float4x4 g_inv_view_matrix;
};

Texture2D input_tex;

cbuffer cbPerFrame
{
	float3 g_eye_pos;
	float3 g_eye_z;
};

struct VertexIn
{
	float3 pos  : POSITION;
};

struct VertexOut
{
	float4 pos			: SV_POSITION;
	float2 tex			: TEXCOORD;
};
struct PSOutput
{
	float4 color		: SV_Target0;
};

SamplerState ShadowMapSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
    vout.pos = float4(vin.pos.xyz, 1.0f);
	vout.tex = float2(vin.pos.x * 0.5 + 0.5, -vin.pos.y * 0.5 + 0.5);
    return vout;
}

PSOutput PS(VertexOut pin) 
{
	float blurSize = 1.0f/1024.0f;
	PSOutput output;	
	float4 sum =0.0;
 
    // blur in y (vertical)
    // take nine samples, with the distance blurSize between them
    sum += input_tex.Sample(ShadowMapSampler, float2(pin.tex.x, pin.tex.y - 4.0*blurSize)) * 0.05;
    sum += input_tex.Sample(ShadowMapSampler, float2(pin.tex.x, pin.tex.y - 3.0*blurSize)) * 0.09;
    sum += input_tex.Sample(ShadowMapSampler, float2(pin.tex.x, pin.tex.y - 2.0*blurSize)) * 0.12;
    sum += input_tex.Sample(ShadowMapSampler, float2(pin.tex.x, pin.tex.y - blurSize)) * 0.15;
    sum += input_tex.Sample(ShadowMapSampler, float2(pin.tex.x, pin.tex.y)) * 0.16;
    sum += input_tex.Sample(ShadowMapSampler, float2(pin.tex.x, pin.tex.y + blurSize)) * 0.15;
    sum += input_tex.Sample(ShadowMapSampler, float2(pin.tex.x, pin.tex.y + 2.0*blurSize)) * 0.12;
    sum += input_tex.Sample(ShadowMapSampler, float2(pin.tex.x, pin.tex.y + 3.0*blurSize)) * 0.09;
    sum += input_tex.Sample(ShadowMapSampler, float2(pin.tex.x, pin.tex.y + 4.0*blurSize)) * 0.05;
	
	output.color = sum;
	return output;
}

technique11 PPTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}
