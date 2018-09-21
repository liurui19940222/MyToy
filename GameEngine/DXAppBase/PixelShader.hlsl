struct InputVertex{
	float4 position : SV_POSITION;
};

float4 main(InputVertex vertex) : SV_TARGET
{
	return float4(0.0f, 0.5f, 0.5f, 1.0f);
}