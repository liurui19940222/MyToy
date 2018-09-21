struct InputVertex{
	float4 position : POSITION;
};

struct OutputVertex{
	float4 position : SV_POSITION;
};

cbuffer uniform_struct{
	float time;
};

OutputVertex main( InputVertex vertex )
{
	OutputVertex output;
	float4 pos = vertex.position;
	output.position = float4(pos.x, pos.y, pos.z, 1.0f);
	return output;
}