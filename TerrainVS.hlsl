cbuffer Constants : register(b0)
{
	matrix view;
	matrix transform;
};

struct VS_OUT
{
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
	float4 pos : SV_POSITION;
};

VS_OUT main(float3 pos : POSITION, float3 normal : NORMAL, float2 texcoord : TEXCOORD)
{
	VS_OUT output;
	output.pos = mul(float4(pos, 1.0f), view);
	output.normal = mul(normal, (float3x3) transform);
	output.worldPos = (float3)mul(float4(pos, 1.0f), transform);
    output.texcoord = texcoord;
	return output;
}