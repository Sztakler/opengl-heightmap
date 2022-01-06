#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
out vec4 FragColor;

in vec3 objectColor;
in vec3 fragmentPosition;  
in vec3 Normal;  
in vec3 Color;
  
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewerPosition; 

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outCutOff;

};
vec3 CalcDirLight(DirLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir);

uniform Material material;
uniform SpotLight spotlight;
uniform DirLight directionlight;

#define NR_POINT_LIGHTS 6
uniform PointLight pointLights[NR_POINT_LIGHTS];

void main()
{
    Material objColor = Material(Color, Color * 3, vec3(1.0, 1.0, 1.0), 36.0);

    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(viewerPosition - fragmentPosition);
    vec3 lightDirection = normalize(spotlight.position - fragmentPosition);

    vec3 result = CalcDirLight(directionlight, objColor, norm, fragmentPosition, -viewDirection);
    result += calculateSpotLight(spotlight, objColor, norm, fragmentPosition, -viewDirection);

     for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], material, norm, fragmentPosition, viewDirection);
    }

    FragColor = vec4(result, 0.5);
} 

vec3 CalcDirLight(DirLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // /* Beer-Lambert Law https://graphics.fandom.com/wiki/Beer-Lambert_law */
    float depth = 23.0 - fragPos.y;
    float beer_intensity = pow(10.0, -1.0 * 0.0048 * depth * depth);

    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular + vec3(0.0314, 0.1686, 0.4275)) * beer_intensity;
}

vec3 CalcPointLight(PointLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    float depth = 23.0 - fragPos.y;
    float beer_intensity = pow(10.0, -1.0 * 0.004 * distance * depth );

    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular) + vec3(0.0314, 0.1686, 0.4275) * beer_intensity;
}

vec3 calculateSpotLight(SpotLight light, Material material, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outCutOff;
    float intensity = clamp((theta - light.outCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    // /* Beer-Lambert Law https://graphics.fandom.com/wiki/Beer-Lambert_law */
    float depth = 23.0 - fragPos.y;
    float beer_intensity = pow(10.0, -1.0 * 0.004 * distance * depth );
// vec3(0.8941, 0.7412, 0.9647)
    // ambient *= attenuation * intensity;
    // diffuse *= attenuation * intensity;
    // specular *= attenuation * intensity;
    return ( (ambient + diffuse + specular) * attenuation * 5 + vec3(0.0314, 0.1686, 0.4275) * beer_intensity ) * intensity;
    // return vec3(0.7804, 0.2784, 0.2784);
}