#version 410
layout(location = 0) in vec4 a_VertexPosition;
layout(location = 1) in vec2 a_Texcoord;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec4 a_TexRange;
layout(location = 4) in mat4 a_ModelMatrix;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 CameraWorldPos;

out VERTEX{
	vec2 uv;
	vec4 range;
	vec4 color;
}vertex;

void main()
{
	vertex.uv = a_Texcoord;
	vertex.range = a_TexRange;
	vertex.color = a_Color;

	vec4 localPos = a_VertexPosition;
	mat4 m = M * a_ModelMatrix;
	mat4 world2Obj = inverse(m);
	vec4 cameraObjPos = world2Obj * vec4(CameraWorldPos, 1.0);
	vec3 forward = normalize(cameraObjPos.xyz);
	vec3 up = vec3(0.0, 1.0, 0.0);
	if (forward.y >= 0.9998)
	{
		up.y = 0.0;
		up.z = 1.0;
	}
	vec3 right = normalize(cross(up, forward));
	up = normalize(cross(forward, right));

	localPos.xyz = localPos.x * right + localPos.y * up + localPos.z * forward;

	gl_Position = P * V * m * localPos;
}