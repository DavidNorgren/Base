/* Gaussian blur. */

// Vertex

#version 420 core

in vec3 aPos;
in vec2 aTexCoord;
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
uniform vec2 uBlurDir;

void main()
{
    vec2 blur = uBlurDir * 0.0;
	vec4 color = vec4(0.0);
	color += texture2D(uSampler, vTexCoord + vec2(-3.0 * blur.x, -3.0 * blur.y)) * 0.015625;
	color += texture2D(uSampler, vTexCoord + vec2(-2.0 * blur.x, -2.0 * blur.y)) * 0.09375;
	color += texture2D(uSampler, vTexCoord + vec2(-1.0 * blur.x, -1.0 * blur.y)) * 0.234375;
	color += texture2D(uSampler, vTexCoord) * 0.3125;
	color += texture2D(uSampler, vTexCoord + vec2(1.0 * blur.x,  1.0 * blur.y)) * 0.234375;
	color += texture2D(uSampler, vTexCoord + vec2(2.0 * blur.x,  2.0 * blur.y)) * 0.09375;
	color += texture2D(uSampler, vTexCoord + vec2(3.0 * blur.x, -3.0 * blur.y)) * 0.015625;
    out_FragColor = color;
}
