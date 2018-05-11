/* Puts the depth in location 0. */

// Vertex

#version 420 core

uniform mat4 uMatMVP;
in vec3 aPos;
in vec2 aTexCoord;
in vec3 aNormal;
out vec2 vTexCoord;

void main()
{
    vTexCoord = aTexCoord;
    gl_Position = uMatMVP * vec4(aPos, 1.0);
}

// Fragment

#version 420 core

uniform sampler2D uSampler;
in vec2 vTexCoord;
layout(location = 0) out float out_FragDepth;

void main()
{
    if (texture2D(uSampler, vTexCoord).a < 0.1)
        discard;
    
    out_FragDepth = gl_FragCoord.z;
}
