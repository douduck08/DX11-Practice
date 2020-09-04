cbuffer CameraBuffer : register(b0)
{
    matrix cameraView;
    matrix cameraProject;
    float4 cameraPosition;
};

cbuffer PreDrawBuffer : register(b1)
{
    matrix transform;
};

cbuffer LightBuffer : register(b2)
{
    float4 lightColor;
    float4 lightPosition;
};