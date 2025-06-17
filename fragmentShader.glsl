#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in float useTexture;

out vec4 FragColor;

uniform sampler2D tex0;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform float ambientStrength = 0.3; // Burayý ekleyin (varsayýlan deðer 0.3)

void main()
{
    if (useTexture > 0.5) {
        // For textured objects (like walls)
        FragColor = texture(tex0, TexCoord);
    } else {
        // For non-textured objects with lighting
        // Ambient lighting - sabit deðer yerine uniform deðiþkeni kullanýn
        vec3 ambient = ambientStrength * lightColor; // ambientStrength = 0.3 deðerini silip, uniform deðiþkeni kullanýn

        // Diffuse lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // Specular lighting
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor;

        // Final color
        vec3 result = (ambient + diffuse + specular) * objectColor;
        FragColor = vec4(result, 1.0);
    }
}


