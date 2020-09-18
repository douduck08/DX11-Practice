#include "Cbuffers.hlsli"

struct PSIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float4 normal : Normal;
    float4 tangent : Tangent;
    float4 bitangent : Bitangent;
    float4 worldPos : WorldPos;
    float2 uv : Texcoord;
    uint id : SV_PrimitiveID;
};

cbuffer PreMaterialBuffer : register(b3)
{
    int useSpecularMap;
    int useNormalMap;
    int pad0;
    int pad1;
};

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);
SamplerState texSampler : register(s0);

float4 main(PSIn input) : SV_TARGET
{
    const float2 uv = input.uv;
    float3 n = input.normal.xyz;
    if (useNormalMap && length(input.tangent.xyz) > 0)
    {
        float3 tanSpaceNormal = normalMap.Sample(texSampler, uv).xyz * 2.0 - 1.0;
        float3x3 tanToWorld = float3x3(input.tangent.xyz, input.bitangent.xyz, input.normal.xyz);
        n = normalize(mul(tanSpaceNormal, tanToWorld));
    }
    
    float3 v = normalize(cameraPosition.xyz - input.worldPos.xyz);
    float3 l = lerp(lightPosition.xyz, lightPosition.xyz - input.worldPos.xyz, lightPosition.w);
    float dist = length(l);
    l /= dist;

    const float attConst = 1.0;
    const float attLin = 0.045;
    const float attQuad = 0.0075;
    float att = lightPosition.w * 1.0f / (attConst + attLin * dist + attQuad * (dist * dist));
    att = lerp(1.0, att, lightPosition.w);
    
    float nl = saturate(dot(n, l));
    float3 light = lightColor.rgb * att * nl;

    float3 ambient = float3(0.2, 0.2, 0.2);
    float3 diffuse = (ambient + input.color.rgb * light) * diffuseMap.Sample(texSampler, uv).rgb;

    float3 specColor = 1;
    float specularPower = 128;
    if (useSpecularMap)
    {
        float4 specularSample = specularMap.Sample(texSampler, uv);
        specColor = specularSample.rgb;
        // specularPower = pow(2.0f, specularSample.a * 13.0f);
    }
    float3 r = reflect(-l, n);
    float3 spec = specColor * input.color.rgb * light * pow(saturate(dot(v, r)), specularPower);

    
    return float4(saturate(diffuse + spec), 1.0);
}