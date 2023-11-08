struct VertexShaderOutput
{
    float3 OriginPos : POSITION;
    float2 UV : UV_TEXCOORD;
    float4 Position : SV_POSITION;
};

cbuffer Instance : register(b1) {
    matrix M;
    float3 inputColor;
};

#include "../Core.hlsli"

VertexShaderOutput main(DefaultVertexData input)
{
    VertexShaderOutput output;
    matrix MVP = mul(M, VP);
    output.Position = mul(float4(input.Position, 1.0f), MVP);
    output.OriginPos = output.Position.xyz;
    output.UV = input.UV;

    return output;
}