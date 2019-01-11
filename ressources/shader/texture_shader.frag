#version 330 core

in vec2 UV;
in float ALPHA;

out vec4 color;

uniform sampler2D myTextureSampler;

void main()
{
	color = texture( myTextureSampler, UV ).rgba;
	color.a = color.a * ALPHA;
}
