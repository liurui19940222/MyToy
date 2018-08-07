#version 410
layout(location = 0) out vec4 FragColor;

uniform sampler2D MainTex;

in VERTEX{
	vec2 uv;
	vec4 range;
	vec4 color;
}vertex;

void main()
{
	vec2 uv = vertex.uv.xy / vertex.range.zw + vertex.range.xy;
	vec4 color = texture2D(MainTex, uv) * vertex.color;
	//discard(color.r + color.g + color.b < 0.05);
	FragColor = color;
}