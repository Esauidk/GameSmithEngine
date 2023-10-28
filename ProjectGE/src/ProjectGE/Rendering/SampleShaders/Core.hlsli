cbuffer Global : register(b0) {
	matrix VP;
	float3 CAMERA_WORLD_POS;
	float3 LIGHT_WORLD_POS;
	float3 LIGHT_DIRECTION;
	float3 LIGHT_COLOR;
};