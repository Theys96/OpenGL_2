#version 330 core

// Define constants
#define M_PI 3.141593

#define AMBIENT 0.3
#define DIFFUSE 0.6
#define SPECULAR 0.6
#define EXPONENT 5

// Specify the inputs to the fragment shader
// These must have the same type and name!
in vec3 vertNormal;
in vec3 actualPosition;
in vec2 texturePos;

// Specify the Uniforms of the fragment shaders
// uniform vec3 lightPosition; // for example
uniform mat4 modelViewTransform;
uniform mat3 normalTransform;
uniform mat4 projectionTransform;
uniform vec3 lightPosition;
uniform vec3 materialColour;
uniform sampler2D sampler;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    vec3 textureColor = (texture(sampler, texturePos)).rgb;
    vec3 localLightPosition = (modelViewTransform * vec4(lightPosition, 1.0)).xyz;
    vec3 lightDirection = normalize(localLightPosition - actualPosition);
    vec3 ambient = AMBIENT * textureColor;
    vec3 diffuse = max(0, dot(vertNormal,lightDirection)) * DIFFUSE * textureColor;
    vec3 eyeDirection = normalize(actualPosition);
    float specular = SPECULAR * pow(max(0, dot(reflect(lightDirection, vertNormal), eyeDirection)), EXPONENT);

    fColor = vec4(ambient + diffuse + specular, 1.0);
}
