#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout (location = 0) in vec3 aPosition;

layout (location = 0) uniform mat4 model;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 projection;
layout (location = 3) uniform vec2 origin;

out vec3 pos;
out float scale;

void main()
{
    float delta = 1.0 / 1201.0;
    scale = 0.0001;

    // TODO
    // Coś tu nie działa, trzeba wydrukować w programie albo zapisać do pliku dane, które są tutaj wyliczane
    pos = vec3( origin.x  - aPosition.x * delta,
                aPosition.y * scale,
                origin.y + aPosition.z * delta
                 );

    if (aPosition.y <= -500 || aPosition.y >= 9000)
    {
        pos.y = -1;
    }

    gl_Position = projection * view * vec4(pos, 1.0);
}