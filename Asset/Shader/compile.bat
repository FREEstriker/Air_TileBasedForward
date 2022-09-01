glslangValidator.exe -t -V F_Opaque_Wall_Shader.vert -IUtils/ -o Spv/F_Opaque_Wall_Shader.vert.spv
glslangValidator.exe -t -V F_Opaque_Wall_Shader.frag -IUtils/ -o Spv/F_Opaque_Wall_Shader.frag.spv

glslangValidator.exe -t -V TBF_Opaque_Wall_Shader.vert -IUtils/ -o Spv/TBF_Opaque_Wall_Shader.vert.spv
glslangValidator.exe -t -V TBF_Opaque_Wall_Shader.frag -IUtils/ -o Spv/TBF_Opaque_Wall_Shader.frag.spv

glslangValidator.exe -t -V F_Opaque_Mirror_Shader.vert -IUtils/ -o Spv/F_Opaque_Mirror_Shader.vert.spv
glslangValidator.exe -t -V F_Opaque_Mirror_Shader.frag -IUtils/ -o Spv/F_Opaque_Mirror_Shader.frag.spv

glslangValidator.exe -t -V TBF_Opaque_Mirror_Shader.vert -IUtils/ -o Spv/TBF_Opaque_Mirror_Shader.vert.spv
glslangValidator.exe -t -V TBF_Opaque_Mirror_Shader.frag -IUtils/ -o Spv/TBF_Opaque_Mirror_Shader.frag.spv

glslangValidator.exe -t -V F_Opaque_Glass_Shader.vert -IUtils/ -o Spv/F_Opaque_Glass_Shader.vert.spv
glslangValidator.exe -t -V F_Opaque_Glass_Shader.frag -IUtils/ -o Spv/F_Opaque_Glass_Shader.frag.spv

glslangValidator.exe -t -V TBF_Opaque_Glass_Shader.vert -IUtils/ -o Spv/TBF_Opaque_Glass_Shader.vert.spv
glslangValidator.exe -t -V TBF_Opaque_Glass_Shader.frag -IUtils/ -o Spv/TBF_Opaque_Glass_Shader.frag.spv

glslangValidator.exe -t -V Background_Skybox_Shader.vert -IUtils/ -o Spv/Background_Skybox_Shader.vert.spv
glslangValidator.exe -t -V Background_Skybox_Shader.frag -IUtils/ -o Spv/Background_Skybox_Shader.frag.spv

glslangValidator.exe -t -V F_Transparent_BrokenGlass_Shader.vert -IUtils/ -o Spv/F_Transparent_BrokenGlass_Shader.vert.spv
glslangValidator.exe -t -V F_Transparent_BrokenGlass_Shader.frag -IUtils/ -o Spv/F_Transparent_BrokenGlass_Shader.frag.spv

@REM glslangValidator.exe -t -V TBF_TransparentShader.vert -IUtils/ -o Spv/TBF_TransparentShader.vert.spv
@REM glslangValidator.exe -t -V TBF_TransparentShader.frag -IUtils/ -o Spv/TBF_TransparentShader.frag.spv

glslangValidator.exe -t -V GeometryShader.vert -IUtils/ -o Spv/GeometryShader.vert.spv
glslangValidator.exe -t -V GeometryShader.frag -IUtils/ -o Spv/GeometryShader.frag.spv

glslangValidator.exe -t -V TBF_OIT_DepthPeeling_Shader.vert -IUtils/ -o Spv/TBF_OIT_DepthPeeling_Shader.vert.spv
glslangValidator.exe -t -V TBF_OIT_DepthPeeling_Shader.frag -IUtils/ -o Spv/TBF_OIT_DepthPeeling_Shader.frag.spv

@REM glslangValidator.exe -t -V TBF_OIT_ALL_Shader.vert -IUtils/ -o Spv/TBF_OIT_ALL_Shader.vert.spv
@REM glslangValidator.exe -t -V TBF_OIT_ALL_Shader.frag -IUtils/ -o Spv/TBF_OIT_ALL_Shader.frag.spv

glslangValidator.exe -t -V TBF_OIT_DepthPeelingBlend_Shader.vert -IUtils/ -o Spv/TBF_OIT_DepthPeelingBlend_Shader.vert.spv
glslangValidator.exe -t -V TBF_OIT_DepthPeelingBlend_Shader.frag -IUtils/ -o Spv/TBF_OIT_DepthPeelingBlend_Shader.frag.spv

@REM glslangValidator.exe -t -V OIT_ALL_SortBlendShader.vert -IUtils/ -o Spv/OIT_ALL_SortBlendShader.vert.spv
@REM glslangValidator.exe -t -V OIT_ALL_SortBlendShader.frag -IUtils/ -o Spv/OIT_ALL_SortBlendShader.frag.spv

glslangValidator.exe -t -V SSAO_Occlusion_Shader.vert -IUtils/ -o Spv/SSAO_Occlusion_Shader.vert.spv
glslangValidator.exe -t -V SSAO_Occlusion_Shader.frag -IUtils/ -o Spv/SSAO_Occlusion_Shader.frag.spv

glslangValidator.exe -t -V AO_Blur_Shader.vert -IUtils/ -o Spv/AO_Blur_Shader.vert.spv
glslangValidator.exe -t -V AO_Blur_Shader.frag -IUtils/ -o Spv/AO_Blur_Shader.frag.spv

glslangValidator.exe -t -V AO_Cover_Shader.vert -IUtils/ -o Spv/AO_Cover_Shader.vert.spv
glslangValidator.exe -t -V AO_Cover_Shader.frag -IUtils/ -o Spv/AO_Cover_Shader.frag.spv

glslangValidator.exe -t -V PresentShader.vert -IUtils/ -o Spv/PresentShader.vert.spv
glslangValidator.exe -t -V PresentShader.frag -IUtils/ -o Spv/PresentShader.frag.spv

glslangValidator.exe -t -V TBF_LightList_Shader.comp -IUtils/ -o Spv/TBF_LightList_Shader.comp.spv

cmd
