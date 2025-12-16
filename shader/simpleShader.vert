#version 450

struct PointLight {
	vec4	position;
	vec4	color;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;
layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;
layout(location = 3) out vec2 fragUv;

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4		projection;
	mat4		view;
	mat4		invView;
	vec4		ambientLightColor;
	PointLight	pointLights[16];
	int			numLights;
	int			textureOn;
} ubo;

layout(push_constant) uniform Push {
	mat4	modelMatrix;
	mat4	normalMatrix;
} push;

void	main(void){
	// Transforms position into world space coordinates
	vec4	positionWorld = push.modelMatrix * vec4(position, 1.0);

	// 1. Calculates the vertex pos by applying projection and view matrices
	// 2. Normal is multiplied by this matrix and then normalized
	// 3. Assigns the position of the vertex in world space to fragPosWorld
	// 4. Pass the fragment color to the fragment shader.
	gl_Position = ubo.projection * ubo.view * positionWorld;
	fragNormalWorld = normalize(mat3(push.normalMatrix) * normal);
	fragPosWorld = positionWorld.xyz;
	fragColor = color;
	fragUv = uv;
}
