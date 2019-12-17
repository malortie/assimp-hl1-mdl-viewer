#version 330 core

#include "uniform_blocks.in"

in vec2 frag_uv;
in float frag_intensity;
out vec4 color;

uniform bool masked;
uniform vec3 maskColor;

uniform sampler2D myTexture;

void main() {

    color =  texture(myTexture, frag_uv);
    
    if (masked && color.rgb == maskColor)
        discard;
        
    if (g_LightingEnabled)
        color = vec4(color.xyz * frag_intensity * g_LightColor.xyz, 1.0f);
}
