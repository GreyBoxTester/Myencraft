cbuffer Constants : register(b0)
{
    matrix view;
    matrix transform;
};

cbuffer Time : register(b1)
{
    float time;
    float3 padding;
};

struct VS_OUT
{
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 pos : SV_POSITION;
};

float curve(float n)
{
    float num = fmod(n, 4.0f);
    return 0.333333f * num * num * num - 2.0f * num * num + 2.666666f * num;
}

VS_OUT main(float3 pos : POSITION, float3 normal : NORMAL, float2 texcoord : TEXCOORD)
{
    VS_OUT output;
    output.worldPos = (float3) mul(float4(pos, 1.0f), transform);
    if (texcoord.y != 1.0f)
    {
        float offset = curve(time * 0.5f + abs(output.worldPos.x + output.worldPos.y + output.worldPos.z) * 0.25f) * 0.1f;
        output.worldPos += float3(offset, offset, offset);
        output.pos = mul(float4(pos + float3(offset, offset, offset), 1.0f), view);
    }
    else
    {
        output.pos = mul(float4(pos, 1.0f), view);
    }
    output.normal = mul(normal, (float3x3) transform);
    output.texcoord = texcoord;
    return output;
}