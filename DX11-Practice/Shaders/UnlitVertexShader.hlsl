#include "Cbuffers.hlsli"

struct VSOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOut main(float3 pos : POSITION)
{
    matrix matrix_VP = mul(cameraProject, cameraView);
    matrix matrix_MVP = mul(matrix_VP, transform);

    VSOut o;
    o.position = mul(matrix_MVP, float4(pos, 1));
    o.color = float4(1, 1, 1, 1);
    return o;
}