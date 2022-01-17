#version 330 core
out vec4 FragColor;

in vec3 pos;
void main()
{
    // vec3 position = vec3( (pos.x + 90.0) / 180.0, 0, (pos.z + 180.0) / 360.0);
    // position.y = cos(position.x);

    FragColor = vec4(0.0, 0.0, 1.0, 1.0); //blue;
}