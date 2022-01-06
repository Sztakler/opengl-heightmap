#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout (location = 0) in vec3 aPosition;

layout (location = 0) uniform mat4 model;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 projection;

layout (location = 3) uniform vec3 spherePosition;

void main()
{
    gl_Position = projection * view * model * vec4(aPosition + spherePosition, 1.0);
}