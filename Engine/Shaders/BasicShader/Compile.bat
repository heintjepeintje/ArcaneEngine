@Echo Off

ECHO "Compiling Vertex Shader..."
glslangValidator -V -G -S vert Source/BasicShader.vert -o Binaries/Output/Debug/BasicShader.vert.spv

ECHO "Compiling Fragment Shader..."
glslangValidator -V -G -S frag Source/BasicShader.frag -o Binaries/Output/Debug/BasicShader.frag.spv