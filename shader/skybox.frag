#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;
layout(location = 3) in vec2 fragUv;

void	main(void){
	outColor = texture(texSampler, fragUv);
}
