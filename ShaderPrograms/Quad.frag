#version 330 core

in vec2 TexCoord;

uniform vec2  u_WindowDimension = vec2(0);
uniform float u_Time = 0.f;


void main(void)
{	
	float AspectRatio = u_WindowDimension.x/u_WindowDimension.y;

	vec2 uv = gl_FragCoord.xy - ( u_WindowDimension * 0.5);

	//Circle as Dot
	vec2  CirclePosition = vec2(0.0);
	float Radius = 2.0;

	float Circle = distance( CirclePosition, uv) / Radius;
	Circle = min(Circle, 1.0);
	float PushToEdge = 10.0;

	Circle = pow(Circle, PushToEdge);
	Circle = 1.0 - Circle;
	
	gl_FragColor = vec4( Circle, Circle, Circle, 0.3);
}

