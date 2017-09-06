#version 400
layout(location = 0) out vec4 FragColor;

uniform vec4 AddColor;
uniform vec4 Color;
uniform sampler2D MainTex;

in vec2 uv;

void main()
{
	vec4 c = texture(MainTex, uv);
	FragColor = vec4(c.rgb * Color.rgb + AddColor.rgb * c.a, 1.0);
}