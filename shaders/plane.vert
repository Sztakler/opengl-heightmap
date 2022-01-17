#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout (location = 0) in vec3 aPosition;

layout (location = 0) uniform mat4 model;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 projection;

out vec3 pos;

void main()
{
    pos = vec3( (aPosition.x + 90.0) / 180.0, aPosition.y, (aPosition.z + 180.0) / 360.0);;

    gl_Position = projection * view * model * vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
}
