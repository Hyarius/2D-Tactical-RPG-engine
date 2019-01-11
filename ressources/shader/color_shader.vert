#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexColor;

out vec4 fragmentColor;

void main()
{	
	gl_Position.xyz = vertexPosition_modelspace;
	fragmentColor = vertexColor;
}