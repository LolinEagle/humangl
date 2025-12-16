#version 450

const float SHININESS = 512.0;

struct PointLight {
	vec4	position;
	vec4	color;
};

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;
layout(location = 3) in vec2 fragUv;
layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4		projection;
	mat4		view;
	mat4		invView;
	vec4		ambientLightColor;
	PointLight	pointLights[16];
	int			numLights;
	int			textureOn;
} ubo;

layout(binding = 1) uniform sampler2D texSampler;

layout(push_constant) uniform Push {
	mat4	modelMatrix;
	mat4	normalMatrix;
} push;

void	main(void){
	// 1. Calculus the diffuse light contribution using the ambient light color
	// 2. Store the specular light contribution and it sets it to zero initially
	// 3. Calculus surface normal vector using the normalized fragNormalWorld
	// 4. Retrieves the camera pos in world space by accessing the translation
	// 5. Calculus view direction vector from the fragment pos to the camera pos
	vec3	diffuseLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w;
	vec3	specularLight = vec3(0.0);
	vec3	surfaceNormal = normalize(fragNormalWorld);
	vec3	cameraPosWorld = ubo.invView[3].xyz;
	vec3	viewDirection = normalize(cameraPosWorld - fragPosWorld);

	for (int i = 0; i < ubo.numLights; i++){
		/* Diffuse lighting ************************************************* */
		// 1. Retrieves the current point light
		// 2. Calculus the direction to light vector
		// 3. Factor based on the inverse square law of light attenuation
		// 4. Normalized direction to light
		// 5. Cos of the angle of incidence between surface and directionToLight
		// 6. Intensity of light considering its color, intensity, attenuation
		// 7. Adds the light contribution from the current light to the total
		PointLight	light = ubo.pointLights[i];
		vec3	directionToLight = light.position.xyz - fragPosWorld;
		float	attenuation = 1.0 / dot(directionToLight, directionToLight);
		directionToLight = normalize(directionToLight);
		float	cosAngIncidence = max(dot(surfaceNormal, directionToLight), 0);
		vec3	intensity = light.color.xyz * light.color.w * attenuation;
		diffuseLight += intensity * cosAngIncidence;

		/* Specular lighting ************************************************ */
		// 1. Half-angle vector between directionToLight and viewDirection
		// 2. Calculates the blinnTerm specular term
		// 3. Clamps the blinnTerm to ensure it's within the valid range
		// 4. Applies the specular exponent to blinnTerm
		// 5. Adds the specular light contribution to the total specular light
		vec3	halfAngle = normalize(directionToLight + viewDirection);
		float	blinnTerm = dot(surfaceNormal, halfAngle);
		blinnTerm = clamp(blinnTerm, 0, 1);
		blinnTerm = pow(blinnTerm, SHININESS);
		specularLight += intensity * blinnTerm;
	}
	if (ubo.textureOn > 0)// Is true
		outColor = texture(texSampler, fragUv);
	else
		outColor = vec4((diffuseLight + specularLight + 0.5) * fragColor, 1.0);
}
