/* Renders with shadows. */

// Vertex

#version 420 core

in vec3 aPos;
in vec2 aTexCoord;
in vec3 aNormal;
out vec2 vTexCoord;
out vec4 vShadowCoord;
uniform mat4 uMatM;
uniform mat4 uMatMVP;
uniform mat4 uMatDepthBiasMVP;

void main()
{
    vTexCoord = aTexCoord;
    vShadowCoord = uMatDepthBiasMVP * vec4(aPos, 1.0);
    gl_Position = uMatMVP * vec4(aPos, 1.0);
}

// Fragment

#version 420 core

in vec2 vTexCoord; 
in vec4 vShadowCoord;
layout(location = 0) out vec4 out_FragColor;
uniform sampler2D uSampler;
uniform sampler2D uDepthSampler;
uniform vec4 uColor;

void main()
{
    float light = 1.0;
    float sampleDepth = texture2D(uDepthSampler, vShadowCoord.xy).z;

    float bias = 0.0005;
	if (vShadowCoord.x > 0.0 && vShadowCoord.y > 0.0 && vShadowCoord.x < 1.0 && vShadowCoord.y < 1.0)
        if (sampleDepth < vShadowCoord.z - bias)
            light = 0.0;

    vec4 ambient = vec4(0.05, 0.05, 0.1, 1.0);
    vec4 diff = uColor * texture2D(uSampler, vTexCoord);
    out_FragColor = ambient + light * diff;
}
