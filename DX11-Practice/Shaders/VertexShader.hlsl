cbuffer CameraBuffer : register(b0)
{
    matrix view;
    matrix project;
};

cbuffer PreDrawBuffer : register(b1)
{
    matrix transform;
};

struct VSOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOut main(float3 pos : POSITION)
{
    matrix matrix_VP = mul(project, view);
    matrix matrix_MVP = mul(matrix_VP, transform);

    VSOut o;
    o.position = mul(matrix_MVP, float4(pos, 1));
    o.color = float4(pos + 0.5, 1);
    return o;
}