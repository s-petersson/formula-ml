#version 400

in  vec4 position;
in  vec4 color;

out vec4 outColor;

void main() 
{
	gl_Position = position;
	outColor = color;
}