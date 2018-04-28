/* Renders using a colored texture */

// Vertex

#version 420 core

in vec3 aPos;
in vec2 aTexCoord;
in vec3 aNormal;
out vec4 vNormal;
out vec2 vTexCoord;
uniform mat4 uMat;

void main(void)
{
    vTexCoord = aTexCoord;
    vNormal = vec4(aNormal, 1.0);
    gl_Position = uMat * vec4(aPos, 1.0);
}

// Fragment

#version 420 core
layout(location = 0) out vec4 out_FragColor;

in vec4 vNormal; 
in vec2 vTexCoord; 
out vec4 fColor;
uniform sampler2D uSampler;
uniform vec4 uColor;

void main(void)
{
    out_FragColor = uColor * texture2D(uSampler, vTexCoord);
}
