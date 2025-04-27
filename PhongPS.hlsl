#define MAX_POINT_LIGHTS 4u
#define AMBIENT float3(0.2f, 0.2f, 0.2f)

struct pointLight
{
	float3 pos;
    float fallofIntensity;
    float modifier;
	float3 color;
};

struct directionalLight
{
    float3 color;
    bool active; //is also 4 byte padding
    float3 direction;
};

cbuffer material : register(b0)
{
    float3 materialColor;
    float specularPower;
}

cbuffer cameraBuffer : register(b1)
{
    float3 cameraPos;
    float unused; //padding
}

cbuffer pointLightBuffer : register(b2)
{
    pointLight pointLights[MAX_POINT_LIGHTS];
}

cbuffer directionalLightBuffer : register(b3)
{
    directionalLight dirLight;
    float padding;
}

/*float dist2(float3 vec)
{
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}*/

Texture2D tex : register(t0);

SamplerState smplr : register(s0);

float4 main(float3 pos : POSITION, float3 normal : NORMAL, float2 texcoord : TEXCOORD) : SV_TARGET
{	
    float4 pixel = tex.Sample(smplr, texcoord);
    clip(pixel.a < 0.1f ? -1 : 1);
    
    float3 color = float3(0.0f, 0.0f, 0.0f);
    
    if (specularPower != 0.0f)
    {
        float3 pixelToCamera = normalize(cameraPos - pos);
        
        for (uint i = 0; i < MAX_POINT_LIGHTS && pointLights[i].modifier > 0.0f; i++)
        {
            //diffuse
            float3 pixelToPointLight = pointLights[i].pos - pos;
            float dist = length(pixelToPointLight);
            pixelToPointLight /= dist;
            float att = pointLights[i].fallofIntensity * dist * (dist + 1) + pointLights[i].modifier;
            color += pointLights[i].color * max(0.0f, dot(pixelToPointLight, normal)) / att;
        
            //specular
            color += pointLights[i].color * pow(max(0.0f, dot(normalize(pixelToCamera + pixelToPointLight), normal)), specularPower) / att;
        }
        
        //directional
        if(dirLight.active)
        {
            color += dirLight.color * max(0.0f, dot(-dirLight.direction, normal));
            color += dirLight.color * pow(max(0.0f, dot(normalize(pixelToCamera - dirLight.direction), normal)), specularPower);
        }
    }
    else
    {
        for (uint i = 0; i < MAX_POINT_LIGHTS && pointLights[i].modifier > 0.0f; i++)
        {
            //diffuse
            float3 pixelToPointLight = pointLights[i].pos - pos;
            float dist = length(pixelToPointLight);
            pixelToPointLight /= dist;
            float att = pointLights[i].fallofIntensity * dist * (dist + 1) + pointLights[i].modifier;
            color += pointLights[i].color * max(0.0f, dot(pixelToPointLight, normal)) / att;
        }
        
        //directional
        if (dirLight.active)
        {
            color += dirLight.color * max(0.0f, dot(-dirLight.direction, normal));
        }
    }
	
    return float4(saturate(pixel.rgb * materialColor * (color + AMBIENT)), pixel.a/* * clamp(3096.0f - dist2(pos - cameraPos), 0.0f, 100.0f) * 0.01f*/);
}