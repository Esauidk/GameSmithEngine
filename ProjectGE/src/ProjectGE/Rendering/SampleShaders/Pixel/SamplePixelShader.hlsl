struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
    float2 UV : UV_TEXCOORD;
};

cbuffer Instance : register(b1){
    matrix M;
    float3 inputColor;
};

Texture2D sampleImg : register(t0);

sampler samp : register(s0);

float4 main(VertexShaderOutput input) : SV_TARGET
{
    Texture2D copy = sampleImg;
    float4 color = copy.Sample(samp, input.UV);
    return color;
}