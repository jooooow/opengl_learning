#version 330 core

layout(location = 0) out vec4 color;

in vec3 vertex_color;
uniform float alpha2;

void main()
{
	color = vec4(vertex_color, alpha2);
};
