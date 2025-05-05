@Echo Off

set "FLAGS=-D_DEBUG -g --target-env=opengl"

set "OUTPUT_DIR=Binaries/Output"

CALL glslc %FLAGS% -fshader-stage=vert Source/Shadow.vert -o %OUTPUT_DIR%/Shadow.vert.spv
CALL glslc %FLAGS% -fshader-stage=frag Source/Shadow.frag -o %OUTPUT_DIR%/Shadow.frag.spv