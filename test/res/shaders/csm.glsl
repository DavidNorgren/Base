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
uniform mat4 uMatLightV[NUM_CASCADES];
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
uniform ivec2 uDepthSamplerSize;
uniform float uCascadeEndClipSpace[NUM_CASCADES];
uniform float uCascadeSize[NUM_CASCADES];
uniform float uCascadeZnear[NUM_CASCADES];
uniform float uCascadeZfar[NUM_CASCADES];
uniform vec3 uEyePosition;

float znear = 5.f;
float zfar = 1500.f;
float zdis = zfar - znear;

float rand(float x)
{
    return fract(sin(dot(vec2(x * 10.3,  x * 10.3), vec2(12.9898, 78.233))) * 43758.5453);
}

const float lightbleed = 0.75f;
const float invlightbleed = 1.0f / (1.0f - lightbleed );

float chebyshevUpperBound(int i)
{
    // We retrive the two moments previously stored (depth and depth*depth)
    float dis = vShadowCoord[i].z;
    vec2 moments = texture2D(uDepthSampler[i], vShadowCoord[i].xy).rg;
    
    // Surface is fully lit. as the current fragment is before the light occluder
    if (dis < moments.x)
        return 1.0;

    // The fragment is either in shadow or penumbra. We now use chebyshev's upperBound to check
    // How likely this pixel is to be lit (p_max)
    float variance = max(moments.y - (moments.x * moments.x), 0.0001);
    float diff = dis - moments.x;
    float pMax = variance / (variance + diff * diff);
    pMax= clamp((pMax - lightbleed) * invlightbleed, 0, 1);// light bleeding calculation
    return pMax;
}

void main()
{
    // Find the cascade to use
    int i;
    for (i = 0; i < NUM_CASCADES; i++)
        if (vClipSpaceDepth < uCascadeEndClipSpace[i])
            break;

    float light = 1.0;
    float dif = max(0.00, dot(vNormal, uLightDir));
  
    if (dif > 0.0 && i < NUM_CASCADES)
    {
        float bias = 0.005 * tan(acos(dif));
        bias = clamp(bias, 0.0, 0.005);

        if (vShadowCoord[i].x > 0.0 && vShadowCoord[i].y > 0.0 && vShadowCoord[i].z > 0.0 &&
            vShadowCoord[i].x < 1.0 && vShadowCoord[i].y < 1.0 && vShadowCoord[i].z < 1.0)
        {
            light = chebyshevUpperBound(i);
        }
    }
    vec4 ambient = vec4(0.1, 0.2, 0.2, 1.0);
    vec4 baseColor = uColor * texture(uSampler, vTexCoord);
    out_FragColor = (ambient + light * dif) * baseColor;
}
