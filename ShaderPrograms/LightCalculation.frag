#version 330

//Material redefintion Will be ignored by our parsing algorithm. 
//I added it so i don't get any errors when using it in our function ( by the glsl extension )
//to ginore, you have to make this a one liner since we parse by line
struct Material { vec3 Color; float Shineness; float Specularity; float Diffuseness; };

#define SPOT_LIGHT        1
#define DIRECTIONAL_LIGHT 2
#define POINT_LIGHT       3
#define AMBIENT_LIGHT     4
#define NONE              0

struct Light
{
	int   Type;
	vec3  Position;
	float Intensity;
	vec3  Direction;

	//For spot light
	float ConeOpening;
	float PushToEdge;

	//Attenuation
	float Constant;
	float Linear;
	float Quadratic;
};

const   int   TotalLights  = 10;              //Our array size: the compiler expects the array size to be set at compile time AKA a constant
uniform int   UTotalLights = 0;               //we can control his to determine the actual number of lights in array to iterate over
uniform Light USceneLights[TotalLights];
uniform vec3  UCameraPosition = vec3(0);


vec3 GetLitColor( vec3 SurfacePosition, vec3 SurfaceNormal, Material ObjectMaterial  )
{
	float AmbientFactor = 0.0;

	vec3 LitColor = vec3(0.0, 0.0, 0.0);

	for(int i = 0; i < UTotalLights; i++)
	{
		Light CurrentLight = USceneLights[i];
		vec3 ToLightSource = vec3(0.0);

		float SpotLightMask = 1.0;

		if(CurrentLight.Type == POINT_LIGHT)
		{
			ToLightSource = normalize( CurrentLight.Position - SurfacePosition );
		}
		else if( CurrentLight.Type == DIRECTIONAL_LIGHT)
		{
			ToLightSource = normalize(-CurrentLight.Direction);
		}
		else if(CurrentLight.Type == AMBIENT_LIGHT)
		{
			AmbientFactor = CurrentLight.Intensity;
			continue;
		}
        else if( CurrentLight.Type == SPOT_LIGHT )
        {
			ToLightSource = normalize( CurrentLight.Position - SurfacePosition );

			float OntoLightDirection = max( dot( -ToLightSource, normalize(CurrentLight.Direction)), 0.0);            
            
            float DistanceFromCenter = sin(acos(OntoLightDirection));
            
            float Radius = sin( radians( CurrentLight.ConeOpening / 2.0) );
                        
            SpotLightMask = DistanceFromCenter / Radius;
            
            SpotLightMask = clamp(SpotLightMask, 0.0, 1.0); 
            
            SpotLightMask = pow(SpotLightMask, CurrentLight.PushToEdge); //raising a proper fraction to a positive exponent will further push it closer to 0.0
            
            
            SpotLightMask = 1.0 - SpotLightMask; 
            
           
            //Debugging
            //return vec3(SpotLightMask);
        }
		else continue; //No logic implemented yet for other light types

		float Distance    = length(CurrentLight.Position - SurfacePosition);
		float Attenuation = 1.0 / (CurrentLight.Constant + CurrentLight.Linear * Distance + 
    		    CurrentLight.Quadratic * (Distance * Distance));
		
		Attenuation = CurrentLight.Type == DIRECTIONAL_LIGHT ? 1.0 : Attenuation;

		//Diffuse
		float I    = max( dot(ToLightSource, SurfaceNormal ), 0.0) * CurrentLight.Intensity * SpotLightMask;
		LitColor  += ( ObjectMaterial.Color * I * Attenuation );

        //Specular
		if( ObjectMaterial.Shineness > 0.0 )
		{
			vec3  EyeRay = normalize(UCameraPosition - SurfacePosition);
			I = dot( normalize( reflect(-ToLightSource, SurfaceNormal) ), EyeRay ) ;

			I = max( I, 0.0) * CurrentLight.Intensity;
			I = pow( I, ObjectMaterial.Shineness ) * SpotLightMask;
			I *= Attenuation;

			LitColor += (ObjectMaterial.Color * I);

		}
	}

	//Should attenuation be added to ambient calculation?
	LitColor += ObjectMaterial.Color * AmbientFactor;

	return LitColor;
}
