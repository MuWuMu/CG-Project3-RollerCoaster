#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 ViewDir;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform samplerCube skybox;

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 1.0;
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 viewDir = normalize(ViewDir);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    // Reflection and Refraction
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, norm);
    vec3 refractColor = texture(skybox, R).rgb;
    vec3 reflectColor = texture(skybox, R).rgb;
    
    vec3 result = (ambient + diffuse + specular) * objectColor + reflectColor * 0.5 + refractColor * 0.5;
    FragColor = vec4(result, 1.0);
}