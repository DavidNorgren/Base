/* Renders with shadows. */

// Vertex

#version 420 core

in vec3 aPos;
in vec2 aTexCoord;
in vec3 aNormal;
out vec2 vTexCoord;
out vec3 vNormal;
out vec4 vShadowCoord;
uniform mat4 uMatM;
uniform mat4 uMatMVP;
uniform mat4 uMatDepthBiasMVP;

void main()
{
    vTexCoord = aTexCoord;
    vNormal = normalize((uMatM * vec4(aNormal, 0.0)).xyz);
    vShadowCoord = uMatDepthBiasMVP * vec4(aPos, 1.0);
    gl_Position = uMatMVP * vec4(aPos, 1.0);
}

// Fragment

#version 420 core

in vec2 vTexCoord; 
in vec3 vNormal;
in vec4 vShadowCoord;
layout(location = 0) out vec4 out_FragColor;
uniform sampler2D uSampler;
uniform sampler2D uDepthSampler;
uniform vec4 uColor;
uniform vec3 uLightDir;

void main()
{
    float light = 1.0;
    float dif = max(0.00, dot(vNormal, uLightDir));
    
    if (dif > 0.0)
    {
        float bias = 0.001 * tan(acos(dif));
        bias = clamp(bias, 0.0, 0.001);

        if (vShadowCoord.x > 0.0 && vShadowCoord.y > 0.0 && vShadowCoord.z > 0.0 &&
            vShadowCoord.x < 1.0 && vShadowCoord.y < 1.0 && vShadowCoord.z < 1.0)
        {
            float sampleDepth = texture2D(uDepthSampler, vShadowCoord.xy).z;
            if (sampleDepth < vShadowCoord.z - bias)
                light = 0.0;
        }
    }
    vec4 ambient = vec4(0.1, 0.1, 0.2, 1.0);
    vec4 baseColor = uColor * texture2D(uSampler, vTexCoord);
    out_FragColor = (ambient + light * dif) * baseColor;
}
