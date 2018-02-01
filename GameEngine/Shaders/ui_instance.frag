layout(location = 0) out vec4 FragColor;

uniform sampler2D u_MainTex;

in VERTEX{
	vec2 uv;
	vec4 range;
	vec4 color;
}vertex;

void main()
{
	vec2 uv = vertex.uv.xy * vertex.range.zw + vertex.range.xy;
	vec4 color = texture2D(u_MainTex, uv) * vertex.color;
	FragColor = color;
}