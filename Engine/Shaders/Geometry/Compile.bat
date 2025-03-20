@Echo Off

set "FLAGS=-D_DEBUG -g --target-env=opengl"

set "OUTPUT_DIR=Binaries/Output"

CALL glslc %FLAGS% -fshader-stage=vert Source/GeometryShader.vert -o %OUTPUT_DIR%/GeometryShader.vert.spv
CALL glslc %FLAGS% -fshader-stage=frag Source/GeometryShader.frag -o %OUTPUT_DIR%/GeometryShader.frag.spv