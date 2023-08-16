struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
    float2 UV : UV_TEXCOORD;
};

cbuffer ExternalInput : register(b2){
    float3 inputColor;
};

float4 main(VertexShaderOutput input) : SV_TARGET
{
    return float4(input.UV,0, 1);
}