@Echo Off

set "FLAGS=-D_DEBUG --target-env opengl --glsl-version 430 -g -e main"

set "OUTPUT_DIR=Binaries/Output"

CALL glslangValidator %FLAGS% -S vert -o %OUTPUT_DIR%/PBRShader.vert.spv Source/PBRShader.vert
CALL glslangValidator %FLAGS% -S frag -o %OUTPUT_DIR%/PBRShader.frag.spv Source/PBRShader.frag