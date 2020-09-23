// common structs/methods
struct LightParam
{
    float4 color;
    float4 position;
};

struct Light
{
    float3 direction;
    float3 color;
};

Light GetLightData(LightParam lightParam, float3 worldPos)
{
    Light l;
    
    if (lightParam.position.w > 0)
    {
        // point light
        l.direction = lightParam.position.xyz - worldPos.xyz;
        float dist = length(l.direction);
        l.direction /= dist;
        
        const float attConst = 1.0;
        const float attLin = 0.045;
        const float attQuad = 0.0075;
        float atten = 1.0f / (attConst + attLin * dist + attQuad * (dist * dist));
        l.color = lightParam.color.rgb * atten * lightParam.color.a;
    }
    else
    {
        // directional light
        l.direction = lightParam.position.xyz;
        l.color = lightParam.color.rgb;
    }
    
    return l;
}

// constant buffers
cbuffer PerFrameBuffer : register(b0)
{
    float4 ambientColor;
};

cbuffer CameraBuffer : register(b1)
{
    matrix cameraView;
    matrix cameraProject;
    float4 cameraPosition;
};

cbuffer PreDrawBuffer : register(b2)
{
    matrix transform;
};

cbuffer LightBuffer : register(b3)
{
    LightParam lights[8];
    int lightNumber;
    int p0;
    int p1;
    int p2;
};
