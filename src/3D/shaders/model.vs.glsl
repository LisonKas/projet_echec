#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aKd;
layout (location = 4) in vec3 aKs;
layout (location = 5) in vec3 aKa;
layout (location = 6) in float aNs;
layout (location = 7) in int aUseTexture;

// Uniformes pour les matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec3 Kd;
out vec3 Ks;
out vec3 Ka;
out float Ns;
flat out int useTexture;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    Kd = aKd;
    Ks = aKs;
    Ka = aKa;
    Ns = aNs;
    useTexture = aUseTexture;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}