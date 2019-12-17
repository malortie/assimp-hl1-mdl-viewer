#version 330 core

#include "base.in"
#include "lighting.in"

out float frag_intensity;

uniform bool useFlatShade;
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
    
    if (g_LightingEnabled)
        frag_intensity = calculateLightingIntensity(mat3(worldMatrix) * normal, useFlatShade);
}
