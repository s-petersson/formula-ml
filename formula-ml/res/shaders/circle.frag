#version 400

in vec4 out_color;
in vec2 out_uv;

out vec4 fragmentColor;

const float smoothing = 1.0/32.0;

void main() 
{
	float dist = length(out_uv - vec2(0.5, 0.5));
	float alpha = 1.0 - smoothstep(0.5 - smoothing, 0.5 + smoothing, dist);
    //float distance = texture(fontmap, out_uv).a;
    //float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
	//fragmentColor = texture(fontmap, out_uv);
	//fragmentColor = out_color;
	fragmentColor = out_color * vec4(1,1,1,alpha);
}