#version 330 core

#include "base.in"
#include "lighting.in"

vec3 g_vright = vec3(1.0f, 0.0f, 0.0f);

out vec2 frag_uv;
out float frag_intensity;

uniform bool useFlatShade;
uniform bool useChrome;
uniform bool applyBoneTransform;
uniform bool applyOffsetMatrix;

void main() {
    
    frag_uv = uv;
    
    mat4 worldMatrix = g_Model * g_SceneTransform;
    mat4 worldSpaceBoneTransform = worldMatrix * g_BoneMatrix;

    if (applyBoneTransform)
        worldMatrix = worldSpaceBoneTransform;
        
    if (applyOffsetMatrix)
        worldMatrix = worldMatrix * g_BoneOffsetMatrix;
        
    vec3 normalInLocalSpace = mat3(g_BoneOffsetMatrix) * normal;
    vec3 normalInWorldSpace = mat3(worldMatrix) * normal;

    gl_Position = g_Projection * g_View * worldMatrix * vec4(position, 1.0f);
        
    if (useChrome)
    {
        vec3 tmp = vec3(0,0,0);
        tmp += worldSpaceBoneTransform[3].xyz;
        tmp = normalize(tmp);
        
        vec3 g_vrightWorldSpace = mat3(g_Model) * g_vright;

        vec3 chromeupvec = cross(tmp, g_vrightWorldSpace);
        chromeupvec = normalize(chromeupvec);
        vec3 chromerightvec = cross(tmp, chromeupvec);
        chromerightvec = normalize(chromerightvec);
        mat3 worldMatrixTranspose = transpose(mat3(worldSpaceBoneTransform));
        vec3 g_chromeright = worldMatrixTranspose * chromerightvec;
        vec3 g_chromeup = worldMatrixTranspose * chromeupvec;
        float g_chrome_u = dot(normalInLocalSpace, g_chromeright);
        float g_chrome_v = dot(normalInLocalSpace, g_chromeup);
        frag_uv.x = (g_chrome_u + 1.0f) * 0.5f;
        frag_uv.y = (g_chrome_v + 1.0f) * 0.5f;
    }
    
    if (g_LightingEnabled)
        frag_intensity = calculateLightingIntensity(normalInWorldSpace, useFlatShade);  
}
