#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormal_in;

// Specify the Uniforms of the vertex shader
 uniform mat4 modelViewTransform;
 uniform mat3 normalTransform;
 uniform mat4 projectionTransform;
 uniform vec3 lightPosition = vec3(0,1,-0);
 uniform vec3 materialColour = vec3(0.3,1,0.75);

// Specify the output of the vertex stage
out vec3 vertNormal;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = projectionTransform * modelViewTransform * vec4(vertCoordinates_in, 1.0);
    vertNormal = normalize(normalTransform * vertNormal_in);
    vec3 lightDirection = normalize(lightPosition - vertCoordinates_in);
    vec3 ambient = normalize(materialColour * 0.2);
    vec3 diffuse = max(vec3(0.0,0.0,0.0),normalize(dot(vertNormal,lightDirection) * materialColour));
    vec3 eyeDirection = vertCoordinates_in;
    vec3 specular = pow(max(vec3(0.0,0.0,0.0), dot((2*reflect(vertNormal, lightDirection)) - lightDirection, eyeDirection)), vec3(5.0,5.0,5.0));
    vertNormal = ambient + diffuse + specular;

}
