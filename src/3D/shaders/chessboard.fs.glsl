#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;
uniform vec3 Kd; 
uniform vec3 Ka;
uniform float Ks;
uniform float Ns;

out vec4 FragColor;

void main()
{
    // vec4 texColor = texture(texture1, TexCoord);

    vec3 ambient = Ka * ((lightColor) * 0.05);

    vec3 V = normalize(viewPos - FragPos);
    vec3 N = normalize(Normal);
    vec3 L1 = normalize(lightPos - FragPos);

    float diff1 = max(dot(L1, N), 0.0);
    vec3 diffuse1 = diff1 * lightColor;

    vec3 H1 = normalize(L1 + V);
    float spec1 = pow(max(dot(N, H1), 0.0), Ns);
    vec3 specular1 = spec1 * Ks * lightColor;

    // FragColor = vec4(Kd, 1.0); // Set the color to the diffuse color
    // FragColor = texture(texture1, TexCoord);

    vec3 result = ambient + diffuse1 + specular1;
    FragColor = vec4(result, 1.0);

    // vec3 result = (ambient + diffuse1) * texColor.rgb + specular1;
    
    // FragColor = vec4(result, texColor.a);
}