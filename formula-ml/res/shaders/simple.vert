#version 400

in  vec3 position;
in  vec3 color;

out vec3 outColor;

void main() 
{
	gl_Position = vec4(position,1);
	outColor = color;
}