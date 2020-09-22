cbuffer PreMaterialBuffer : register(b4)
{
    int useDiffuseMap;
    int useSpecularMap;
    int useNormalMap;
};

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);
SamplerState texSampler : register(s0);

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