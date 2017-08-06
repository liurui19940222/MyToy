#version 400
layout(location = 0) out vec4 FragColor;

uniform vec4 Color;
uniform sampler2D MainTex;

in vec2 uv;

void main()
{
	vec3 c = texture(MainTex, uv).rgb * Color.rgb;
	FragColor = vec4(c, 1.0);
}