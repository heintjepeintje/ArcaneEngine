@Echo Off

glslangValidator -V -G -S vert Source/BasicShader.vert -o Binaries/Output/Debug/BasicShader.vert.spv
glslangValidator -V -G -S frag Source/BasicShader.frag -o Binaries/Output/Debug/BasicShader.frag.spv