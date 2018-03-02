#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!
in vec2 texturePos;
in float ambient;
in float diffuse;
in float specular;

// Specify the Uniforms of the fragment shaders
// uniform vec3 lightPosition; // for example
uniform sampler2D sampler;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    vec3 textureColor = (texture2D(sampler, texturePos)).rgb;
    vec3 color = textureColor*(ambient + diffuse) + specular;
    fColor = vec4(color, 1.0);
}
