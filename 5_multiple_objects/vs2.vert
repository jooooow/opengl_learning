#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out vec3 vertex_color;

uniform vec3 bias2;
uniform float vertex_rotation;

void main()
{
	float x1 = position.x;
	float y1 = position.y;
	float x2 = cos(vertex_rotation) * x1 - sin(vertex_rotation) * y1;
	float y2 = sin(vertex_rotation) * x1 + cos(vertex_rotation) * y1;
	gl_Position = vec4(vec3(x2,y2,0.0) + bias2, 1.0);
	vertex_color = color;
};