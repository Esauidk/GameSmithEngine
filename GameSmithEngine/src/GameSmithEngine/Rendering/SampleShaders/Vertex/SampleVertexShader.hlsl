struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
    float2 UV : UV_TEXCOORD;
    float3 Normal : NORMAL;
    float3 WorldNormal : WORLDNORMAL;
    float3 WorldPos : WORLDPOS;
    uint vert_Id : ID;
};


cbuffer Instance : register(b1) {
    matrix M;
    float3 inputColor;
};

#include "../Core.hlsl"
    
VertexShaderOutput main(DefaultVertexData input, uint id : SV_VertexID)
{
    VertexShaderOutput output;
    output.vert_Id = id;
    output.Normal = input.Normal;
    matrix MVP = mul(VP, M);
    output.WorldPos = mul(M, float4(input.Position, 1.0f)).xyz;
    output.WorldNormal = mul(M, float4(input.Normal, 0.0f)).xyz;
    output.Position = mul(MVP, float4(input.Position, 1.0f));
    output.UV = input.UV;

    return output;
}