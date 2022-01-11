#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require
out vec4 FragColor;
in vec3 pos;
void main()
{
    FragColor = vec4(pos, 1.0);
    // FragColor = vec4(0.1176, 0.8392, 0.8039, 0.7);
}
