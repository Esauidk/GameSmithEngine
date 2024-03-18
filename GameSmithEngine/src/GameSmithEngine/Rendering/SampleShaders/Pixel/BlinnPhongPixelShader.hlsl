struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
    float2 UV : UV_TEXCOORD;
    float3 Normal : NORMAL;
    float3 WorldNormal : WORLDNORMAL;
    float3 WorldPos : WORLDPOS;
    uint vert_Id : ID;
};


cbuffer Instance : register(b1) {
	matrix M;
};

#include "../Core.hlsli"

float4 main(VertexShaderOutput input) : SV_TARGET
{
	// Assuming only light is directional light
	float3 L = -normalize(LIGHT_WORLD_POS.xyz);
	float atten = 1.0f;
    float power = 1;
	
	if (LIGHT_WORLD_POS.w != 0) {
        float test = length(input.WorldNormal);
		L = normalize(LIGHT_WORLD_POS.xyz - input.WorldPos);
		atten = 1.0f / pow(length(LIGHT_WORLD_POS.xyz - input.WorldPos), 2);
        power = 5;
    }

	float3 N = normalize(input.WorldNormal);
	
	float diffuseIntensity = max(dot(N, L), 0);
	float3 diffuse = power * diffuseIntensity * LIGHT_COLOR * float3(0.9, 0.1, 0.33334) * atten; // Last one is diffuse color

	float3 V = normalize(CAMERA_WORLD_POS - input.WorldPos);
	float3 H = normalize(V + L);
	float specularIntensity = pow(max(dot(N, H), 0), 0.8);
	float3 specular = power * specularIntensity * LIGHT_COLOR * float3(0.1, 0.4, 0.4) * atten; // Last one is specular color

	return float4((diffuse + specular), 1.0f);
}