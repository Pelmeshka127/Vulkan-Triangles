// #version 450

// layout(location = 0) in vec3 position;
// layout(location = 1) in vec3 color;
// layout(location = 2) in vec3 normal;

// layout(location = 0) out vec3 fragColor;

// layout(set = 0, binding = 0) uniform GlobalUbo {
//     mat4 projectionViewMatrix;
//     vec4 ambientLightColor;
//     vec3 lightPosition;
//     vec4 lightColor;
//     int  size;
// } ubo;

// layout(push_constant) uniform Push {
//     mat4 modelMatrix;
//     mat4 normalMatrix;
// } push;

// void main() {
//     vec4 positionWorld = push.modelMatrix * vec4(position, 1.0);

//     gl_Position = ubo.projectionViewMatrix * positionWorld;

//     vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

//     vec3 directionToLight = ubo.lightPosition - positionWorld.xyz;

//     float attenuation = 1.0 / (dot(directionToLight, directionToLight));

//     vec3 lightColor = ubo.lightColor.xyz - ubo.lightColor.w;

//     vec3 ambientLight = ubo.ambientLightColor.xyz * ubo.ambientLightColor.w * attenuation;

//     vec3 diffuseLighting = lightColor * max(dot(normalWorldSpace, normalize(directionToLight)), 0);

//     fragColor = ubo.size * (diffuseLighting + ambientLight) * color;
// }

#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 projectionViewMatrix;
    mat4 modelMatrix;
    vec3 inViewPos;
} ubo;

// layout(push_constant) uniform Push {
//     mat4 modelMatrix;
//     mat4 normalMatrix;
// } push;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 Normal;
layout(location = 2) out vec3 fragPos;
layout(location = 3) out vec3 viewPos;

void main () {

    gl_Position = ubo.projectionViewMatrix * ubo.modelMatrix * vec4(inPosition, 1.0);
    fragColor   = inColor;
    Normal      = normalize(mat3(ubo.modelMatrix) * inNormal);
    fragPos     = vec3(ubo.modelMatrix * vec4(inPosition, 1.0));
    viewPos     = ubo.inViewPos;

}