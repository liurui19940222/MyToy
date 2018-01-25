layout(location = 0) out vec4 FragColor;

uniform sampler2D u_MainTex;

in VERTEX{
	vec2 uv;
}vertex;

void main()
{
	vec4 color = texture2D(u_MainTex, vertex.uv);
	FragColor = color;
}