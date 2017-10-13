#version 400
layout (location = 0) out vec4 FragColor;

void main()
{
	FragColor = vec4(gl_FragCoord.xyz, 1.0);
}