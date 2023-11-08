struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
};

#include "../Core.hlsli"

VertexShaderOutput main(DefaultVertexData input )
{
    VertexShaderOutput vso;
    vso.Position = float4(input.Position,1);

    return vso;
}