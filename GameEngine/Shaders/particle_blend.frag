#version 410
layout(location = 0) out vec4 FragColor;

uniform sampler2D MainTex;

in VERTEX{
	vec2 uv;
	vec4 range;
}vertex;

void main()
{
	vec2 uv = vertex.uv.xy * vertex.range.zw + vertex.range.xy;
	vec4 color = texture2D(MainTex, uv);
	//FragColor = color;
	FragColor = vec4(vec3(1.0, 1.0, 1.0) * vertex.range.w, 1.0);
}