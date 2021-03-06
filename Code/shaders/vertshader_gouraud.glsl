#version 330 core

// Define constants
#define M_PI 3.141593

#define AMBIENT 0.2
#define DIFFUSE 0.3
#define SPECULAR 0.4
#define EXPONENT 10

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormal_in;
layout (location = 2) in vec2 vertTexture_in;

// Specify the Uniforms of the vertex shader
uniform mat4 modelViewTransform;
uniform mat3 normalTransform;
uniform mat4 projectionTransform;
uniform vec3 lightPosition = vec3(2,2,2);
uniform vec3 materialColour = vec3(1,0.3,0.75);
uniform sampler2D sampler;

// Specify the output of the vertex stage
out vec2 texturePos;
out float ambient;
out float diffuse;
out float specular;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    vec4 position = modelViewTransform * vec4(vertCoordinates_in, 1.0);
    gl_Position = projectionTransform * position;
    vec3 actualPosition = position.xyz;
    vec3 localLightPosition = (modelViewTransform * vec4(lightPosition, 1.0)).xyz;
    vec3 vertNormal = normalize(normalTransform * vertNormal_in);
    vec3 lightDirection = normalize(localLightPosition - actualPosition);
    ambient = AMBIENT;
    diffuse = max(0, dot(vertNormal,lightDirection)) * DIFFUSE;
    vec3 eyeDirection = normalize(actualPosition);
    specular = SPECULAR * pow(max(0, dot(reflect(lightDirection, vertNormal), eyeDirection)), EXPONENT);
    texturePos = vertTexture_in;
}
