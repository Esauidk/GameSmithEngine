struct VertexPosColor
{
    float3 Position : POSITION;
    float2 uv : UV_TEXCOORD;
};

struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
    float2 UV : UV_TEXCOORD;
};

cbuffer ViewProjection : register(b0)
{
    matrix VP;
};

cbuffer Model : register(b1){
    matrix M;
};

cbuffer ExternalInput : register(b2){
    float3 inputColor;
};

    
VertexShaderOutput main(VertexPosColor input)
{
    VertexShaderOutput output;
    matrix MVP = mul(M,VP);
    output.Position = mul(float4(input.Position, 1.0f), MVP);
    output.UV = input.uv;

    return output;
}