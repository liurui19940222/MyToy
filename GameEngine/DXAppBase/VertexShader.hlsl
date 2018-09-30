struct InputVertex{
	float4 position : POSITION;
};

struct OutputVertex{
	float4 position : SV_POSITION;
};

cbuffer uniform_struct{
	float4 time;
};

OutputVertex main( InputVertex vertex )
{
	OutputVertex output;
	float4 pos = vertex.position;
	pos.xyz *= time.xyz;
	output.position = float4(pos.x, pos.y, pos.z, 1.0f);
	return output;
}