struct VertexPosColor
{
    float3 Position : POSITION;
    float2 UV : UV_TEXCOORD;
    float3 Normal : NORMAL;
};

struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
    float2 UV : UV_TEXCOORD;
    float3 WorldNormal : NORMAL;
    float3 WorldPos : WORLDPOS;
};


cbuffer Instance : register(b1) {
    matrix M;
    float3 inputColor;
};

#include "../Core.hlsli"
    
VertexShaderOutput main(VertexPosColor input)
{
    VertexShaderOutput output;
    output.WorldPos = mul(float4(input.Position, 1.0f), M).xyz;
    output.WorldNormal = mul(float4(input.Normal, 1.0f), M).xyz;
    output.Position = mul(float4(output.WorldPos, 1.0f), VP);
    output.UV = input.UV;

    return output;
}