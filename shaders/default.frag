#version 330 core

in vec3 pos;

out vec4 FragColor;

void main()
{
    vec3 color;
    float ht = pos.y;

    if      (ht < 0   * 0.0001)   color = vec3(0.,       0.,        1.); //blue
    else if (ht < 500 * 0.0001)   color = vec3(0.,       ht/(500*0.0001),    0.); //->green
    else if (ht < 1000 * 0.0001)  color = vec3(ht/(500*0.0001)-1, 1.,        0.); //->yellow
    else if (ht < 2000 * 0.0001)  color = vec3(1.,       2.-ht/(1000*0.0001),0.); //->red
    else                 color = vec3(1.,       ht/(2000*0.0001)-1 ,ht/(2000*0.0001)-1);  //->white

    FragColor = vec4(color*0.8, 1.0f);
}