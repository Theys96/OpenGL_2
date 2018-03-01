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
 uniform vec3 lightPosition;
 uniform vec4 materialColour = vec4(0.3,1,0.75,1);

// Specify the output of the vertex stage
out vec3 vertNormal;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = projectionTransform * modelViewTransform * vec4(vertCoordinates_in, 1.0);
    vertNormal = normalize(normalTransform * vertNormal_in);
    vertNormal = (materialColour * 0.2);
}
