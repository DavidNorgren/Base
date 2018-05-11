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
const int NUM_PCF_SAMPLES = 16;
const int NUM_PCSS_BLOCKER_SAMPLES = 16;

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
uniform float uFrustumSize;
uniform vec3 uEyePosition;

vec2 poissonDisk[16] = vec2[](
    vec2(-0.94201624, -0.39906216 ),
    vec2(0.94558609, -0.76890725 ),
    vec2(-0.094184101, -0.92938870 ),
    vec2(0.34495938, 0.29387760 ),
    vec2(-0.91588581, 0.45771432 ),
    vec2(-0.81544232, -0.87912464 ),
    vec2(-0.38277543, 0.27676845 ),
    vec2(0.97484398, 0.75648379 ),
    vec2(0.44323325, -0.97511554 ),
    vec2(0.53742981, -0.47373420 ),
    vec2(-0.26496911, -0.41893023 ),
    vec2(0.79197514, 0.19090188 ),
    vec2(-0.24188840, 0.99706507 ),
    vec2(-0.81409955, 0.91437590 ),
    vec2(0.19984126, 0.78641367 ),
    vec2(0.14383161, -0.14100790 ) 
);

float rand(float x)
{
    return fract(sin(dot(vec2(x * 10.3,  x * 10.3), vec2(12.9898, 78.233))) * 43758.5453);
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

    // Find blockers
	int blockers = 0;
	float avgBlockerDistance = 0.0;
    float worldLightSize = 10.0;
    float uvLightSize = worldLightSize / uDepthSamplerSize.x;
	float searchWidth = uvLightSize * (vShadowCoord[i].z - 1.f) / vShadowCoord[i].z; // 1->zNear
	for (int s = 0; s < NUM_PCSS_BLOCKER_SAMPLES; s++)
	{
		float sampleDepth = texture(uDepthSampler[i], vShadowCoord[i].xy + poissonDisk[s] * searchWidth).r;
		if (sampleDepth < vShadowCoord[i].z - bias)
		{
			blockers++;
			avgBlockerDistance += sampleDepth;
		}
	}
    
    if (blockers < 1)
        return 0.0;
    
	avgBlockerDistance /= blockers;

	// Penumbra estimation
	float penumbraWidth = (vShadowCoord[i].z - avgBlockerDistance) / avgBlockerDistance;
	float uvRadius = (penumbraWidth * uvLightSize * 1.f) ;
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
            light = 1.0 - pcss(i, bias);
        }
    }
    vec4 ambient = vec4(0.1, 0.2, 0.2, 1.0);
    vec4 baseColor = uColor * texture(uSampler, vTexCoord);
    out_FragColor = (ambient + light * dif) * baseColor;
}
