#version 330 core

uniform vec2  UWindowDimension = vec2(0, 0);
uniform float UTime = 0;
uniform float UDeltaTime = 0;
uniform vec3  UObjectColor = vec3(1, 0, 0);


in vec3 SurfacePosition;
in vec3 SurfaceNormal;
flat in int  Material_ID;

struct Material
{
	vec3  Color;
	float Shineness;
	float Specularity;
	float Diffuseness;
};

const Material DefaultMaterial = Material( vec3(1.0, 0.0, 0.0), 0.0, 0.0, 0.0);

vec3 GetLitColor( vec3 SurfacePosition, vec3 SurfaceNormal, Material ObjectMaterial );

//INSERT

	//'INSERT' will append in the content of 'LightCalculation.frag'. 
	//If removed, calling 'GetLitColor()' will result in a shader compilation error since there's no definition for the function

uniform Material UObjectMaterials[5];

void main(void)
{
	vec3 LitColor = GetLitColor( SurfacePosition, normalize(SurfaceNormal), UObjectMaterials[Material_ID] );

	const float Gamma = 2.2;
	LitColor = pow(LitColor, vec3(1.0/Gamma));

	// vec3 adjNormal = dFdy(SurfaceNormal) - ( SurfaceNormal * -1.f) ;
	// float x = dot( adjNormal, vec3(1,0, 0) );

	gl_FragColor = vec4( LitColor, 1.0 );
}
