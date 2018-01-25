#version 410
layout(location = 0) out vec4 FragColor;

uniform vec4 Color;
uniform sampler2D MainTex;

const float width = 0.05;

in VERTEX{
	vec2 uv;
}vertex;

void main()
{
	vec4 c = texture(MainTex, vertex.uv);
	c.rgb *= Color.rgb;
	c.a = pow(c.a, 0.45455);
	//draw border
	//if(uv.x - width < 0.0 || uv.x + width > 1.0 || uv.y - width < 0 || uv.y + width > 1)
	//{
	//	c = vec4(1.0, 0.0, 0.0, 1.0);
	//}
	FragColor = c;
}