float4 main(uint id : SV_PrimitiveID) : SV_TARGET
{
    const float3 colors[] =
    {
        { 1.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f },
        { 0.0f, 1.0f, 1.0f },
    };
	
    return float4(colors[id / 2], 1.0f);
}