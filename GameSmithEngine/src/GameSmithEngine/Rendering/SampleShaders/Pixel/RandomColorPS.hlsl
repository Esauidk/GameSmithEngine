struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
    float2 UV : UV_TEXCOORD;
};


cbuffer Instance : register(b1)
{
    matrix model;
    float4 colors[8];
};
#include "../Core.hlsli"

float4 main(VertexShaderOutput input) : SV_TARGET
{
    return float4(input.UV.x, input.UV.y, 1, 1);
}