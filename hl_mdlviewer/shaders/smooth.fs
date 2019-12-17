#version 330 core

#include "uniform_blocks.in"

uniform vec4 color;
in float frag_intensity;
out vec4 frag_color;

void main() {

    frag_color = g_LightingEnabled 
        ? vec4(color.xyz * frag_intensity * g_LightColor.xyz, color.w) 
        : color;
}
