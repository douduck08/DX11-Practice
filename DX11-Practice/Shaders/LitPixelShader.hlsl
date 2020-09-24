#include "Cbuffers.hlsli"
#include "BlinnPhongLighting.hlsli"

struct PSIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float4 normal : Normal;
    float4 tangent : Tangent;
    float4 bitangent : Bitangent;
    float4 worldPos : WorldPos;
    float4 shadowPos : ShadowPos;
    float2 uv : Texcoord;
};

float4 main(PSIn input, uint id : SV_PrimitiveID) : SV_TARGET
{
    const float2 uv = input.uv;
    float3 diffColor = GetDiffuseColor(uv);
    float3 specColor = GetSpecularColor(uv);
    float3 n = GetWorldNormal(input.normal.xyz, input.tangent.xyz, input.bitangent.xyz, uv);
    float3 v = normalize(cameraPosition.xyz - input.worldPos.xyz);

    float3 ambient = ambientColor.rgb;
    float3 c = 0;
    for (int idx = 0; idx < lightNumber; idx++)
    {
        Light light = GetLightData(lights[idx], input.worldPos.xyz);
        if (lights[idx].position.w == 0)
        {
            float shadow = GetShadowPCF(input.shadowPos);
            light.color *= shadow;
        }
        
        float nl = saturate(dot(n, light.direction));
        c += ambient * diffColor;
        c += light.color * nl * (diffColor + specColor * SpecularTerm(n, light.direction, v));
    }
    
    return float4(saturate(c), 1.0);
}