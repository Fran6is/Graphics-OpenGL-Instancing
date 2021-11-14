#version 330 core

layout (location = 0) in vec3 aPos;  
layout (location = 1) in vec3 aTexCoord;
layout (location = 2) in vec3 aNormals;

uniform mat4 UModel = mat4(1.f);
uniform mat4 UView  = mat4(1.f);
uniform mat4 UProjection = mat4(1.f);
uniform mat4 URotationMatrix = mat4(1.f);


out vec3 SurfacePosition;
out vec3 SurfaceNormal;
flat out int Material_ID;


void main(void)
{
	Material_ID = gl_InstanceID;

	float PI = acos(-1.0);
	float OffsetAngle = PI / 2.0 * float(gl_InstanceID);
	float Distance    = 50;

	vec3 Displacement = vec3( cos( OffsetAngle ) * Distance, 0.0, sin(OffsetAngle) * Distance );

	mat4 ModelMatrix    = UModel;
	ModelMatrix[3].xyz += Displacement;

	vec4 VertexPosition = ModelMatrix * vec4(aPos, 1.0f);

    SurfaceNormal   = (URotationMatrix * vec4(aNormals, 0.0)).xyz;
	SurfacePosition = VertexPosition.xyz;

	gl_Position = UProjection * UView * VertexPosition;
}