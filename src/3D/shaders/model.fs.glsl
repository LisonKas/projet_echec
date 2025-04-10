#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Kd;
in vec3 Ka;
in vec3 Ks;
in float Ns;
flat in int useTexture;

// Uniforme de texture
uniform sampler2D texture1;

// Uniformes pour la lumière
uniform vec3 lightPos1;
uniform vec3 lightColor1;
uniform vec3 lightPos2;
uniform vec3 lightColor2;

// Autres Uniformes
uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    vec3 L1 = normalize(lightPos1 - FragPos);
    vec3 L2 = normalize(lightPos2 - FragPos);
    vec3 H1 = normalize(L1 + V);
    vec3 H2 = normalize(L2 + V);
    
    vec3 ambient = Ka * ((lightColor1 + lightColor2) * 0.05);
    
    float diff1 = max(dot(N, L1), 0.0);
    vec3 diffuse1 = Kd * diff1 * lightColor1;
    float diff2 = max(dot(L2, N), 0.0);
    vec3 diffuse2 = Kd * diff2 * lightColor2;
    
    float spec1 = pow(max(dot(N, H1), 0.0), Ns);
    vec3 specular1 = Ks * spec1 * lightColor1;
    float spec2 = pow(max(dot(N, H2), 0.0), Ns);
    vec3 specular2 = Ks * spec2 * lightColor2;
    
    vec3 texColor;
    if (useTexture != 0) {
        vec2 uv = TexCoord;
        texColor = texture(texture1, uv).rgb;
    } else {
        texColor = vec3(1.0, 1.0, 1.0); 
    }
    
    vec3 result = (ambient + diffuse1 + diffuse2) * texColor + specular1 + specular2;
    
    result = pow(result, vec3(1.0/2.2));
    
    FragColor = vec4(result, 1.0);
}