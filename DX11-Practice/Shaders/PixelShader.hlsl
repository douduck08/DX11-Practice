struct PSIn
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    uint id : SV_PrimitiveID;
};

float4 main(PSIn input) : SV_TARGET
{
    return input.color;
}