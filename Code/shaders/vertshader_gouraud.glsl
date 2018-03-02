#version 330 core

// Define constants
#define M_PI 3.141593

#define AMBIENT 0.4
#define DIFFUSE 0.8
#define SPECULAR 0.5
#define EXPONENT 20

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormal_in;

// Specify the Uniforms of the vertex shader
uniform mat4 modelViewTransform;
uniform mat3 normalTransform;
uniform mat4 projectionTransform;
uniform vec3 lightPosition = vec3(2,2,2);
uniform vec3 materialColour = vec3(1,0.3,0.75);

// Specify the output of the vertex stage
out vec3 vertNormal;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    vec4 position = modelViewTransform * vec4(vertCoordinates_in, 1.0);
    gl_Position = projectionTransform * position;
    vec3 actualPosition = position.xyz;
    vec3 localLightPosition = (modelViewTransform * vec4(lightPosition, 1.0)).xyz;

    vertNormal = normalize(normalTransform * vertNormal_in);
    vec3 lightDirection = normalize(localLightPosition - actualPosition);
    vec3 ambient = AMBIENT * materialColour;
    vec3 diffuse = max(0, dot(vertNormal,lightDirection)) * DIFFUSE * materialColour;
    vec3 eyeDirection = normalize(actualPosition);
    float specular = SPECULAR * pow(max(0, dot(reflect(lightDirection, vertNormal), eyeDirection)), EXPONENT);
    vertNormal = ambient + diffuse + specular;

}
