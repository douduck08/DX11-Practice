cbuffer CBuffer
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
    VSOut o;
    o.position = mul(transform, float4(pos, 1));
    o.color = float4(pos + 0.5, 1);
    return o;
}