
uniform vec3 eyePosition;
uniform sampler2D gPosition;
uniform sampler2D gNormal;

float getSpecular(vec3 normal, vec3 position, vec3 lightDirection)
{
	vec3 viewDirection = normalize(eyePosition-position);
	vec3 halfDirection = normalize(-viewDirection-lightDirection);
	float specFactor = dot(halfDirection, -normal);

	return 0.2*pow(max(specFactor,0),32);
}

float getDiffuse(vec3 normal, vec3 lightDirection)
{
	return 0.8*clamp(dot(normal, lightDirection), 0, 1);
}

vec4 getLighting(vec3 position, vec3 normal, 
				 vec3 lightDirection, vec3 lightColor)
{
	float intensity = getSpecular(normal, position, lightDirection)
		 			+ getDiffuse(normal, lightDirection);	

	return vec4(intensity*lightColor, 1.0); 
}

vec4 calcDirectionalLight(vec3 color, vec3 direction, vec2 uv)
{
	vec3 normal = texture2D(gNormal, uv).xyz;
	normal = normal*2.0-1.0;

	vec3 position = texture2D(gPosition, texCoord).xyz;
	position = position*2.0-1.0;
	
	return getLighting(position, normal,
					   direction, color);
}

vec4 calcPointLight(vec3 lightPosition, vec3 color, vec3 atten, vec2 uv)
{
	vec3 normal = texture2D(gNormal, uv).xyz;
	normal = normal*2.0-1.0;

	vec3 position = texture2D(gPosition, texCoord).xyz;
	position = position*2.0-1.0;

	vec3 lightDirection = eyePosition - lightPosition;
	float distance = length(lightDirection);
	lightDirection = lightDirection/distance;

	float attenuation = atten.x +
						atten.y*distance +
						atten.z*distance*distance*0.001;

	return getLighting(position, normal, 
					   lightDirection, color)/attenuation;
}
