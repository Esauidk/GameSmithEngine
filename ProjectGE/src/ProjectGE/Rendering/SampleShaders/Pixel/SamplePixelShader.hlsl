struct VertexShaderOutput
{
    float4 Color : COLOR;
    float4 Position : SV_POSITION;
};

cbuffer ExternalInput : register(b2){
    float3 inputColor;
};

float4 main(VertexShaderOutput input) : SV_TARGET
{
    return input.Color * float4(inputColor, 1);
}