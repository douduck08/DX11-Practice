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

struct Light
{
    float4 color;
    float4 position;
};

cbuffer LightBuffer : register(b2)
{
    Light lights[8];
    int lightNumber;
    int p0;
    int p1;
    int p2;
};