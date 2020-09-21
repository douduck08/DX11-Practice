#include "Cbuffers.hlsli"

struct VSIn
{
    float3 pos : POSITION;
    float3 normal : Normal;
    float2 texcoord : Texcoord;
    float3 tangent : Tangent;
    float3 bitangent : Bitangent;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float4 normal : Normal;
    float4 tangent : Tangent;
    float4 bitangent : Bitangent;
    float4 worldPos : WorldPos;
    float2 uv : Texcoord;
};

VSOut main(VSIn input)
{
    matrix matrix_VP = mul(cameraProject, cameraView);
    matrix matrix_MVP = mul(matrix_VP, transform);

    VSOut o;
    o.position = mul(matrix_MVP, float4(input.pos, 1));
    o.color = float4(1, 1, 1, 1);
    o.normal = float4(input.normal, 0);
    o.tangent = float4(input.tangent, 0);
    o.bitangent = float4(input.bitangent, 0);
    o.worldPos = mul(transform, float4(input.pos, 1));
    o.uv = input.texcoord;
    return o;
}