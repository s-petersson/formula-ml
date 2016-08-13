#version 400

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

in vec4 position;
in vec4 color;
in vec2 uv;

out vec4 out_color;
out vec2 out_uv;

void main() 
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
	out_color = color;
	out_uv = uv;
}