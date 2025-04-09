#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

// Uniformes de textures et/ou couleurs
uniform sampler2D texture1;
uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Ns;

// Uniformes pour la lumière
uniform vec3 lightPos;
uniform vec3 lightColor;

// Autres Uniformes
uniform vec3 viewPos;
uniform bool useTexture;

out vec4 FragColor;

void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);
    vec3 L = normalize(lightPos - FragPos);
    vec3 H = normalize(L + V);
    
    vec3 ambient = Ka * lightColor * 0.05;
    
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = Kd * diff * lightColor;
    
    float spec = pow(max(dot(N, H), 0.0), Ns);
    vec3 specular = Ks * spec * lightColor;
    
    vec3 texColor;
    if (useTexture) {
        texColor = texture(texture1, TexCoord).rgb;
    } else {
        texColor = vec3(1.0); 
    }
    
    vec3 result = (ambient + diffuse) * texColor + specular;
    
    result = pow(result, vec3(1.0/2.2));
    
    FragColor = vec4(result, 1.0);
    // FragColor = vec4((Normal + 1.0) / 2.0, 1.0);
}