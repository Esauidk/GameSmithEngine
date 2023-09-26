struct VertexInput
{
    float3 Position : POSITION;
};

struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
};


VertexShaderOutput main(VertexInput input )
{
    VertexShaderOutput vso;
    vso.Position = float4(input.Position,1);

    return vso;
}