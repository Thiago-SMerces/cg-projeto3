#version 410
out vec4 FragColor;

in vec3 fragTexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, fragTexCoords);
}