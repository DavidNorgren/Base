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
const int NUM_PCF_SAMPLES = 64;
const int NUM_PCSS_BLOCKER_SAMPLES = 64;
const bool LIGHT_PCSS = true;

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

vec2 poissonDisk[64] = vec2[]( // TODO uniform
	vec2(0.0617981, 0.07294159),
	vec2(0.6470215, 0.7474022),
	vec2(-0.5987766, -0.7512833),
	vec2(-0.693034, 0.6913887),
	vec2(0.6987045, -0.6843052),
	vec2(-0.9402866, 0.04474335),
	vec2(0.8934509, 0.07369385),
	vec2(0.1592735, -0.9686295),
	vec2(-0.05664673, 0.995282),
	vec2(-0.1203411, -0.1301079),
	vec2(0.1741608, -0.1682285),
	vec2(-0.09369049, 0.3196758),
	vec2(0.185363, 0.3213367),
	vec2(-0.1493771, -0.3147511),
	vec2(0.4452095, 0.2580113),
	vec2(-0.1080467, -0.5329178),
	vec2(0.1604507, 0.5460774),
	vec2(-0.4037193, -0.2611179),
	vec2(0.5947998, -0.2146744),
	vec2(0.3276062, 0.9244621),
	vec2(-0.6518704, -0.2503952),
	vec2(-0.3580975, 0.2806469),
	vec2(0.8587891, 0.4838005),
	vec2(-0.1596546, -0.8791054),
	vec2(-0.3096867, 0.5588146),
	vec2(-0.5128918, 0.1448544),
	vec2(0.8581337, -0.424046),
	vec2(0.1562584, -0.5610626),
	vec2(-0.7647934, 0.2709858),
	vec2(-0.3090832, 0.9020988),
	vec2(0.3935608, 0.4609676),
	vec2(0.3929337, -0.5010948),
	vec2(-0.8682281, -0.1990303),
	vec2(-0.01973724, 0.6478714),
	vec2(-0.3897587, -0.4665619),
	vec2(-0.7416366, -0.4377831),
	vec2(-0.5523247, 0.4272514),
	vec2(-0.5325066, 0.8410385),
	vec2(0.3085465, -0.7842533),
	vec2(0.8400612, -0.200119),
	vec2(0.6632416, 0.3067062),
	vec2(-0.4462856, -0.04265022),
	vec2(0.06892014, 0.812484),
	vec2(0.5149567, -0.7502338),
	vec2(0.6464897, -0.4666451),
	vec2(-0.159861, 0.1038342),
	vec2(0.6455986, 0.04419327),
	vec2(-0.7445076, 0.5035095),
	vec2(0.9430245, 0.3139912),
	vec2(0.0349884, -0.7968109),
	vec2(-0.9517487, 0.2963554),
	vec2(-0.7304786, -0.01006928),
	vec2(-0.5862702, -0.5531025),
	vec2(0.3029106, 0.09497032),
	vec2(0.09025345, -0.3503742),
	vec2(0.4356628, -0.0710125),
	vec2(0.4112572, 0.7500054),
	vec2(0.3401214, -0.3047142),
	vec2(-0.2192158, -0.6911137),
	vec2(-0.4676369, 0.6570358),
	vec2(0.6295372, 0.5629555),
	vec2(0.1253822, 0.9892166),
	vec2(-0.1154335, 0.8248222),
	vec2(-0.4230408, -0.7129914)
);

float znear = 5.f;
float zfar = 1500.f;
float zdis = zfar - znear;

float rand(float x)
{
    return fract(sin(dot(vec2(x * 10.3,  x * 10.3), vec2(12.9898, 78.233))) * 43758.5453);
}

float lightClipToEye(float zClip, int i)
{
    float eyeZnear = uCascadeZnear[i];
    float eyeZfar  = uCascadeZfar[i];
	return eyeZfar * eyeZnear / (eyeZfar - zClip * (eyeZfar - eyeZnear));   
}

float pcf(int i, float bias, float radius)
{
    float sum = 0.0;
    for (int s = 0; s < NUM_PCF_SAMPLES; s++)
    {
        float sampleDepth = texture(uDepthSampler[i], vShadowCoord[i].xy + poissonDisk[s] * radius).r;
        if (sampleDepth < vShadowCoord[i].z - bias)
            sum += 1.0;
    }

    return sum / NUM_PCF_SAMPLES;
}

float pcss(int i, float bias)
{
    // http://developer.download.nvidia.com/whitepapers/2008/PCSS_Integration.pdf

    float zEye = lightClipToEye(vShadowCoord[i].z, i);

    // Find blockers
    float lightWorldSize = 15.0;
    float pxPerUnit = 1.0; //uDepthSamplerSize.x / uCascadeSize[i];
    float lightUvSize = (lightWorldSize * pxPerUnit) / uDepthSamplerSize.x;
    float searchWidth = lightUvSize * (zEye - uCascadeZnear[i]) / zEye;

    int blockers = 0;
    float avgBlockerDistance = 0.0;
    for (int s = 0; s < NUM_PCSS_BLOCKER_SAMPLES; s++)
    {
        float sampleDepth = texture(uDepthSampler[i], vShadowCoord[i].xy + poissonDisk[s] * searchWidth).r;
        if (sampleDepth < vShadowCoord[i].z - 0.003)
        {
            blockers++;
            avgBlockerDistance += sampleDepth;
            break;
        }
    }

    if (blockers < 1)
        return 0.0;
	else if (blockers == NUM_PCSS_BLOCKER_SAMPLES)
		return 1.0;
    
    avgBlockerDistance /= blockers;
    float avgBlockerDistanceEye = lightClipToEye(avgBlockerDistance, i);

    // Penumbra estimation
    //float penumbraRatio = (zEye - avgBlockerDistanceEye) / avgBlockerDistanceEye;
    //float uvRadius = penumbraRatio * lightUvSize * uCascadeZnear[i] / zEye;
    float penumbraRatio = zEye - avgBlockerDistanceEye;
    float uvRadius = penumbraRatio * 0.00001;
    // Filtering
    return pcf(i, bias, uvRadius);
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
        float bias = 0.001 * tan(acos(dif));
        bias = clamp(bias, 0.0, 0.001);

        if (vShadowCoord[i].x > 0.0 && vShadowCoord[i].y > 0.0 && vShadowCoord[i].z > 0.0 &&
            vShadowCoord[i].x < 1.0 && vShadowCoord[i].y < 1.0 && vShadowCoord[i].z < 1.0)
        {
            if (LIGHT_PCSS)
                light = 1.0 - pcss(i, bias);
            else
            {
                float pxPerUnit = uDepthSamplerSize.x / uCascadeSize[i];
                float uvRadius = (2.0 * pxPerUnit) / uDepthSamplerSize.x;
                light = 1.0 - pcf(i, bias, uvRadius);
            }
        }
    }
    vec4 ambient = vec4(0.1, 0.2, 0.2, 1.0);
    vec4 baseColor = uColor * texture(uSampler, vTexCoord);
    out_FragColor = (ambient + light * dif) * baseColor;

    float debug = light;
    //out_FragColor = vec4(debug, 0.0, 0.0, 1.0);
}
