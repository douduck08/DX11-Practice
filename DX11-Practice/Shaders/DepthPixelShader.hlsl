#include "Cbuffers.hlsli"

cbuffer PreMaterialBuffer : register(b3)
{
    int useDiffuseMap;
    int useSpecularMap;
    int useNormalMap;
};

Texture2D diffuseMap : register(t0);
SamplerState texSampler : register(s0);

struct PSIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float4 normal : Normal;
    float4 tangent : Tangent;
    float4 bitangent : Bitangent;
    float4 worldPos : WorldPos;
    float2 uv : Texcoord;
};

float4 main(PSIn input, uint id : SV_PrimitiveID) : SV_TARGET
{
    const float2 uv = input.uv;
    if (useDiffuseMap)
    {
        float alpha = diffuseMap.Sample(texSampler, uv).a;
        clip(alpha - 0.5);
    }
    return 0;
}