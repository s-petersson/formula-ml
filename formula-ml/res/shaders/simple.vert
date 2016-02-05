#version 400

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
//uniform mat4 modelMatrix;

in vec4 position;
in vec4 color;

out vec4 outColor;

void main() 
{
	gl_Position = projectionMatrix * viewMatrix * mat4(1.0) * position;
	outColor = color;
}