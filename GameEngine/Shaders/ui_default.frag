#version 400
layout(location = 0) out vec4 FragColor;

uniform vec4 u_AddColor;
uniform vec4 u_Color;
uniform sampler2D u_MainTex;

in vec2 uv;

void main()
{
	vec4 c = texture(u_MainTex, uv);
	FragColor = vec4(c.rgb * u_Color.rgb + u_AddColor.rgb * c.a, 1.0);
}