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
    
VertexShaderOutput main(VertexPosColor input)
{
    VertexShaderOutput output;
    
    output.Position = float4(input.Position, 1.0f);
    output.Color = float4(input.Color, 1.0f);
    
    return output;
}