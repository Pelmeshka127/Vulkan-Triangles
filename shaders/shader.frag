#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 viewPos;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 lightPos = vec3(50.0, 50.0, 50.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    float ambient_strength = 0.8;
    vec3 ambient = lightColor * ambient_strength;
    vec3 lightDir = normalize(lightPos - fragPos);

    float diffuse_strength = 0.8;
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diffuse_strength * diff * lightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float specularStrength = 0.8;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = fragColor * (ambient + diffuse + specular);
    
    vec3 dir = viewPos - fragPos;
    result = clamp(0.1 * result, result, result / dot(dir, dir) * 350);

    outColor = vec4(result, 1.0);
}