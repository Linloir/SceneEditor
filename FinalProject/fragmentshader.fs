#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 lightColor;

void main()
{    
    FragColor = texture(texture_specular1, TexCoords);
    FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(FragColor * vec4(lightColor,1.0f));
}