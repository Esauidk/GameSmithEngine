struct VertexPosColor
{
    float3 Position : POSITION;
    float2 uv : UV_TEXCOORD;
};

struct VertexShaderOutput
{
    float3 OriginPos : POSITION;
    float2 UV : UV_TEXCOORD;
    float4 Position : SV_POSITION;
};

cbuffer Global : register(b0) {
    matrix VP;
};

cbuffer Instance : register(b1) {
    matrix M;
    float3 inputColor;
};


VertexShaderOutput main(VertexPosColor input)
{
    VertexShaderOutput output;
    matrix MVP = mul(M, VP);
    output.Position = mul(float4(input.Position, 1.0f), MVP);
    output.OriginPos = input.Position;
    output.UV = input.uv;

    return output;
}