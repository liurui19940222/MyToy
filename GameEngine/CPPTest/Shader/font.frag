#version 400
layout(location = 0) out vec4 FragColor;

uniform vec4 Color;
uniform sampler2D MainTex;

in vec2 uv;
const float width = 0.01;

void main()
{
	vec4 c = texture(MainTex, uv);
	//if(uv.x < width || uv.x > 1 - width || uv.y < width || uv.y > 1 - width)
	//	c = vec4(1, 0, 0, 1);
	
	FragColor = vec4(Color.rgb * c.rgb, c.a);
}