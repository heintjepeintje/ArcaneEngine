@Echo Off

set "FLAGS=-D_DEBUG -g --target-env=opengl"

set "OUTPUT_DIR=Binaries/Output"

CALL glslc %FLAGS% -fshader-stage=vert Source/UIShader.vert -o %OUTPUT_DIR%/UIShader.vert.spv
CALL glslc %FLAGS% -fshader-stage=frag Source/UIShader.frag -o %OUTPUT_DIR%/UIShader.frag.spv