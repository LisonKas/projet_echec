#version 330 core

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 Kd; // Diffuse color

out vec4 FragColor;

void main()
{
    FragColor = vec4(Kd, 1.0); // Set the color to the diffuse color
    // FragColor = texture(texture1, TexCoord);
}