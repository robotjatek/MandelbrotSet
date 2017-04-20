#version 330
layout(location = 0) in vec3 modelspace;

void main()
{
	gl_Position.xyz = modelspace;
	gl_Position.w = 1.0;
}