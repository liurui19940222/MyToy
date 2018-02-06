#version 410
layout(location = 0) out vec4 FragColor;

uniform vec4 Color;
uniform sampler2D MainTex;

const float width = 0.05;

in VERTEX{
	vec2 uv;
	vec4 range;
}vertex;

void main()
{
	vec2 uv = vertex.uv.xy * vertex.range.zw + vertex.range.xy;
	vec4 c = texture(MainTex, uv);
	c.rgb *= Color.rgb;
	//c.a = pow(c.a, 0.45455);
	//draw the border
	//c = mix(c, vec4(1.0, 0.0, 0.0, 1.0), step(vertex.uv.x - width, 0.0));
	//c = mix(c, vec4(1.0, 0.0, 0.0, 1.0), step(1.0, vertex.uv.x + width));
	//c = mix(c, vec4(1.0, 0.0, 0.0, 1.0), step(vertex.uv.y - width, 0.0));
	//c = mix(c, vec4(1.0, 0.0, 0.0, 1.0), step(1.0, vertex.uv.y + width));
	FragColor = c;
}