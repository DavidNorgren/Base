/* Renders with shadows. */

// Vertex

#version 420 core

const int NUM_CASCADES = 3;

in vec3 aPos;
in vec2 aTexCoord;
in vec3 aNormal;
out vec2 vTexCoord;
out vec3 vNormal;
out float vClipSpaceDepth;
out vec4 vShadowCoord[NUM_CASCADES];
uniform mat4 uMatM;
uniform mat4 uMatMVP;
uniform mat4 uMatLightMVP[NUM_CASCADES];
uniform mat4 uMatLightBiasMVP[NUM_CASCADES];

void main()
{
    vec4 pos = vec4(aPos, 1.0);
    vTexCoord = aTexCoord;
    vNormal = normalize((uMatM * vec4(aNormal, 0.0)).xyz);
    gl_Position = uMatMVP * pos;

    vClipSpaceDepth = gl_Position.z;
    for (int i = 0; i < NUM_CASCADES; i++)
        vShadowCoord[i] = uMatLightBiasMVP[i] * pos;
}

// Fragment

#version 420 core

const int NUM_CASCADES = 3;

in vec2 vTexCoord; 
in vec3 vNormal;
in float vClipSpaceDepth;
in vec4 vShadowCoord[NUM_CASCADES];
layout(location = 0) out vec4 out_FragColor;
uniform vec4 uColor;
uniform vec3 uLightDir;
uniform sampler2D uSampler;
uniform sampler2D uDepthSampler[NUM_CASCADES];
uniform float uCascadeEndClipSpace[NUM_CASCADES];

void main()
{
    int i;
    if (vClipSpaceDepth < uCascadeEndClipSpace[0])
        i = 0;
    else if (vClipSpaceDepth < uCascadeEndClipSpace[1])
        i = 1;
    else if (vClipSpaceDepth < uCascadeEndClipSpace[2])
        i = 2;
    else
        discard;

    float light = 1.0;
    float dif = max(0.00, dot(vNormal, uLightDir));
  
    if (dif > 0.0)
    {
        float bias = 0.003 * tan(acos(dif));
        bias = clamp(bias, 0.0, 0.003);

          if (vShadowCoord[i].x > 0.0 && vShadowCoord[i].y > 0.0 && vShadowCoord[i].z > 0.0 &&
              vShadowCoord[i].x < 1.0 && vShadowCoord[i].y < 1.0 && vShadowCoord[i].z < 1.0)
        {
            float sampleDepth = texture2D(uDepthSampler[i], vShadowCoord[i].xy).z;
            if (sampleDepth < vShadowCoord[i].z - bias)
                light = 0.0;
        }
    }
    vec4 ambient = vec4(0.1, 0.1, 0.2, 1.0);
    vec4 baseColor = uColor * texture2D(uSampler, vTexCoord);
    out_FragColor = (ambient + light * dif) * baseColor;
        
   // float debug = texture2D(uDepthSampler[i], vShadowCoord[i].xy).z;
    //out_FragColor = vec4(vec3(debug), 1.0);
}
