/*
 * ShaderFontAlpha.cpp
 *
 *  Created on: May 11, 2012
 *      Author: prophesci
 */

#include "ShaderFontAlpha.h"
#include "Mesh.h"

namespace slh {

ShaderFontAlpha::ShaderFontAlpha() {
}

ShaderFontAlpha::~ShaderFontAlpha() {
}

char* ShaderFontAlpha::fshFileName() { return (char*)"font_alpha.fsh"; }

void ShaderFontAlpha::bindUniforms() {
	ShaderFont::bindUniforms();
    mTextureLoc[1] = glGetUniformLocation(muiProgram, "texture_2");
}


}
