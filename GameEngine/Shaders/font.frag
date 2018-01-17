#version 400
layout(location = 0) out vec4 FragColor;

uniform vec4 Color;
uniform sampler2D MainTex;

in vec2 uv;
const float width = 0.05;

void main()
{
	vec4 c = texture(MainTex, uv);
	c.rgb *= Color.rgb;
	c.a = pow(c.a, 0.45455);
	//if(uv.x - width < 0.0 || uv.x + width > 1.0 || uv.y - width < 0 || uv.y + width > 1)
	//{
	//	c = vec4(1.0, 0.0, 0.0, 1.0);
	//}
	FragColor = c;
}