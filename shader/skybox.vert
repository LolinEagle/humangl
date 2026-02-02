#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;
layout(location = 4) in uint texId;
layout(location = 3) out vec2 fragUv;

struct PointLight {
	vec4	position;
	vec4	color;
};
struct TexIdMapping {
	vec2	tl;
	vec2	br;
	uint	flags;
};

const uint  TMAP_NONE				= 0;
const uint  TMAP_FLIP_HORIZONTAL	= 1 << 0;
const uint  TMAP_FLIP_VERTICAL		= 1 << 1;
const uint  TMAP_ROT_90				= 1 << 2;
const uint  TMAP_ROT_180			= 1 << 3;
const uint  TMAP_ROT_270			= 1 << 4;

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

layout(set = 0, binding = 2) uniform TexIdMap {
	TexIdMapping texUv[128];
} texIdMap;


vec2 applyTransform(vec2 uv, uint flags)
{
	// Rotations
	if ((flags & TMAP_ROT_90) != 0u)		uv = vec2(1-uv.y,   uv.x);
	else if ((flags & TMAP_ROT_180) != 0u)	uv = vec2(1-uv.x, 1-uv.y);
	else if ((flags & TMAP_ROT_270) != 0u)	uv = vec2(  uv.y, 1-uv.x);
	// Flips
	if ((flags & TMAP_FLIP_HORIZONTAL) != 0u)	uv.x = 1-uv.x;
	if ((flags & TMAP_FLIP_VERTICAL) != 0u)		uv.y = 1-uv.y;

	return uv;
}

void	main(void){
	TexIdMapping map = texIdMap.texUv[texId];
	fragUv = mix(map.tl, map.br, applyTransform(uv, map.flags));
	// trim the last column (?) of the matrix -> removed rotation data
	mat4 viewNoRot = mat4(mat3(ubo.view));

	// Transforms position into world space coordinates
	vec4	positionWorld = push.modelMatrix * vec4(position, 1.0);

	vec4 pos = ubo.projection * ubo.view * positionWorld;

	gl_Position = pos.xyww;
	gl_Position.z *= 0.999;
}
