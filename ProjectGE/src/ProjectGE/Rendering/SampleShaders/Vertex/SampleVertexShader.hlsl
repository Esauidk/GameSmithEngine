struct VertexPosColor
{
    float3 Position : POSITION;
    float3 Color : COLOR;
};

struct VertexShaderOutput
{
    float4 Color : COLOR;
    float4 Position : SV_POSITION;
};

cbuffer ViewProjection : register(b0)
{
    matrix VP;
};

cbuffer Model : register(b1){
    matrix M;
};

cbuffer ExternalInput : register(b2){
};

    
VertexShaderOutput main(VertexPosColor input)
{
    VertexShaderOutput output;
    matrix MVP = mul(M,VP);
    output.Position = mul(float4(input.Position, 1.0f), MVP);
    output.Color = float4(input.Color, 1.0f);
    
    return output;
}