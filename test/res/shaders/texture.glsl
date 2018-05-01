/* Renders using a colored texture */

// Vertex

#version 420 core

in vec3 aPos;
in vec2 aTexCoord;
in vec3 aNormal;
out vec2 vTexCoord;
uniform mat4 uMatMVP;


void main()
{
    vTexCoord = aTexCoord;
    gl_Position = uMatMVP * vec4(aPos, 1.0);
}

// Fragment

#version 420 core

in vec2 vTexCoord; 
layout(location = 0) out vec4 out_FragColor;
uniform sampler2D uSampler;
uniform vec4 uColor;

void main()
{
    out_FragColor = uColor * texture2D(uSampler, vTexCoord);
}
