struct VertexShaderOutput
{
    float4 Color : COLOR;
    float4 Position : SV_POSITION;
};

float4 main(VertexShaderOutput input) : SV_TARGET
{
    return input.Color;
}