cbuffer PreMaterialBuffer : register(b4)
{
    int useDiffuseMap;
    int useSpecularMap;
    int useNormalMap;
};

cbuffer ShadowBuffer : register(b5)
{
    matrix shadowMatrix;
    float4 shadowMapSize;
};

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);
Texture2D shadowMap : register(t3);

SamplerState texSampler : register(s0);
SamplerComparisonState shadowSampler : register(s1);

float3 GetWorldNormal(float3 normal, float3 tangent, float3 bitangent, float2 uv)
{
    if (useNormalMap && length(tangent) > 0)
    {
        const float3 tanSpaceNormal = normalMap.Sample(texSampler, uv).xyz * 2.0 - 1.0;
        const float3x3 tanToWorld = float3x3(tangent, bitangent, normal);
        const float3 n = normalize(mul(tanSpaceNormal, tanToWorld));
        return n;
    }
    else
    {
        return normal;
    }
}

float3 GetDiffuseColor(float2 uv)
{
    if (useDiffuseMap)
    {
        return diffuseMap.Sample(texSampler, uv).rgb;
    }
    else
    {
        return 1;
    }
}

float3 GetSpecularColor(float2 uv)
{
    if (useSpecularMap)
    {
        return specularMap.Sample(texSampler, uv).rgb;
    }
    else
    {
        return 1;
    }
}

float SpecularTerm(float3 n, float3 l, float3 v)
{
    const float specularPower = 128;
    float3 h = normalize(l + v);
    return pow(saturate(dot(h, n)), specularPower);
}

float GetShadow(float4 shadowPos)
{
    shadowPos.xyz /= shadowPos.w;
    return shadowMap.SampleCmpLevelZero(shadowSampler, shadowPos.xy, shadowPos.z).r;
}

float GetShadowPCF(float4 shadowPos)
{
    const float2 offset = shadowMapSize.zw;
    const float2 offsets[9] =
    {
        float2(-offset.x, -offset.y), float2(0.0f, -offset.y), float2(offset.x, -offset.y),
        float2(-offset.x, 0.0f),      float2(0.0f, 0.0f),      float2(offset.x, 0.0f),
        float2(-offset.x, offset.y),  float2(0.0f, offset.y),  float2(offset.x, offset.y)
    };

    shadowPos.xyz /= shadowPos.w;
    float shadow = 0;
    for (int i = 0; i < 9; i++)
    {
        shadow += shadowMap.SampleCmpLevelZero(shadowSampler, shadowPos.xy + offsets[i], shadowPos.z).r;
    }
    return shadow / 9.0f;
}