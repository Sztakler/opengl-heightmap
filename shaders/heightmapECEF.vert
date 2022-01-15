#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout (location = 0) in float height;

layout (location = 0) uniform mat4 model;
layout (location = 1) uniform mat4 view;
layout (location = 2) uniform mat4 projection;
layout (location = 3) uniform vec2 origin;

out vec3 positionECEF;
out float scale;
out float alt;

void main()
{
    float radius = 10;
    float delta = 1.0 / 1201.0;
    scale = 0.00001;

    /* Offsets in a hgt file (from upper left corner). */

    int index = gl_VertexID;
    int col = gl_VertexID % 1201;
    int row = gl_VertexID / 1201;

    /* 2D map coordinates */
    vec3 pos = vec3( origin.x  - row * delta,
                     height * scale,
                     origin.y + col  * delta
                   );

    /* 3D globe coordinates
    * LLA (Latitude-Longitude-Altitude) to ECEF (Earth-Centered, Earth-Fixed) conversion
    * based on https://danceswithcode.net/engineeringnotes/geodetic_to_ecef/geodetic_to_ecef.html
    */
    float latitude = radians(pos.x);
    float altitude = pos.y;
    float longitude = radians(pos.z);

    if (height <= -500 || height >= 9000)
    {
        altitude = 0;
    }

    alt = altitude;

    float z = (radius + altitude) * cos(latitude) * cos(longitude); // 3D x
    float x = (radius + altitude) * cos(latitude) * sin(longitude); // 3D y
    float y = (radius + altitude) * sin(latitude);                  // 3D z

    positionECEF = vec3(x, y, z);

    gl_Position = projection * view * vec4(positionECEF, 1.0);
}