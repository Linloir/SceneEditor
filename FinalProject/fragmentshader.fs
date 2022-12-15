#version 430 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;  

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
// above 2 uniform can caculate objectColor

uniform vec3 lightPos; 
uniform vec3 lightColor;

void main()
{   
    vec3 objectColor = texture(texture_specular1, TexCoords).rgb;
    objectColor = texture(texture_diffuse1, TexCoords).rgb;
    // ambient
    float ambientStrength = 0;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}