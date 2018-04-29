/* Renders using the normals as colors. */

// Vertex

#version 420 core

in vec3 aPos;
in vec2 aTexCoord;
in vec3 aNormal;
out vec4 vColor;
out vec2 vTexCoord;
uniform mat4 uMatM;
uniform mat4 uMatMVP;

void main()
{
	vec3 norm = normalize((uMatM * vec4(aNormal, 0.0)).xyz);
    vColor = vec4((norm + vec3(1.0)) / 2.0, 1.0);
    gl_Position = uMatMVP * vec4(aPos, 1.0);
}

// Fragment

#version 420 core

in vec4 vColor; 
layout(location = 0) out vec4 out_FragColor;

void main()
{
    out_FragColor = vColor;
}
