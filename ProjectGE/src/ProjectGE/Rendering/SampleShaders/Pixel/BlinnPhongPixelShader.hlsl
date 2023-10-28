struct VertexShaderOutput
{
	float4 Position : SV_POSITION;
	float2 UV : UV_TEXCOORD;
	float3 WorldNormal : NORMAL;
	float3 WorldPos : WORLDPOS;
};

cbuffer Instance : register(b1) {
	matrix M;
};

#include "../Core.hlsli"

float4 main(VertexShaderOutput input) : SV_TARGET
{
	// Assuming only light is directional light
	float3 L = normalize(LIGHT_DIRECTION);
	float3 N = normalize(input.WorldNormal);
	
	float3 diffuseIntensity = max(dot(N, L), 0);
	float3 diffuse = diffuseIntensity * LIGHT_COLOR * float3(0.9, 0.1, 0.33334); // Last one is diffuse color

	float3 V = normalize(CAMERA_WORLD_POS - input.WorldPos);
	float3 H = normalize(V + L);
	float3 specularIntensity = pow(max(dot(N, H), 0), 0.9);
	float3 specular = specularIntensity * LIGHT_COLOR * float3(0.1, 0.4, 0.4); // Last one is specular color

	return float4((diffuse + specular), 1.0f);
}