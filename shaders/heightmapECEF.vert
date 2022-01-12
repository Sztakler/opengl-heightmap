#version 330 core
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_shading_language_420pack : require

layout (location = 0) in vec3 aPosition;

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

    /* 2D map coordinates */
    vec3 pos = vec3( origin.x  - aPosition.x * delta,
                aPosition.y * scale,
                origin.y + aPosition.z * delta
                 );

    /* 3D globe coordinates
    * LLA (Latitude-Longitude-Altitude) to ECEF (Earth-Centered, Earth-Fixed) conversion
    * based on https://danceswithcode.net/engineeringnotes/geodetic_to_ecef/geodetic_to_ecef.html
    */

    float latitude = radians(pos.x);
    float altitude = pos.y;
    float longitude = radians(pos.z);

    if (aPosition.y <= -500 || aPosition.y >= 9000)
    {
        altitude = 0;
    }

    alt = altitude;

    // float f = 0.5; // flattening
    // float ls = atan( (1 - f) * (1 - f) * tan(latitude)); // lambda

    // float x = radius * cos(latitude) * cos(longitude) + altitude * cos(latitude) * cos(longitude);
    // float y = radius * cos(latitude) * sin(longitude) + altitude * cos(latitude) * sin(longitude);
    // float z = radius * sin(latitude) + altitude * sin(latitude);

    float x = (radius + altitude) * cos(latitude) * cos(longitude);
    float y = (radius + altitude) * cos(latitude) * sin(longitude);
    float z = (radius + altitude) * sin(latitude) * sin(latitude);

    positionECEF = vec3(x, y, z);

    // float a = 6378137.0;              //WGS-84 semi-major axis
    // float e2 = 6.6943799901377997e-3;  //WGS-84 first eccentricity squared

    // float n = a / sqrt(1 - e2 * sin(latitude) * sin(latitude));

    // positionECEF = vec3( (n + altitude) * cos(latitude) * cos(longitude) ,
    //                      (n + altitude) * cos(latitude) * sin(longitude),
    //                      (n * (1 - e2) + altitude) * sin(latitude) );


    gl_Position = projection * view * vec4(positionECEF, 1.0);
}