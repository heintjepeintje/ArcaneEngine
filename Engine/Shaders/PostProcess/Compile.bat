@Echo Off

set "FLAGS=-D_DEBUG -g --target-env=opengl"

set "OUTPUT_DIR=Binaries/Output"

CALL glslc %FLAGS% -fshader-stage=vert Source/PostProcess.vert -o %OUTPUT_DIR%/PostProcess.vert.spv
CALL glslc %FLAGS% -fshader-stage=frag Source/PostProcess.frag -o %OUTPUT_DIR%/PostProcess.frag.spv