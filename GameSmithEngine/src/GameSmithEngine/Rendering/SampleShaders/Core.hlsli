struct DefaultVertexData
{
	float3 Position : POSITION;
	float2 UV : UV_TEXCOORD;
	float3 Normal : NORMAL;
};

cbuffer Global : register(b0) {
	matrix VP;
	float3 CAMERA_WORLD_POS;
	float4 LIGHT_WORLD_POS;
	float3 LIGHT_COLOR;
};