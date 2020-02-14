#version 330 core

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 in_UV;

uniform mat4 MVP;
uniform mat4 textureMatrix;

out vec4 col;
out vec2 uv;

void main(void)
{
	gl_Position = MVP * vec4(vertPosition, 1.0);

	col = color;
	uv = (textureMatrix * vec4(in_UV, 0.0, 1.0)).xy;
}