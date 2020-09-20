#include "Cbuffers.hlsli"

struct PSIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float4 normal : Normal;
    float4 worldPos : WorldPos;
    uint id : SV_PrimitiveID;
};

float4 main(PSIn input) : SV_TARGET
{
    float3 v = normalize(cameraPosition.xyz - input.worldPos.xyz);
    float3 l = lerp(lights[0].position.xyz, lights[0].position.xyz - input.worldPos.xyz, lights[0].position.w);
    float dist = length(l);
    l /= dist;
    
    const float attConst = 1.0;
    const float attLin = 0.045;
    const float attQuad = 0.0075;
    float att = lights[0].position.w * 1.0f / (attConst + attLin * dist + attQuad * (dist * dist));
    att = lerp(1.0, att, lights[0].position.w);
    
    float nl = saturate(dot(input.normal.xyz, l));
    float3 light = lights[0].color.rgb * att * nl;
    
    float3 diffuse = input.color.rgb * light;
    
    float3 r = reflect(-l, input.normal.xyz);
    float3 spec = input.color.rgb * light * pow(saturate(dot(v, r)), 128);
    
    return float4(saturate(diffuse + spec), 1.0);
}