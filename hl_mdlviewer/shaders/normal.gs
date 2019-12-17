#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 2) out;

#include "uniform_blocks.in"

uniform float lineLength;

in VertexData {
    vec3 normal;
    int boneid;
} vertex[];

void main() {

    mat4 worldMatrix = g_Model * g_SceneTransform * g_BoneMatrices[vertex[0].boneid] * g_BoneOffsetMatrices[vertex[0].boneid];
    mat4 mvp = g_Projection * g_View * worldMatrix;

    gl_Position = mvp * gl_in[0].gl_Position;
    EmitVertex();
    
    gl_Position = mvp * vec4(gl_in[0].gl_Position.xyz + vertex[0].normal * lineLength, 1.0);
    EmitVertex();
    
    EndPrimitive();
}
