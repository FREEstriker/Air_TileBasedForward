glslangValidator.exe -t -V OpaqueShader.vert -IUtils/ -o Spv/OpaqueShader.vert.spv
glslangValidator.exe -t -V OpaqueShader.frag -IUtils/ -o Spv/OpaqueShader.frag.spv

glslangValidator.exe -t -V BackgroundShader.vert -IUtils/ -o Spv/BackgroundShader.vert.spv
glslangValidator.exe -t -V BackgroundShader.frag -IUtils/ -o Spv/BackgroundShader.frag.spv

glslangValidator.exe -t -V TransprantShader.vert -IUtils/ -o Spv/TransparentShader.vert.spv
glslangValidator.exe -t -V TransprantShader.frag -IUtils/ -o Spv/TransparentShader.frag.spv

glslangValidator.exe -t -V MirrorShader.vert -IUtils/ -o Spv/MirrorShader.vert.spv
glslangValidator.exe -t -V MirrorShader.frag -IUtils/ -o Spv/MirrorShader.frag.spv

glslangValidator.exe -t -V GlassShader.vert -IUtils/ -o Spv/GlassShader.vert.spv
glslangValidator.exe -t -V GlassShader.frag -IUtils/ -o Spv/GlassShader.frag.spv

::glslangValidator.exe -t -V TestShader.comp -IUtils/ -o Spv/TestShader.comp.spv

cmd
