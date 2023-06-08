#version 330

in vec4 vCol;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 vColor;

out vec4 color;

const int MAX_POINT_LIGHTS = 10; //Sirve para que como usuario sepamos el limite de luces 
const int MAX_SPOT_LIGHTS = 11; //Son 8 luces max, menos 1 (..) quedan 7 luces para ocupar 
//PEro solo acepta 3 de cada tipo
//como hacer que me muestre mia 20 luces, cada una etc (sig práctica pared de ST)}b

struct Light
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight 
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};


struct SpotLight
{
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction) //Calculo de los coeficinetes difuso pero tomando en cuenta la posicion del otro
{
	vec4 ambientcolor = vec4(light.color, 1.0f) * light.ambientIntensity;
	//producto punto: coseno del ángulo entre los dos vectores y normalizar para que sus módulos sean 1
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffusecolor = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0f);
	
	vec4 specularcolor = vec4(0, 0, 0, 0);
	
	if(diffuseFactor > 0.0f)
	{//si hay diffuse color entonces existe specular color; dependemos de la posición de la cámara
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));
		
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			specularcolor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	return (ambientcolor + diffusecolor + specularcolor);
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

//nuevo
vec4 CalcPointLight(PointLight pLight)
{
	vec3 direction = FragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	
	vec4 color = CalcLightByDirection(pLight.base, direction);
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance +
						pLight.constant; //Esta es la ecuacion de segundo grado para la atenueación
						//No se puede tener cero por que es una ecuacion de segundo grado
	
	return (color / attenuation);
}




vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position); //Rayo de direccion 
	float slFactor = dot(rayDirection, sLight.direction);
	
	if(slFactor > sLight.edge) //La apertura 
	{
		vec4 color = CalcPointLight(sLight.base);
		
		return color * (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - sLight.edge)));
		
	} else {
		return vec4(0, 0, 0, 0);
	}
}










vec4 CalcPointLights()
{
	vec4 totalcolor = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{		
		totalcolor += CalcPointLight(pointLights[i]);
	}
	
	return totalcolor;
}

vec4 CalcSpotLights()
{
	vec4 totalcolor = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{		
		totalcolor += CalcSpotLight(spotLights[i]);
	}
	
	return totalcolor;
}


void main()
{
	vec4 finalcolor = CalcDirectionalLight();
	finalcolor += CalcPointLights();
	finalcolor += CalcSpotLights();
	color = finalcolor*texture(theTexture, TexCoord)*vColor;
	//Se debe mutiplicar nunca sumar 
}