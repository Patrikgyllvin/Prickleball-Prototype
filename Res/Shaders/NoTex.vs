#version 330 core

layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec4 in_Col;

out vec4 col;

uniform mat4 MVP;

void main(void)
{
	gl_Position = MVP * vec4( in_Vertex, 1.0 );

	col = in_Col;
}