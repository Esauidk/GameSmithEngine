cbuffer Instance : register(b1)
{
    matrix model;
};

#include "../Core.hlsl"

float4 main() : SV_TARGET
{
    return float4(LIGHT_COLOR.xyz, 1);

}