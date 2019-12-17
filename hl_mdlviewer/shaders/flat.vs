#version 330 core

#include "base.in"

out vec4 frag_color;

uniform bool applyBoneTransform;
uniform bool applyOffsetMatrix;
uniform float pointSize;

void main() {
    gl_PointSize = pointSize;

    mat4 worldMatrix = g_Model * g_SceneTransform;
    if (applyBoneTransform)
        worldMatrix = worldMatrix * g_BoneMatrix;
    if (applyOffsetMatrix)
        worldMatrix = worldMatrix * g_BoneOffsetMatrix;
        
    gl_Position = g_Projection * g_View * worldMatrix * vec4(position, 1.0);
}
