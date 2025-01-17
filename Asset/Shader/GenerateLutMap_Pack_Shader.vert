#version 450
#extension GL_GOOGLE_include_directive: enable

#include "BackgroundRendering.glsl"

layout(location = 0) in vec3 vertexPosition;

layout(push_constant) uniform PackInfo
{
    uint stepCount;
    uint resolution;
    uint placeholder0;
    uint placeholder1;
} packInfo;

void main() 
{
    gl_Position = vec4(vertexPosition, 1.0);
}
