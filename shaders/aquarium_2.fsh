#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
out vec4 FragColor;

in vec3 objectColor;
in vec3 fragmentPosition;  
in vec3 Normal;  
  
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewerPosition; 

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant
    float linear;
    float quadratic;
    
    float cutOff;
    float outerCutOff; 
}


vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - fragmentPosition);
    
    float diff = max(dot(normal, lightDirection), 0.0);

    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular) * attenuation;
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDirection = normalize(-light.direction);

    float diff = max( dot(normal, lightDirection), 0.0);

    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow( max( dot(viewDirection, reflectDirection), 0.0), material.shininess );

    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - fragmentPosition);

    float diff = max(dot(normal, lightDirection), 0.0);

    vec3 reflectDirection = reflect(-light.position, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;

    return (ambient + diffuse + specular) * attenuation * intensity;
}

uniform Material material;
uniform DirectionalLight directionalLight;
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(viewerPosition - fragmentPosition);

    vec3 result = calculateDirectionalLight(directionalLight, norm, viewDirection);

    for (int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += calculatePointLight(pointLights[i], norm, fragmentPosition, viewDirection);
    }

    result += calculateSpotLight(spotLight, norm, fragmentPosition, viewDirection);

    FragColor = vec4(result, 1.0);
} 