#version 410
layout (location = 0) in vec3 inPosition;

out vec3 fragTexCoords;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

void main()
{
    fragTexCoords = inPosition;
    gl_Position = projMatrix * viewMatrix * vec4(inPosition, 1.0);
}  