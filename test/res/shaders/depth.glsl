/* Puts the depth in location 0. */

// Vertex

#version 420 core

uniform mat4 uMatMVP;
in vec3 aPos;
in vec2 aTexCoord;
in vec3 aNormal;
out vec2 vTexCoord;
out vec4 vPosition;

void main()
{
    vTexCoord = aTexCoord;
    gl_Position = uMatMVP * vec4(aPos, 1.0);
    vPosition = gl_Position;
}

// Fragment

#version 420 core

uniform sampler2D uSampler;
in vec2 vTexCoord;
in vec4 vPosition;
layout(location = 0) out vec4 out_FragColor;

void main()
{
    if (texture2D(uSampler, vTexCoord).a < 1.0)
        discard;
        
    float depth = vPosition.z / vPosition.w;
    depth = depth * 0.5 + 0.5;			//Don't forget to move away from unit cube ([-1,1]) to [0,1] coordinate system

    float moment1 = depth;
    float moment2 = depth * depth;

    // Adjusting moments (this is sort of bias per pixel) using partial derivative
    float dx = dFdx(depth);
    float dy = dFdy(depth);
    moment2 += 0.25 * (dx * dx + dy * dy);

    out_FragColor = vec4(moment1, moment2, 0.0, 1.0);
}
