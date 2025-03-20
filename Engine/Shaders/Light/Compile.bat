@Echo Off

set "FLAGS=-D_DEBUG -g --target-env=opengl"

set "OUTPUT_DIR=Binaries/Output"

CALL glslc %FLAGS% -fshader-stage=vert Source/LightShader.vert -o %OUTPUT_DIR%/LightShader.vert.spv
CALL glslc %FLAGS% -fshader-stage=frag Source/LightShader.frag -o %OUTPUT_DIR%/LightShader.frag.spv