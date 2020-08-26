cbuffer CBuffer
{
    matrix transform;
};

float4 main( float3 pos : POSITION ) : SV_POSITION
{
    return mul(transform, float4(pos, 1));
}