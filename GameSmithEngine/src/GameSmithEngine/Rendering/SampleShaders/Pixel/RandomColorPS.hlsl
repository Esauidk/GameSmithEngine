struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
    float2 UV : UV_TEXCOORD;
    float3 WorldNormal : NORMAL;
    float3 WorldPos : WORLDPOS;
    uint vert_Id : ID;
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