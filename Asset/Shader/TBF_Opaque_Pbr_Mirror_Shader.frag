#version 450
#extension GL_GOOGLE_include_directive: enable
#extension GL_EXT_shader_atomic_float: enable

#include "TBForwardLighting.glsl"

layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec3 inWorldPosition;
layout(location = 2) in vec3 inWorldNormal;

layout(location = 0) out vec4 ColorAttachment;

void main() 
{
    vec3 wNormal = normalize(inWorldNormal);
    vec3 wView = CameraWObserveDirection(inWorldPosition, cameraInfo.info);
    vec3 rmo = vec3(0.00, 1.00, 1.00);
    vec3 albedo = vec3(1.0, 1.0, 1.0);

    vec3 radiance = vec3(0, 0, 0);

    radiance += PbrLighting(lightInfos.mainLightInfo, inWorldPosition, wView, wNormal, albedo, rmo.x, rmo.y);
    for(int i = 0; i < opaqueLightIndexList.count; i++)
    {
        radiance += PbrLighting(lightInfos.ortherLightInfos[opaqueLightIndexList.indexes[i]], inWorldPosition, wView, wNormal, albedo, rmo.x, rmo.y);
    }

    vec3 iblRadiance;
    PBR_IBL_LIGHTING(iblRadiance, lightInfos.ambientLightInfo, inWorldPosition, wView, wNormal, albedo, rmo.x, rmo.y, irradianceCubeImage, prefilteredCubeImage, lutImage);
    radiance += iblRadiance;

    float shadowIntensity = GetShadowIntensity((cameraInfo.info.view * vec4(inWorldPosition, 1)).xyz, wNormal);

    ColorAttachment = vec4(radiance * rmo.z * (1 - shadowIntensity), 1);
}
