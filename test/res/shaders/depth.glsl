/* Puts the depth in location 0. */

// Vertex

#version 420 core

in vec3 aPos;
in vec2 aTexCoord;
in vec3 aNormal;
uniform mat4 uMatMVP;

void main()
{
    gl_Position = uMatMVP * vec4(aPos, 1.0);
}

// Fragment

#version 420 core

layout(location = 0) out float out_FragDepth;

void main()
{
    out_FragDepth = gl_FragCoord.z;
}
