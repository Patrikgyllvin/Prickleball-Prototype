#version 330 core

in vec4 col;
in vec2 uv;

out vec4 color;

uniform sampler2D tex;

void main(void)
{
	vec4 Color = texture(tex, uv) * col;

    color = Color;
}
