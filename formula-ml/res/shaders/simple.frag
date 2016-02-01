#version 400

in vec4 outColor;
out vec4 fragmentColor;

void main() 
{
	fragmentColor = outColor;
}