/*
 * ShaderType7.cpp
 *
 *  Created on: Jul 19, 2011
 *      Author: prophesci
 */

#include "ShaderType7.h"
#include "Mesh.h"

namespace slh {

ShaderType7::ShaderType7() {
}

ShaderType7::~ShaderType7() {
}

char* ShaderType7::vshFileName() { return (char*)"type7.vsh"; }
char* ShaderType7::fshFileName() { return (char*)"type7.fsh"; }

void ShaderType7::bindAttribs() {
	glBindAttribLocation(muiProgram, GL_IDX_VERTEX, "in_Position");
	glBindAttribLocation(muiProgram, GL_IDX_NORMAL, "in_Normal");
	glBindAttribLocation(muiProgram, GL_IDX_UV, "in_TexCoord");
	glBindAttribLocation(muiProgram, GL_IDX_WEIGHT, "in_Weights");
	glBindAttribLocation(muiProgram, GL_IDX_INF, "in_Infs");
}

void ShaderType7::bindUniforms() {
	mModelMatLoc = glGetUniformLocation(muiProgram, "modelMatrix"); // Get the location of our model matrix in the shader
    mNormalMatLoc = glGetUniformLocation(muiProgram, "normalMatrix"); // Get the location of our normal matrix in the shader
    mTextureLoc[0] = glGetUniformLocation(muiProgram, "texture_1");
    mInfMatLoc = glGetUniformLocation(muiProgram, "arrInfMatrix"); // Get the location of our influence matrices in the shader
}

void ShaderType7::setAnimation(Animation* iAnimation) {
	glUniformMatrix4fv(mInfMatLoc, MD_MAX_INFS, GL_FALSE, iAnimation->matGlInfs()); // Uploads influence matrices for vertex shader.
}

}
